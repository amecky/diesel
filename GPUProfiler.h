#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <thread>
#include <chrono>

const int GTS_MAX = 64;

// this is mainly taken from here: http://reedbeta.com/blog/gpu-profiling-101/

void reportLastError(const char* method, HRESULT hr) {
	char msg[256];
	DWORD result = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&msg,
		255, NULL);
	if (result > 0) {
		char errorBuffer[1024];
		sprintf(errorBuffer, "%s - %s\n", method, msg);
		MessageBox(GetDesktopWindow(), errorBuffer, "ERROR", NULL);
		exit(-1);
	}
}

namespace ds {

	class GPUProfiler {

	public:
		GPUProfiler(ID3D11Device* device, ID3D11DeviceContext* context)
			: _device(device), _context(context), _lastFrame(-1), m_frameCountAvg(0), m_tBeginAvg(0.0f), _currentMax(1), _currFrame(0) {
			memset(m_adT, 0, sizeof(m_adT));
			memset(m_adTAvg, 0, sizeof(m_adT));
			memset(m_adTTotalAvg, 0, sizeof(m_adT));
		}

		bool Init() {
			// Create all the queries we'll need
			D3D11_QUERY_DESC queryDesc = { D3D11_QUERY_TIMESTAMP_DISJOINT, 0 };
			for (int i = 0; i < 2; ++i) {
				_device->CreateQuery(&queryDesc, &_disjointQuery[i]);
			}
			queryDesc.Query = D3D11_QUERY_TIMESTAMP;
			for (int i = 0; i < 2; ++i) {
				for (int gts = 0; gts < GTS_MAX; ++gts) {
					_device->CreateQuery(&queryDesc, &_timestampQuery[gts][i]);
				}
			}
			return true;
		}

		void Shutdown() {
			for (UINT64 i = 0; i < 2; ++i) {
				if (_disjointQuery[i]) {
					_disjointQuery[i]->Release();
				}
				for (int gts = 0; gts < GTS_MAX; ++gts) {
					if (_timestampQuery[gts][i]) {
						_timestampQuery[gts][i]->Release();					
					}
				}
			}
		}

		void beginFrame() {
			_currentMax = 0;
			_context->Begin(_disjointQuery[_currFrame]);
			Timestamp(0);
		}

		void Timestamp(int gts) {
			if ((gts + 1) > _currentMax) {
				_currentMax = gts + 1;
			}
			_context->End(_timestampQuery[gts][_currFrame]);
		}

		void endFrame() {
			Timestamp(_currentMax);
			_context->End(_disjointQuery[_currFrame]);
			++_currFrame &= 1;			
		}

		// Wait on GPU for last frame's data (not this frame's) to be available
		void WaitForDataAndUpdate(float totalSeconds) {
			if (_lastFrame < 0) {
				// Haven't run enough frames yet to have data
				_lastFrame = 0;
				return;
			}

			// Wait for data
			while (_context->GetData(_disjointQuery[_lastFrame], NULL, 0, 0) == S_FALSE) {
				//Sleep(1);
				std::this_thread::sleep_for(std::chrono::microseconds(200));
			}

			int iFrame = _lastFrame;
			++_lastFrame &= 1;

			D3D11_QUERY_DATA_TIMESTAMP_DISJOINT timestampDisjoint;
			HRESULT hr = _context->GetData(_disjointQuery[iFrame], &timestampDisjoint, sizeof(timestampDisjoint), 0);
			if (hr != S_OK) {
				reportLastError("-- Couldn't retrieve timestamp disjoint query data", hr);
				//printf("-- Couldn't retrieve timestamp disjoint query data\n");
				return;
			}

			if (timestampDisjoint.Disjoint) {
				// Throw out this frame's data
				printf("Timestamps disjoint\n");
				return;
			}

			UINT64 timestampPrev;
			if (_context->GetData(_timestampQuery[0][iFrame], &timestampPrev, sizeof(UINT64), 0) != S_OK) {
				printf("Couldn't retrieve timestamp query data for GTS 0");
				return;
			}

			for (int gts = 1; gts < _currentMax; ++gts) {
				UINT64 timestamp;
				if (_context->GetData(_timestampQuery[gts][iFrame], &timestamp, sizeof(UINT64), 0) != S_OK) {
					printf("Couldn't retrieve timestamp query data for GTS 6\n");
					return;
				}

				m_adT[gts] = float(timestamp - timestampPrev) / float(timestampDisjoint.Frequency);
				timestampPrev = timestamp;

				m_adTTotalAvg[gts] += m_adT[gts];
			}

			++m_frameCountAvg;
			if (totalSeconds > m_tBeginAvg + 0.5f) {				
				for (int gts = 0; gts < _currentMax; ++gts) {
					m_adTAvg[gts] = m_adTTotalAvg[gts] / m_frameCountAvg;
					m_adTTotalAvg[gts] = 0.0f;
				}
				m_frameCountAvg = 0;
				m_tBeginAvg = totalSeconds;
			}
		}

		float Dt(int gts) {
			return m_adT[gts];
		}
		float DtAvg(int gts) {
			return m_adTAvg[gts];
		}

		float getTotalTime() {
			float dTDrawTotal = 0.0f;
			for (int gts = 0; gts < _currentMax; ++gts) {
				dTDrawTotal += DtAvg(gts);
			}
			return dTDrawTotal;
		}
	protected:
		ID3D11Device* _device;
		ID3D11DeviceContext* _context;
		int _currFrame;
		int _lastFrame;
		int _currentMax;
		ID3D11Query* _disjointQuery[2];
		ID3D11Query* _timestampQuery[GTS_MAX][2];

		float m_adT[GTS_MAX];						// Last frame's timings (each relative to previous GTS)
		float m_adTAvg[GTS_MAX];					// Timings averaged over 0.5 second

		float m_adTTotalAvg[GTS_MAX];				// Total timings thus far within this averaging period
		int m_frameCountAvg;						// Frames rendered in current averaging period
		float m_tBeginAvg;							// Time at which current averaging period started
	};

}