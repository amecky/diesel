#pragma once
#include <stdint.h>
#define MATH_IMPLEMENTATION
#include "math.h"

typedef uint32_t RID;

const RID INVALID_RID = UINT32_MAX;
const RID NO_RID = UINT32_MAX - 1;

#ifndef DEGTORAD
#define DEGTORAD( degree ) ((degree) * (3.141592654f / 180.0f))
#endif
#ifndef RADTODEG
#define RADTODEG( radian ) ((radian) * (180.0f / 3.141592654f))
#endif

namespace ds {

	const float PI = 3.141592654f;

	typedef struct Color {
		union {
			float values[4];
			struct {
				float r;
				float g;
				float b;
				float a;
			};
		};
		Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
		Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
		Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.0f) {}

		operator float* () {
			return &values[0];
		}

		operator const float* () const {
			return &values[0];
		}

	} Color;

	enum BufferAttribute {
		POSITION,
		COLOR,
		TEXCOORD,
		NORMAL
	};

	enum BufferAttributeType {
		FLOAT,
		UINT_8
	};

	enum BufferType {
		STATIC,
		DYNAMIC
	};

	enum TextureAddressModes {
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MIRROR_ONCE
	};

	enum TextureFilters {
		POINT,
		LINEAR,
		ANISOTROPIC
	};

	enum PrimitiveTypes {
		POINT_LIST,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		LINE_LIST
	};

	struct VertexDeclaration {
		BufferAttribute attribute;
		BufferAttributeType type;
		uint8_t size;
	};

	enum BlendStates {
		ZERO,
		ONE,
		SRC_COLOR,
		INV_SRC_COLOR,
		SRC_ALPHA,
		INV_SRC_ALPHA,
		DEST_ALPHA,
		INV_DEST_ALPHA,
		DEST_COLOR,
		INV_DEST_COLOR,
		SRC_ALPHA_SAT,
		BLEND_FACTOR,
		INV_BLEND_FACTOR,
		SRC1_COLOR,
		INV_SRC1_COLOR,
		SRC1_ALPHA,
		INV_SRC1_ALPHA
	};

	// ---------------------------------------------------
	// The depth buffer state
	// ---------------------------------------------------
	enum DepthBufferState {
		ENABLED,
		DISABLED
	};

	// ---------------------------------------------------
	// Supported texture formats
	// FIXME: add much more!
	// ---------------------------------------------------
	enum TextureFormat {
		R8G8B8A8_UINT,
		R8G8B8A8_UNORM
	};

	// ---------------------------------------------------
	// The two valid formats of an index buffer
	// ---------------------------------------------------
	enum IndexType {
		UINT_16,
		UINT_32
	};

	// ---------------------------------------------------
	// The shader type
	// ---------------------------------------------------
	enum ShaderType {
		VERTEX,
		PIXEL,
		GEOMETRY
	};

	// ---------------------------------------------------
	// cull mode
	// ---------------------------------------------------
	enum CullMode {
		NONE = 1,
		FRONT = 2,
		BACK = 3
	};

	// ---------------------------------------------------
	// fill mode
	// ---------------------------------------------------
	enum FillMode {
		WIREFRAME = 2,
		SOLID = 3
	};

	// ---------------------------------------------------
	// Render settings 
	// ---------------------------------------------------
	typedef struct {
		uint16_t width;
		uint16_t height;
		Color clearColor;
		uint8_t multisampling;
		const char* title;
	} RenderSettings;

	bool init(const RenderSettings& settings);

	// vertex declaration / buffer input layout

	RID createVertexDeclaration(VertexDeclaration* decl, uint8_t num, RID shaderId);

	//! Sets the current vertex declaration (input buffer layout).
	/*!
		The engine(??) will track the selected vertex declaration
		and will prevent dupilcate settings

		\param rid the resource of the vertex declaration
	*/
	void setVertexDeclaration(RID rid);

	// constant buffer

	RID createConstantBuffer(int byteWidth);

	void updateConstantBuffer(RID rid, void* data, size_t size);

	void setVertexConstantBuffer(RID rid);

	void setPixelConstantBuffer(RID rid);

	void setGeometryConstantBuffer(RID rid);

	// index buffer

	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type);

	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type, void* data);

	RID createQuadIndexBuffer(int numQuads);

	void setIndexBuffer(RID rid);

	// vertex buffer

	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl);

	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl, void* data, int vertexSize);

	void setVertexBuffer(RID rid, uint32_t* stride, uint32_t* offset, PrimitiveTypes topology);

	void mapBufferData(RID rid, void* data, uint32_t size);

	// sampler state

	RID createSamplerState(TextureAddressModes addressMode, TextureFilters filter);

	void setSamplerState(RID rid);

	// blendstate

	RID createBlendState(BlendStates srcBlend, BlendStates srcAlphaBlend, BlendStates destBlend, BlendStates destAlphaBlend, bool alphaEnabled);	

	RID createAlphaBlendState(BlendStates srcBlend, BlendStates destBlend);

	void setBlendState(RID rid);

	void setBlendState(RID rid, float* blendFactor,uint32_t mask);

	// shader

	RID createShader();

	void loadVertexShader(RID shader, const char* csoName);

	void loadPixelShader(RID shader, const char* csoName);

	void loadGeometryShader(RID shader, const char* csoName);

	void setShader(RID rid);

	// texture

	RID createTexture(int width, int height, uint8_t channels, void* data, TextureFormat format);

	void setTexture(RID rid, ShaderType type);

	void setTexture(RID rid, ShaderType type, uint8_t slot);

	// render target

	RID createRenderTarget(uint16_t width, uint16_t height);

	// rasterizer state
	RID createRasterizerState(CullMode cullMode, FillMode fillMode, bool multiSample, bool scissor, float depthBias, float slopeDepthBias);

	void setDepthBufferState(DepthBufferState state);

	void drawIndexed(uint32_t num);

	void draw(uint32_t num);

	void begin();

	void end();

	bool isRunning();

	void shutdown();

	// view and projection matrix

	const matrix& getViewMatrix();

	const matrix& getProjectionMatrix();

	const matrix& getViewProjectionMatrix();

	void setViewPosition(const v3& viewPos);

	void lookAt(const v3& pos);

	void setProjectionMatrix(float fieldOfView, float aspectRatio);

	void setProjectionMatrix(float fieldOfView, float aspectRatio, float minDepth, float maxDepth);

	// input

	bool isKeyPressed(uint8_t key);

	v2 getMousePosition();

	bool isMouseButtonPressed(int button);	

	// timing

	uint64_t getElapsedTicks();

	double getElapsedSeconds();

	uint64_t getTotalTicks();

	double GetTotalSeconds();

	uint32_t getFrameCount();

	uint32_t getFramesPerSecond();

	// utils

	float random(float min, float max);

	const char* getLastError();

	// Batching class
	struct BatchDescriptor {
		uint16_t size;
		RID vertexBufferID;
		PrimitiveTypes primitiveType;		
		RID vertexDeclarationID;
		// FIXME: add index buffer
	};
	template<class T>
	class Batch {

	public:
		Batch(const BatchDescriptor& descriptor) : _data(0) , _descriptor(descriptor) {
			_size = descriptor.size;
			_data = new T[_size];
		}
		~Batch() {
			if (_data != 0) {
				delete[] _data;
			}
		}

		void add(const T& t) {
			if ((_size + 1) >= _size) {
				flush();
			}
			_data[_size++] = t;
		}

		void begin() {
			_size = 0;
			_calls = 0;
		}

		void flush() {
			if (_size > 0) {
				++_calls;
				unsigned int stride = sizeof(T);
				unsigned int offset = 0;
				ds::setVertexBuffer(_descriptor.vertexBufferID, &stride, &offset, _descriptor.primitiveType);
				ds::setVertexDeclaration(_descriptor.vertexDeclarationID);
				ds::mapBufferData(_descriptor.vertexBufferID, _data, _size * sizeof(T));
				ds::draw(_size);
				_size = 0;
			}
		}
		uint16_t getCalls() const {
			return _calls;
		}
	private:
		uint16_t _index;
		uint16_t _size;
		uint16_t _calls;
		T* _data;
		BatchDescriptor _descriptor;
	};
}

#ifdef DS_IMPLEMENTATION

#include <Windows.h>
#include <crtdbg.h>  
#include <d3d11.h>
#include <vector>
#include <random>

static void assert_fmt(char* expr_str, bool expr, char* file, int line, char* format, ...);

static void assert_fmt(char* file, int line, char* format, ...);

static void reportLastError(const char* fileName, int line, const char* method, HRESULT hr);

static void assert_result(const char* file, int line, HRESULT result, const char* msg);

#ifndef XASSERT
#define XASSERT(Expr, s, ...) do { assert_fmt(#Expr, Expr,__FILE__,__LINE__,s,__VA_ARGS__); } while(false);
#endif
#ifndef ASSERT_RESULT
#define ASSERT_RESULT(r,s) do { assert_result(__FILE__,__LINE__,r,s); } while(false);
#endif
#ifndef REPORT
#define REPORT(s,d) do { reportLastError(__FILE__,__LINE__,s,d); } while(false);
#endif

namespace ds {

	// ------------------------------------------------------
	// query refresh rate
	// ------------------------------------------------------
	// http://www.rastertek.com/dx11tut03.html
	static bool QueryRefreshRate(UINT screenWidth, UINT screenHeight, bool vsync, DXGI_RATIONAL* rational) {
		DXGI_RATIONAL refreshRate = { 0, 1 };
		if (vsync)
		{
			IDXGIFactory* factory;
			IDXGIAdapter* adapter;
			IDXGIOutput* adapterOutput;
			DXGI_MODE_DESC* displayModeList;

			// Create a DirectX graphics interface factory.
			HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Could not create DXGIFactory instance."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			hr = factory->EnumAdapters(0, &adapter);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapters."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			hr = adapter->EnumOutputs(0, &adapterOutput);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to enumerate adapter outputs."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			UINT numDisplayModes;
			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			displayModeList = new DXGI_MODE_DESC[numDisplayModes];
			//assert(displayModeList);

			hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList);
			if (FAILED(hr))
			{
				MessageBox(0,
					TEXT("Failed to query display mode list."),
					TEXT("Query Refresh Rate"),
					MB_OK);

				return false;
			}

			// Now store the refresh rate of the monitor that matches the width and height of the requested screen.
			for (UINT i = 0; i < numDisplayModes; ++i)
			{
				if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
				{
					refreshRate = displayModeList[i].RefreshRate;
				}
			}

			delete[] displayModeList;
			if (adapterOutput) {
				adapterOutput->Release();
			}
			if (adapter) {
				adapter->Release();
			}
			if (factory) {
				factory->Release();
			}
		}

		*rational = refreshRate;
		//LOG << "refresh: " << refreshRate.Numerator << " " << refreshRate.Denominator;
		return true;
	}

	// ------------------------------------------------------
	// Shader
	// ------------------------------------------------------
	struct Shader {
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11GeometryShader* geometryShader;
		void* vertexShaderBuffer;
		UINT bufferSize;
		ID3D11SamplerState* samplerState;
		ID3DBlob* geometryShaderBuffer;

		Shader() : vertexShader(0), pixelShader(0), vertexShaderBuffer(0), geometryShader(0), samplerState(0) {}

	};

	// ------------------------------------------------------
	// internal input layout 
	// ------------------------------------------------------
	struct InternalVertexDeclaration {
		ID3D11InputLayout* layout;
		int size;
	};

	// ------------------------------------------------------
	// internal texture 
	// ------------------------------------------------------
	struct InternalTexture {
		int width;
		int height;
		RID samplerState;
		ID3D11ShaderResourceView* srv;
	};

	// ------------------------------------------------------
	// internal index buffer 
	// ------------------------------------------------------
	struct InternalIndexBuffer {
		DXGI_FORMAT format;
		uint32_t numIndices;
		BufferType type;
		ID3D11Buffer* buffer;
	};

	struct RenderTarget {
		ID3D11Texture2D* texture;
		ID3D11RenderTargetView* view;
		ID3D11ShaderResourceView* srv;
		ID3D11Texture2D* depthTexture;
		ID3D11DepthStencilView* depthStencilView;
	};
	// ------------------------------------------------------
	// Internal context
	// ------------------------------------------------------
	typedef struct {
		HWND hwnd;
		HINSTANCE instance;
		uint16_t screenWidth;
		uint16_t screenHeight;
		Color clearColor;
		uint8_t multisampling;
		bool running;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;

		ID3D11Device* d3dDevice;
		ID3D11DeviceContext* d3dContext;
		IDXGISwapChain* swapChain;
		ID3D11RenderTargetView* backBufferTarget;

		ID3D11Texture2D* depthTexture;
		ID3D11DepthStencilView* depthStencilView;

		ID3D11DepthStencilState* depthDisabledStencilState;
		ID3D11DepthStencilState* depthEnabledStencilState;

		matrix viewMatrix;
		matrix projectionMatrix;
		matrix viewProjectionMatrix;

		std::vector<ID3D11Buffer*> constantBuffers;
		std::vector<InternalIndexBuffer> indexBuffers;
		std::vector<ID3D11Buffer*> buffers;
		std::vector<ID3D11SamplerState*> samplerStates;
		std::vector<ID3D11BlendState*> blendStates;
		std::vector<Shader*> shaders;
		std::vector<InternalVertexDeclaration> layouts;
		std::vector<ID3D11ShaderResourceView*> shaderResourceViews;
		std::vector<ID3D11RasterizerState*> rasterizerStates;

		v3 viewPosition;
		v3 lookAt;
		v3 up;

		int mouseButtonState[2];
		int keyState[256];

		DepthBufferState depthBufferState;

		// Timing
		LARGE_INTEGER timerFrequency;
		LARGE_INTEGER lastTime;
		// Derived timing data uses a canonical tick format. 
		uint64_t elapsedTicks;
		uint64_t totalTicks;
		uint64_t leftOverTicks;

		// Members for tracking the framerate. 
		uint32_t frameCount;
		uint32_t framesPerSecond;
		uint32_t framesThisSecond;
		uint64_t secondCounter;
		uint64_t maxDelta;

		char errorBuffer[256];
		bool broken;

		RID selectedShaderId;
		RID selectedVertexDeclaration;
		RID selectedIndexBuffer;
		RID selectedVertexBuffer;
		RID selectedRasterizerState;

		RID selectedPSTextures[16];
		RID selectedVSTextures[16];
		RID selectedGSTextures[16];

	} InternalContext;

	static InternalContext* _ctx;

	// -------------------------------------------------------
	// random
	// -------------------------------------------------------
	static std::mt19937 mt;

	static void init_random(unsigned long seed) {
		std::random_device r;
		std::seed_seq new_seed{ r(), r(), r(), r(), r(), r(), r(), r() };
		mt.seed(new_seed);
	}

	
	float random(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt);
	}

	const char* getLastError() {
		return _ctx->errorBuffer;
	}

	static const uint64_t TicksPerSecond = 10000000;

	static double TicksToSeconds(uint64_t ticks) { 
		return static_cast<double>(ticks) / TicksPerSecond; 
	}
	static uint64_t SecondsToTicks(double seconds) { 
		return static_cast<uint64_t>(seconds * TicksPerSecond); 
	}

	uint64_t getElapsedTicks() { 
		return _ctx->elapsedTicks;
	}
	double getElapsedSeconds() { 
		return TicksToSeconds(_ctx->elapsedTicks);
	}

	// Get total time since the start of the program. 
	uint64_t getTotalTicks() { 
		return _ctx->totalTicks;
	}
	double GetTotalSeconds() { 
		return TicksToSeconds(_ctx->totalTicks);
	}

	// Get total number of updates since start of the program. 
	uint32_t getFrameCount() { 
		return _ctx->frameCount;
	}

	// Get the current framerate. 
	uint32_t getFramesPerSecond() { 
		return _ctx->framesPerSecond;
	}

	static void reportLastError(const char* fileName, int line, const char* method, HRESULT hr) {
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
			sprintf_s(_ctx->errorBuffer,"file: %s (%d) method: %s - %s\n", fileName, line, method, msg);
			MessageBox(GetDesktopWindow(), _ctx->errorBuffer, "ERROR", NULL);
			_ctx->running = false;
			exit(-1);
		}
	}

	// ------------------------------------------------------
	// assert functions
	// ------------------------------------------------------
	static void assert_fmt(char* expr_str, bool expr, char* file, int line, char* format, ...) {
		if (!expr) {
			va_list args;
			va_start(args, format);
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			int written = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args);			
			MessageBox(_ctx->hwnd, buffer, "ERROR", NULL);
			va_end(args);
			exit(-1);
		}
	}

	static void assert_fmt(char* file, int line, char* format, ...) {
		va_list args;
		va_start(args, format);
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		int written = vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, format, args);
		MessageBox(_ctx->hwnd, buffer, "ERROR", NULL);
		va_end(args);
		exit(-1);
	}

	static void assert_result(HRESULT result, const char* msg) {
		if (FAILED(result)) {
			REPORT(msg, result);
		}
	}

	static void assert_result(const char* file, int line, HRESULT result, const char* msg) {
		if (FAILED(result)) {
			reportLastError(file, line, msg, result);
		}
	}
	// ------------------------------------------------------
	// is running
	// ------------------------------------------------------
	bool isRunning() {
		MSG msg = { 0 };
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);

		uint64_t timeDelta = currentTime.QuadPart - _ctx->lastTime.QuadPart;

		_ctx->lastTime = currentTime;
		_ctx->secondCounter += timeDelta;

		// Clamp excessively large time deltas (e.g. after paused in the debugger). 
		if (timeDelta > _ctx->maxDelta)
		{
			timeDelta = _ctx->maxDelta;
		}

		// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp. 
		timeDelta *= TicksPerSecond;
		timeDelta /= _ctx->timerFrequency.QuadPart;

		uint32_t lastFrameCount = _ctx->frameCount;
		
		// Variable timestep update logic. 
		_ctx->elapsedTicks = timeDelta;
		_ctx->totalTicks += timeDelta;
		_ctx->leftOverTicks = 0;
		_ctx->frameCount++;
		// Track the current framerate. 
		if (_ctx->frameCount != lastFrameCount) {
			_ctx->framesThisSecond++;
		}

		if (_ctx->secondCounter >= static_cast<uint64_t>(_ctx->timerFrequency.QuadPart))	{
			_ctx->framesPerSecond = _ctx->framesThisSecond;
			_ctx->framesThisSecond = 0;
			_ctx->secondCounter %= _ctx->timerFrequency.QuadPart;
		}

		return _ctx->running;
	}


	// ------------------------------------------------------
	// initialize device
	// ------------------------------------------------------
	static bool initializeDevice(const RenderSettings& settings) {
		_ctx->clearColor = settings.clearColor;
		_ctx->multisampling = settings.multisampling;
		RECT dimensions;
		GetClientRect(_ctx->hwnd, &dimensions);

		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
		};

		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);



		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = _ctx->screenWidth;
		swapChainDesc.BufferDesc.Height = _ctx->screenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_RATIONAL refreshRate;
		QueryRefreshRate(_ctx->screenWidth, _ctx->screenHeight, true, &refreshRate);
		swapChainDesc.BufferDesc.RefreshRate = refreshRate;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = _ctx->hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = _ctx->multisampling;
		swapChainDesc.SampleDesc.Quality = 0;

		unsigned int creationFlags = 0;

#ifdef _DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result;
		unsigned int driver = 0;

		for (driver = 0; driver < totalDriverTypes; ++driver) {
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags,
				featureLevels, totalFeatureLevels,
				D3D11_SDK_VERSION, &swapChainDesc, &_ctx->swapChain, &_ctx->d3dDevice, &_ctx->featureLevel, &_ctx->d3dContext);

			if (SUCCEEDED(result)) {
				_ctx->driverType = driverTypes[driver];
				break;
			}
		}

		if (FAILED(result)) {
			REPORT("Failed to create the Direct3D device!", result);
			return false;
		}

		ID3D11Texture2D* backBufferTexture;

		result = _ctx->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

		if (FAILED(result)) {
			REPORT("Failed to get the swap chain back buffer!",result);
			return false;
		}

		result = _ctx->d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &_ctx->backBufferTarget);

		if (backBufferTexture)
			backBufferTexture->Release();

		if (FAILED(result))
		{
			REPORT("Failed to create the render target view!", result);
			return false;
		}



		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = _ctx->screenWidth;
		depthTexDesc.Height = _ctx->screenHeight;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = _ctx->multisampling;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		result = _ctx->d3dDevice->CreateTexture2D(&depthTexDesc, 0, &_ctx->depthTexture);
		if (FAILED(result)) {
			REPORT("Failed to create the depth texture!", result);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		descDSV.Texture2D.MipSlice = 0;

		result = _ctx->d3dDevice->CreateDepthStencilView(_ctx->depthTexture, &descDSV, &_ctx->depthStencilView);
		if (FAILED(result)) {
			REPORT("Failed to create the depth stencil view!", result);
			return false;
		}

		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(_ctx->screenWidth);
		viewport.Height = static_cast<float>(_ctx->screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		_ctx->d3dContext->RSSetViewports(1, &viewport);

		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the state using the device.
		result = _ctx->d3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &_ctx->depthDisabledStencilState);
		if (FAILED(result)) {
			REPORT("failed to create depth stencil state", result);
			return false;
		}

		depthDisabledStencilDesc.DepthEnable = true;
		result = _ctx->d3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &_ctx->depthEnabledStencilState);
		if (FAILED(result)) {
			REPORT("failed to create depth stencil state", result);
			return false;
		}

		_ctx->viewMatrix = mat_identity();

		_ctx->viewPosition = v3(0, 0, -6);
		_ctx->lookAt = v3(0, 0, 0);
		_ctx->up = v3(0, 1, 0);
		_ctx->viewMatrix = mat_LookAtLH(_ctx->viewPosition, _ctx->lookAt, _ctx->up);

		float fieldOfView = PI / 4.0f;
		float screenAspect = (float)_ctx->screenWidth / (float)_ctx->screenHeight;
		_ctx->projectionMatrix = mat_PerspectiveFovLH(fieldOfView, screenAspect, 0.01f, 100.0f);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
		return true;
	}

	// ------------------------------------------------------
	// set view position
	// ------------------------------------------------------
	void setViewPosition(const v3& viewPos) {
		_ctx->viewPosition = viewPos;
		_ctx->viewMatrix = mat_LookAtLH(_ctx->viewPosition, _ctx->lookAt, _ctx->up);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// look at
	// ------------------------------------------------------
	void lookAt(const v3& pos) {
		_ctx->lookAt = pos;
		_ctx->viewMatrix = mat_LookAtLH(_ctx->viewPosition, _ctx->lookAt, _ctx->up);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// set projection matrix
	// ------------------------------------------------------
	void setProjectionMatrix(float fieldOfView, float aspectRatio) {
		_ctx->projectionMatrix = mat_PerspectiveFovLH(fieldOfView, aspectRatio, 0.01f, 100.0f);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// set projection matrix
	// ------------------------------------------------------
	void setProjectionMatrix(float fieldOfView, float aspectRatio, float minDepth, float maxDepth) {
		_ctx->projectionMatrix = mat_PerspectiveFovLH(fieldOfView, aspectRatio, minDepth, maxDepth);
		_ctx->viewProjectionMatrix = _ctx->viewMatrix * _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// internal windows procedure
	// ------------------------------------------------------
	bool update_input(RAWINPUT* raw) {
		HWND current = GetForegroundWindow();
		if (current == _ctx->hwnd) {
			if (raw->header.dwType == RIM_TYPEKEYBOARD) {
				const RAWKEYBOARD& rawKB = raw->data.keyboard;
				UINT virtualKey = rawKB.VKey;
				UINT scanCode = rawKB.MakeCode;
				UINT flags = rawKB.Flags;
				if (flags == 0) {
					_ctx->keyState[virtualKey] = 80;
				}
				else {
					_ctx->keyState[virtualKey] = 0;

				}
			}
			if (raw->header.dwType == RIM_TYPEMOUSE) {
				if (raw->data.mouse.ulButtons == 1) {
					_ctx->mouseButtonState[0] = 1;
				}
				if (raw->data.mouse.ulButtons == 2) {
					_ctx->mouseButtonState[0] = 0;
				}
				if (raw->data.mouse.ulButtons == 4) {
					_ctx->mouseButtonState[1] = 1;
				}
				if (raw->data.mouse.ulButtons == 8) {
					_ctx->mouseButtonState[1] = 0;
				}
			}
		}
		return true;
	}

	// ------------------------------------------------------
	// get mouse position
	// ------------------------------------------------------
	v2 getMousePosition() {
		v2 mp;
		tagPOINT p;
		if (GetCursorPos(&p)) {
			if (ScreenToClient(ds::_ctx->hwnd, &p)) {
				mp.x = static_cast<float>(p.x);
				mp.y = static_cast<float>(ds::_ctx->screenHeight - p.y);
			}
		}
		return mp;
	}

	// ------------------------------------------------------
	// is key pressed
	// ------------------------------------------------------
	bool isKeyPressed(uint8_t key) {
		return _ctx->keyState[key] == 80;
	}

	// ------------------------------------------------------
	// is mouse button pressed
	// ------------------------------------------------------
	bool isMouseButtonPressed(int button) {
		return _ctx->mouseButtonState[button] == 80;
	}

	// ------------------------------------------------------
	// internal windows procedure
	// ------------------------------------------------------
	LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_INPUT: {
				char buffer[sizeof(RAWINPUT)] = {};
				UINT size = sizeof(RAWINPUT);
				GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
				update_input(raw);
			}
			return 0;
			case WM_LBUTTONDOWN:
				_ctx->mouseButtonState[0] = 80;
				return 0;
			case WM_LBUTTONUP:
				_ctx->mouseButtonState[0] = 0;
				return 0;
			case WM_RBUTTONDOWN:
				_ctx->mouseButtonState[1] = 80;
				return 0;
			case WM_RBUTTONUP:
				_ctx->mouseButtonState[1] = 0;
				return 0;
			case WM_CLOSE:
				_ctx->running = false;
				return 0;
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	// ------------------------------------------------------
	// init
	// ------------------------------------------------------
	bool init(const RenderSettings& settings) {

		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
		flag |= _CRTDBG_CHECK_ALWAYS_DF; // Turn on CrtCheckMemory
										 //flag |= _CRTDBG_DELAY_FREEMEM_DF;
		_CrtSetDbgFlag(flag); // Set flag to the new value

		SetThreadAffinityMask(GetCurrentThread(), 1);

		_ctx = new InternalContext;
		_ctx->instance = GetModuleHandle(NULL);
		_ctx->screenWidth = settings.width;
		_ctx->screenHeight = settings.height;
		_ctx->clearColor = settings.clearColor;

		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WinProc;
		wndClass.hInstance = _ctx->instance;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "D11";
		if (!RegisterClassEx(&wndClass)) {
			REPORT("Failed to create window", GetLastError());
			return false;
		}
		RECT DesktopSize;
		GetClientRect(GetDesktopWindow(), &DesktopSize);
		_ctx->hwnd = CreateWindow("D11", "D11",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			(DesktopSize.right - settings.width) / 2, (DesktopSize.bottom - settings.height) / 2,
			settings.width, settings.height,
			NULL, NULL, _ctx->instance, NULL);

		if (!_ctx->hwnd) {
			REPORT("Failed to create window",GetLastError());
			return false;
		}
		RECT rect = { 0, 0, settings.width, settings.height };
		AdjustWindowRect(&rect, GetWindowLong(_ctx->hwnd, GWL_STYLE), FALSE);
		SetWindowPos(_ctx->hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER | SWP_NOMOVE);
		ShowWindow(_ctx->hwnd, SW_SHOW);
		UpdateWindow(_ctx->hwnd);

		SetWindowText(_ctx->hwnd, settings.title);

		RAWINPUTDEVICE Rid[2];

		// Keyboard
		Rid[0].usUsagePage = 1;
		Rid[0].usUsage = 6;
		Rid[0].dwFlags = 0;
		Rid[0].hwndTarget = NULL;

		// Mouse
		Rid[1].usUsagePage = 1;
		Rid[1].usUsage = 2;
		Rid[1].dwFlags = 0;
		Rid[1].hwndTarget = NULL;

		RegisterRawInputDevices(Rid, 2, sizeof(RAWINPUTDEVICE));

		for (int i = 0; i < 256; ++i) {
			_ctx->keyState[i] = 0;
		}
		_ctx->mouseButtonState[0] = 0;
		_ctx->mouseButtonState[1] = 0;

		init_random(GetTickCount());

		QueryPerformanceFrequency(&_ctx->timerFrequency);
		QueryPerformanceCounter(&_ctx->lastTime);

		_ctx->leftOverTicks = 0;
		_ctx->framesPerSecond = 0;
		_ctx->framesThisSecond = 0;
		_ctx->secondCounter = 0;
		_ctx->maxDelta = _ctx->timerFrequency.QuadPart / 10;
		for (int i = 0; i < 256; ++i) {
			_ctx->errorBuffer[i] = '\0';
		}
		return initializeDevice(settings);
	}

	// ------------------------------------------------------
	// shutdown
	// ------------------------------------------------------
	void shutdown() {
		if (_ctx != 0) {
			for (size_t i = 0; i < _ctx->constantBuffers.size(); ++i) {
				_ctx->constantBuffers[i]->Release();
			}
			for (size_t i = 0; i < _ctx->buffers.size(); ++i) {
				_ctx->buffers[i]->Release();
			}
			for (size_t i = 0; i < _ctx->samplerStates.size(); ++i) {
				_ctx->samplerStates[i]->Release();
			}
			for (size_t i = 0; i < _ctx->blendStates.size(); ++i) {
				_ctx->blendStates[i]->Release();
			}
			for (size_t i = 0; i < _ctx->layouts.size(); ++i) {
				_ctx->layouts[i].layout->Release();
			}
			for (size_t i = 0; i < _ctx->shaderResourceViews.size(); ++i) {
				_ctx->shaderResourceViews[i]->Release();
			}
			for (size_t i = 0; i < _ctx->indexBuffers.size(); ++i) {
				_ctx->indexBuffers[i].buffer->Release();
			}
			for (size_t i = 0; i < _ctx->rasterizerStates.size(); ++i) {
				_ctx->rasterizerStates[i]->Release();
			}
			for (size_t i = 0; i < _ctx->shaders.size(); ++i) {
				Shader* s = _ctx->shaders[i];
				if (s->vertexShader != 0) {
					s->vertexShader->Release();
				}
				if (s->pixelShader != 0) {
					s->pixelShader->Release();
				}
				if (s->geometryShader != 0) {
					s->geometryShader->Release();
				}
			}
			// FIXME: release shaders
			if (_ctx->backBufferTarget) _ctx->backBufferTarget->Release();
			if (_ctx->swapChain) _ctx->swapChain->Release();
			if (_ctx->d3dContext) _ctx->d3dContext->Release();
			if (_ctx->depthStencilView) _ctx->depthStencilView->Release();
			if (_ctx->depthTexture) _ctx->depthTexture->Release();
			if (_ctx->depthDisabledStencilState) _ctx->depthDisabledStencilState->Release();
			if (_ctx->depthEnabledStencilState) _ctx->depthEnabledStencilState->Release();
			if (_ctx->d3dDevice) _ctx->d3dDevice->Release();
			delete _ctx;
		}
	}

	// ------------------------------------------------------
	// get view matrix
	// ------------------------------------------------------
	const matrix& getViewMatrix() {
		return _ctx->viewMatrix;
	}

	// ------------------------------------------------------
	// get projection matrix
	// ------------------------------------------------------
	const matrix& getProjectionMatrix() {
		return _ctx->projectionMatrix;
	}

	// ------------------------------------------------------
	// get view projection matrix
	// ------------------------------------------------------
	const matrix& getViewProjectionMatrix() {
		return _ctx->viewProjectionMatrix;
	}

	// ------------------------------------------------------
	// begin rendering
	// ------------------------------------------------------
	void begin() {
		_ctx->d3dContext->OMSetRenderTargets(1, &_ctx->backBufferTarget, _ctx->depthStencilView);
		_ctx->d3dContext->ClearRenderTargetView(_ctx->backBufferTarget, _ctx->clearColor);
		_ctx->d3dContext->ClearDepthStencilView(_ctx->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthEnabledStencilState, 1);
		_ctx->depthBufferState = DepthBufferState::ENABLED;
		_ctx->selectedShaderId = INVALID_RID;
		_ctx->selectedVertexDeclaration = INVALID_RID;
		_ctx->selectedIndexBuffer = INVALID_RID;
		_ctx->selectedRasterizerState = INVALID_RID;
		_ctx->selectedVertexBuffer = INVALID_RID;
		for (int i = 0; i < 16; ++i) {
			_ctx->selectedVSTextures[i] = NO_RID;
			_ctx->selectedPSTextures[i] = NO_RID;
			_ctx->selectedGSTextures[i] = NO_RID;
		}
	}

	// ------------------------------------------------------
	// end rendering
	// ------------------------------------------------------
	void end() {
		_ctx->swapChain->Present(0, 0);
	}

	static const char* DXBufferAttributeNames[] = {
		"POSITION",
		"COLOR",
		"TEXCOORD",
		"NORMAL"
	};

	struct DXBufferAttributeType {
		BufferAttributeType type;
		DXGI_FORMAT format;
		int size;
		int bytes;
	};

	static const DXBufferAttributeType DXBufferAttributeTypes[] = {
		{ FLOAT, DXGI_FORMAT_R32G32_FLOAT , 2, 8 },
		{ FLOAT, DXGI_FORMAT_R32G32B32_FLOAT , 3, 12},
		{ FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 16 },

	};

	static int find_format(BufferAttributeType type, int size) {
		for (int i = 0; i < 3; ++i) {
			if (DXBufferAttributeTypes[i].type == type && DXBufferAttributeTypes[i].size == size) {
				return i;
			}
		}
		return -1;
	}
	// ------------------------------------------------------
	// input layout / vertex declaration
	// ------------------------------------------------------
	RID createVertexDeclaration(VertexDeclaration* decl, uint8_t num, RID shaderId) {
		D3D11_INPUT_ELEMENT_DESC* descriptors = new D3D11_INPUT_ELEMENT_DESC[num];
		uint32_t index = 0;
		uint32_t counter = 0;
		int si[8] = { 0 };
		for (int i = 0; i < num; ++i) {
			D3D11_INPUT_ELEMENT_DESC& desc = descriptors[i];
			const VertexDeclaration& current = decl[i];
			int fidx = find_format(decl[i].type, decl[i].size);
			if (fidx == -1) {
				return INVALID_RID;
			}
			const DXBufferAttributeType& formatType = DXBufferAttributeTypes[fidx];
			desc.SemanticName = DXBufferAttributeNames[decl[i].attribute];
			desc.SemanticIndex = si[decl[i].attribute];
			desc.Format = formatType.format;
			desc.InputSlot = 0;
			desc.AlignedByteOffset = index;
			desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc.InstanceDataStepRate = 0;
			index += formatType.bytes;
			si[decl[i].attribute] += 1;
		}
		InternalVertexDeclaration id;
		id.size = index;
		Shader* s = _ctx->shaders[shaderId];
		assert_result(_ctx->d3dDevice->CreateInputLayout(descriptors, num, s->vertexShaderBuffer, s->bufferSize, &id.layout), "Failed to create input layout");
		_ctx->layouts.push_back(id);
		return (RID)(_ctx->layouts.size() - 1);
	}

	// ------------------------------------------------------
	// set vertex declaration
	// ------------------------------------------------------
	void setVertexDeclaration(RID rid) {
		if (_ctx->selectedVertexDeclaration != rid) {
			XASSERT(rid != INVALID_RID, "Invalid input layout selected");
			if (rid == NO_RID) {
				_ctx->d3dContext->IASetInputLayout(NULL);
			}
			else {				
				XASSERT(rid < _ctx->layouts.size(), "Invalid input layout selected");
				ID3D11InputLayout* layout = _ctx->layouts[rid].layout;
				_ctx->d3dContext->IASetInputLayout(layout);
			}
			_ctx->selectedVertexDeclaration = rid;
		}
	}

	// ------------------------------------------------------
	// constant buffer
	// ------------------------------------------------------
	RID createConstantBuffer(int byteWidth) {
		D3D11_BUFFER_DESC constDesc;
		ZeroMemory(&constDesc, sizeof(constDesc));
		constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constDesc.ByteWidth = byteWidth;
		constDesc.Usage = D3D11_USAGE_DYNAMIC;
		constDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&constDesc, 0, &buffer), "Failed to create constant buffer");		
		_ctx->constantBuffers.push_back(buffer);
		return (RID)(_ctx->constantBuffers.size() - 1);
	}

	// ------------------------------------------------------
	// update constant buffer
	// ------------------------------------------------------
	void updateConstantBuffer(RID rid, void* data, size_t size) {
		XASSERT(rid != INVALID_RID, "Invalid constant buffer selected");
		XASSERT(rid < _ctx->constantBuffers.size(), "Invalid constant buffer selected");
		ID3D11Buffer* buffer = _ctx->constantBuffers[rid];
		D3D11_MAPPED_SUBRESOURCE resource;
		assert_result(_ctx->d3dContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource), "Failed to update constant buffer");
		void* ptr = resource.pData;
		// Copy the data into the vertex buffer.
		memcpy(ptr, data, size);
		_ctx->d3dContext->Unmap(buffer, 0);
	}

	// ------------------------------------------------------
	// set vertex shader constant buffer
	// ------------------------------------------------------
	void setVertexConstantBuffer(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid constant buffer selected");
		XASSERT(rid < _ctx->constantBuffers.size(), "Invalid constant buffer selected");
		ID3D11Buffer* buffer = _ctx->constantBuffers[rid];
		_ctx->d3dContext->VSSetConstantBuffers(0, 1, &buffer);
	}

	// ------------------------------------------------------
	// set pixel shader constant buffer
	// ------------------------------------------------------
	void setPixelConstantBuffer(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid constant buffer selected");
		XASSERT(rid < _ctx->constantBuffers.size(), "Invalid constant buffer selected");
		ID3D11Buffer* buffer = _ctx->constantBuffers[rid];
		_ctx->d3dContext->PSSetConstantBuffers(0, 1, &buffer);
	}

	// ------------------------------------------------------
	// set geometry shader constant buffer
	// ------------------------------------------------------
	void setGeometryConstantBuffer(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid constant buffer selected");
		XASSERT(rid < _ctx->constantBuffers.size(), "Invalid constant buffer selected");
		ID3D11Buffer* buffer = _ctx->constantBuffers[rid];
		_ctx->d3dContext->GSSetConstantBuffers(0, 1, &buffer);
	}

	const static int INDEX_BUFFER_SIZE[] = {
		sizeof(uint16_t),
		sizeof(uint32_t)
	};

	const static DXGI_FORMAT INDEX_BUFFER_FORMATS[] = {
		DXGI_FORMAT_R16_UINT,
		DXGI_FORMAT_R32_UINT
	};
	// ------------------------------------------------------
	// index buffer with data
	// ------------------------------------------------------
	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type, void* data) {
		InternalIndexBuffer ib;
		ib.numIndices = numIndices;
		ib.format = INDEX_BUFFER_FORMATS[indexType];
		ib.type = type;
		D3D11_BUFFER_DESC bufferDesc;
		if (type == BufferType::DYNAMIC) {
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.CPUAccessFlags = 0;
		}
		bufferDesc.ByteWidth = numIndices * INDEX_BUFFER_SIZE[indexType];
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesc, data ? &InitData : NULL, &ib.buffer), "Failed to create index buffer");
		_ctx->indexBuffers.push_back(ib);
		return (RID)(_ctx->indexBuffers.size() - 1);
	}

	// ------------------------------------------------------
	// index buffer
	// ------------------------------------------------------
	RID createIndexBuffer(uint32_t numIndices, IndexType indexType, BufferType type) {
		return createIndexBuffer(numIndices, indexType, type, 0);
	}

	// ------------------------------------------------------
	// set index buffer
	// ------------------------------------------------------
	void setIndexBuffer(RID rid) {
		if (_ctx->selectedIndexBuffer != rid) {
			XASSERT(rid != INVALID_RID, "Invalid index buffer selected");
			if (rid == NO_RID) {
				_ctx->d3dContext->IASetIndexBuffer(NULL, DXGI_FORMAT_UNKNOWN, 0);
			}
			else {
				XASSERT(rid < _ctx->indexBuffers.size(), "Invalid index buffer selected");
				_ctx->d3dContext->IASetIndexBuffer(_ctx->indexBuffers[rid].buffer, _ctx->indexBuffers[rid].format, 0);
			}
			_ctx->selectedIndexBuffer = rid;
		}
	}

	// ------------------------------------------------------
	// create a quad index buffer 0, 1, 2, 2, 3, 0
	// ------------------------------------------------------
	RID createQuadIndexBuffer(int numQuads) {
		int size = numQuads * 6;
		uint32_t* data = new uint32_t[size];
		int base = 0;
		int cnt = 0;
		for (int i = 0; i < numQuads; ++i) {
			data[base]     = cnt + 0;
			data[base + 1] = cnt + 1;
			data[base + 2] = cnt + 2;
			data[base + 3] = cnt + 2;
			data[base + 4] = cnt + 3;
			data[base + 5] = cnt + 0;
			base += 6;
			cnt += 4;
		}
		RID rid = createIndexBuffer(size, ds::IndexType::UINT_32, ds::BufferType::STATIC, data);
		delete[] data;
		return rid;
	}

	// ------------------------------------------------------
	// vertex buffer with data
	// ------------------------------------------------------
	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl) {
		const InternalVertexDeclaration& id = _ctx->layouts[vertexDecl];
		UINT size = numVertices * id.size;

		D3D11_BUFFER_DESC bufferDesciption;
		ZeroMemory(&bufferDesciption, sizeof(bufferDesciption));
		if (type == BufferType::DYNAMIC) {
			bufferDesciption.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesciption.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesciption.Usage = D3D11_USAGE_DEFAULT;
			bufferDesciption.CPUAccessFlags = 0;
		}
		bufferDesciption.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesciption.ByteWidth = size;

		ID3D11Buffer* buffer = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesciption, 0, &buffer),"Failed to create vertex buffer");
		_ctx->buffers.push_back(buffer);
		return (RID)(_ctx->buffers.size() - 1);
	}

	// ------------------------------------------------------
	// vertex buffer with data
	// ------------------------------------------------------
	RID createVertexBuffer(BufferType type, int numVertices, RID vertexDecl, void* data,int vertexSize) {

		struct VertexBuffer {
			BufferType type;
			RID vertexDeclaration;
			int vertexSize;
			int numVertices;
			ID3D11Buffer* buffer;
		};

		const InternalVertexDeclaration& id = _ctx->layouts[vertexDecl];
		UINT size = numVertices * id.size;

		D3D11_BUFFER_DESC bufferDesciption;
		ZeroMemory(&bufferDesciption, sizeof(bufferDesciption));
		if (type == BufferType::DYNAMIC) {
			bufferDesciption.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesciption.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			bufferDesciption.Usage = D3D11_USAGE_DEFAULT;
			bufferDesciption.CPUAccessFlags = 0;
		}
		bufferDesciption.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesciption.ByteWidth = size;

		ID3D11Buffer* buffer = 0;
		D3D11_SUBRESOURCE_DATA resource;
		resource.pSysMem = data;
		resource.SysMemPitch = 0;
		resource.SysMemSlicePitch = 0;
		assert_result(_ctx->d3dDevice->CreateBuffer(&bufferDesciption, &resource, &buffer),"Failed to create vertex buffer");
		_ctx->buffers.push_back(buffer);
		return (RID)(_ctx->buffers.size() - 1);
	}

	// ------------------------------------------------------
	// list of primitive topologies
	// ------------------------------------------------------
	static const D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGIES[] = {
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST
	};

	// ------------------------------------------------------
	// set vertex buffer
	// ------------------------------------------------------
	void setVertexBuffer(RID rid, uint32_t* stride, uint32_t* offset, PrimitiveTypes topology) {
		if (_ctx->selectedVertexBuffer != rid) {
			XASSERT(rid != INVALID_RID, "Invalid vertex buffer selected");
			if (rid == NO_RID) {
				_ctx->d3dContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
				_ctx->d3dContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGIES[topology]);
			}
			else {
				XASSERT(rid < _ctx->buffers.size(), "Invalid vertex buffer selected");
				ID3D11Buffer* buffer = _ctx->buffers[rid];
				_ctx->d3dContext->IASetVertexBuffers(0, 1, &buffer, stride, offset);
				_ctx->d3dContext->IASetPrimitiveTopology(PRIMITIVE_TOPOLOGIES[topology]);
			}
			_ctx->selectedVertexBuffer = rid;
		}
	}

	// ------------------------------------------------------
	// map data to vertex buffer
	// ------------------------------------------------------
	void mapBufferData(RID rid, void* data, uint32_t size) {
		XASSERT(rid != INVALID_RID, "Invalid buffer selected");
		XASSERT(rid < _ctx->buffers.size(), "Invalid buffer selected");
		ID3D11Buffer* buffer = _ctx->buffers[rid];
		D3D11_MAPPED_SUBRESOURCE resource;
		assert_result(_ctx->d3dContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource),"Failed to map data");
		void* ptr = resource.pData;
		memcpy(ptr, data, size);
		_ctx->d3dContext->Unmap(buffer, 0);		
	}

	// ------------------------------------------------------
	// Sampler State
	// ------------------------------------------------------
	static const D3D11_TEXTURE_ADDRESS_MODE TEXTURE_ADDRESSMODES[] = {
		{ D3D11_TEXTURE_ADDRESS_WRAP },
		{ D3D11_TEXTURE_ADDRESS_MIRROR },
		{ D3D11_TEXTURE_ADDRESS_CLAMP },
		{ D3D11_TEXTURE_ADDRESS_BORDER },
		{ D3D11_TEXTURE_ADDRESS_MIRROR_ONCE },
	};

	static const D3D11_FILTER TEXTURE_FILTERMODES[] = {
		{ D3D11_FILTER_MIN_MAG_MIP_POINT },
		{ D3D11_FILTER_MIN_MAG_MIP_LINEAR },
		{ D3D11_FILTER_ANISOTROPIC },
	};

	// ------------------------------------------------------
	// create sampler state
	// ------------------------------------------------------
	RID createSamplerState(TextureAddressModes addressMode,TextureFilters filter) {
		D3D11_SAMPLER_DESC colorMapDesc;
		ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));

		colorMapDesc.AddressU = TEXTURE_ADDRESSMODES[addressMode];
		colorMapDesc.AddressV = TEXTURE_ADDRESSMODES[addressMode];
		colorMapDesc.AddressW = TEXTURE_ADDRESSMODES[addressMode];

		colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		colorMapDesc.Filter = TEXTURE_FILTERMODES[filter];
		colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;
		ID3D11SamplerState* sampler;
		assert_result(_ctx->d3dDevice->CreateSamplerState(&colorMapDesc, &sampler), "Failed to create SamplerState");
		_ctx->samplerStates.push_back(sampler);
		return (RID)(_ctx->samplerStates.size() - 1);
	}

	// ------------------------------------------------------
	// set sampler state
	// ------------------------------------------------------
	void setSamplerState(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid sampler state selected");
		XASSERT(rid < _ctx->samplerStates.size(), "Invalid sampler state selected");
		ID3D11SamplerState* sampler = _ctx->samplerStates[rid];
		_ctx->d3dContext->PSSetSamplers(0, 1, &sampler);
	}

	// ------------------------------------------------------
	// Blend States
	// ------------------------------------------------------
	static const D3D11_BLEND BLEND_STATEMAPPINGS[] = {
		D3D11_BLEND_ZERO,
		D3D11_BLEND_ONE,
		D3D11_BLEND_SRC_COLOR,
		D3D11_BLEND_INV_SRC_COLOR,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_DEST_ALPHA,
		D3D11_BLEND_INV_DEST_ALPHA,
		D3D11_BLEND_DEST_COLOR,
		D3D11_BLEND_INV_DEST_COLOR,
		D3D11_BLEND_SRC_ALPHA_SAT,
		D3D11_BLEND_BLEND_FACTOR,
		D3D11_BLEND_INV_BLEND_FACTOR,
		D3D11_BLEND_SRC1_COLOR,
		D3D11_BLEND_INV_SRC1_COLOR,
		D3D11_BLEND_SRC1_ALPHA,
		D3D11_BLEND_INV_SRC1_ALPHA
	};

	// ------------------------------------------------------
	// create alpha enabled blend state
	// ------------------------------------------------------
	RID createAlphaBlendState(BlendStates srcBlend, BlendStates destBlend) {
		return createBlendState(srcBlend, srcBlend, destBlend, destBlend, true);
	}

	// ------------------------------------------------------
	// cretae blend state
	// ------------------------------------------------------
	RID createBlendState(BlendStates srcBlend, BlendStates srcAlphaBlend, BlendStates destBlend, BlendStates destAlphaBlend, bool alphaEnabled) {
		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(blendDesc));
		if (alphaEnabled) {
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
		}
		else {
			blendDesc.RenderTarget[0].BlendEnable = FALSE;
			//blendDesc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
		}
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlend = BLEND_STATEMAPPINGS[srcBlend];
		blendDesc.RenderTarget[0].DestBlend = BLEND_STATEMAPPINGS[destBlend];
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = BLEND_STATEMAPPINGS[srcAlphaBlend];
		blendDesc.RenderTarget[0].DestBlendAlpha = BLEND_STATEMAPPINGS[destAlphaBlend];
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

		ID3D11BlendState* state;
		assert_result(_ctx->d3dDevice->CreateBlendState(&blendDesc, &state), "Failed to create blendstate");
		_ctx->blendStates.push_back(state);
		return (RID)(_ctx->blendStates.size() - 1);
	}

	// ------------------------------------------------------
	// set blend state
	// ------------------------------------------------------
	void setBlendState(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid blendstate selected");
		XASSERT(rid < _ctx->blendStates.size(), "Invalid blendstate selected");
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		_ctx->d3dContext->OMSetBlendState(_ctx->blendStates[rid], blendFactor, 0xFFFFFFFF);
	}

	// ------------------------------------------------------
	// set blend state with factor and mask
	// ------------------------------------------------------
	void setBlendState(RID rid, float* blendFactor,uint32_t mask) {
		XASSERT(rid != INVALID_RID, "Invalid blendstate selected");
		XASSERT(rid < _ctx->blendStates.size(), "Invalid blendstate selected");
		_ctx->d3dContext->OMSetBlendState(_ctx->blendStates[rid], blendFactor, mask);
	}

	// ------------------------------------------------------
	// Draw indexed
	// ------------------------------------------------------
	void drawIndexed(uint32_t num) {
		_ctx->d3dContext->DrawIndexed(num, 0, 0);
	}

	// ------------------------------------------------------
	// draw
	// ------------------------------------------------------
	void draw(uint32_t num) {
		_ctx->d3dContext->Draw(num, 0);
	}

	// ------------------------------------------------------
	// create shader
	// ------------------------------------------------------
	RID createShader() {
		Shader* s = new Shader;
		_ctx->shaders.push_back(s);
		return (RID)(_ctx->shaders.size() - 1);
	}

	struct DataFile {
		char* data;
		int size;
	};

	static DataFile read_data(const char* name) {
		DataFile df;
		df.data = 0;
		df.size = -1;
		FILE *fp = fopen(name, "rb");
		XASSERT(fp != 0, "Cannot find file: '%s'", name);
		if (fp) {			
			fseek(fp, 0, SEEK_END);
			int sz = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			df.data = new char[sz + 1];
			fread(df.data, 1, sz, fp);
			df.data[sz] = '\0';
			fclose(fp);
			df.size = sz;
		}
		return df;
	}

	// ------------------------------------------------------
	// load vertex shader
	// ------------------------------------------------------
	void loadVertexShader(RID shader, const char* csoName) {
		XASSERT(shader != INVALID_RID, "Invalid shader selected");
		XASSERT(shader < _ctx->shaders.size(), "Invalid shader selected");
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load vertex shader file: '%s'", csoName);
		Shader* s = _ctx->shaders[shader];
		assert_result(_ctx->d3dDevice->CreateVertexShader(
			file.data,
			file.size,
			nullptr,
			&s->vertexShader),"Failed to create vertex shader");
		s->vertexShaderBuffer = new char[file.size];
		memcpy(s->vertexShaderBuffer, file.data, file.size);
		s->bufferSize = file.size;
		delete[] file.data;		
	}

	// ------------------------------------------------------
	// load pixel shader
	// ------------------------------------------------------
	void loadPixelShader(RID shader, const char* csoName) {
		XASSERT(shader != INVALID_RID, "Invalid shader selected");
		XASSERT(shader < _ctx->shaders.size(), "Invalid shader selected");
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load pixel shader file: '%s'", csoName);
		Shader* s = _ctx->shaders[shader];
		assert_result(_ctx->d3dDevice->CreatePixelShader(
			file.data,
			file.size,
			nullptr,
			&s->pixelShader),"Failed to create pixel shader");		
		delete[] file.data;
	}

	// ------------------------------------------------------
	// load pixel shader
	// ------------------------------------------------------
	void loadGeometryShader(RID shader, const char* csoName) {
		XASSERT(shader != INVALID_RID, "Invalid shader selected");
		XASSERT(shader < _ctx->shaders.size(), "Invalid shader selected");
		DataFile file = read_data(csoName);
		XASSERT(file.size != -1, "Cannot load geometry shader file: '%s'", csoName);
		Shader* s = _ctx->shaders[shader];
		assert_result(_ctx->d3dDevice->CreateGeometryShader(
			file.data,
			file.size,
			nullptr,
			&s->geometryShader
			),"Failed to create geometry shader");
		delete[] file.data;
	}

	// ------------------------------------------------------
	// set shader
	// ------------------------------------------------------
	void setShader(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid shader selected");
		XASSERT(rid < _ctx->shaders.size(), "Invalid shader selected");
		if (rid != _ctx->selectedShaderId) {
			ds::Shader* s = _ctx->shaders[rid];
			if (s->vertexShader != 0) {
				_ctx->d3dContext->VSSetShader(s->vertexShader, 0, 0);
			}
			else {
				_ctx->d3dContext->VSSetShader(NULL, NULL, 0);
			}
			if (s->pixelShader != 0) {
				_ctx->d3dContext->PSSetShader(s->pixelShader, 0, 0);
			}
			else {
				_ctx->d3dContext->PSSetShader(NULL, NULL, 0);
			}
			if (s->geometryShader != 0) {
				_ctx->d3dContext->GSSetShader(s->geometryShader, 0, 0);
			}
			else {
				_ctx->d3dContext->GSSetShader(NULL, NULL, 0);
			}
			_ctx->selectedShaderId = rid;
		}
	}

	const static DXGI_FORMAT TEXTURE_FOMATS[] = {
		DXGI_FORMAT_R8G8B8A8_UINT,
		DXGI_FORMAT_R8G8B8A8_UNORM
	};
	// ------------------------------------------------------
	// create texture
	// ------------------------------------------------------
	RID createTexture(int width, int height, uint8_t channels, void* data, TextureFormat format) {
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = TEXTURE_FOMATS[format];
		desc.SampleDesc.Count = 1;// _ctx->multisampling;
		desc.SampleDesc.Quality = 0;		
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		ID3D11Texture2D *texture2D = 0;
		/*
		if (_ctx->multisampling > 1) {
			desc.Usage = D3D11_USAGE_DEFAULT;
			ASSERT_RESULT(_ctx->d3dDevice->CreateTexture2D(&desc, 0, &texture2D), "Failed to create Texture2D");
			D3D11_MAPPED_SUBRESOURCE resource;
			ASSERT_RESULT(_ctx->d3dContext->Map(texture2D, 0, D3D11_MAP_WRITE, 0, &resource), "Failed to map data");
			void* ptr = resource.pData;
			memcpy(ptr, data, width * height * channels);
			_ctx->d3dContext->Unmap(texture2D, 0);
		}
		else {
		*/
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			D3D11_SUBRESOURCE_DATA subres;
			subres.pSysMem = data;
			subres.SysMemPitch = width * channels;
			subres.SysMemSlicePitch = 0;			
			ASSERT_RESULT(_ctx->d3dDevice->CreateTexture2D(&desc, &subres, &texture2D), "Failed to create Texture2D");
		//}
		ID3D11ShaderResourceView* srv = 0;
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		//if (_ctx->multisampling > 1) {
			//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		//}
		//else {
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//}
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Format = TEXTURE_FOMATS[format];

		ASSERT_RESULT(_ctx->d3dDevice->CreateShaderResourceView(texture2D, &srvDesc, &srv), "Failed to create resource view");

		InternalTexture tex;
		tex.width = width;
		tex.height = height;
		tex.srv = srv;

		_ctx->shaderResourceViews.push_back(srv);
		return (RID)(_ctx->shaderResourceViews.size() - 1);
	}

	// ------------------------------------------------------
	// set texture
	// ------------------------------------------------------
	void setTexture(RID rid, ShaderType type) {
		setTexture(rid, type, 0);
	}

	// ------------------------------------------------------
	// set texture
	// ------------------------------------------------------
	void setTexture(RID rid, ShaderType type, uint8_t slot) {
		XASSERT(rid != INVALID_RID, "Invalid texture selected");		
		XASSERT(_ctx->selectedShaderId != INVALID_RID, "Invalid or no shader selected");
		Shader* s = _ctx->shaders[_ctx->selectedShaderId];
		ID3D11ShaderResourceView* srv = 0;
		if (rid != NO_RID) {
			XASSERT(rid < _ctx->shaderResourceViews.size(), "Invalid texture selected");
			srv = _ctx->shaderResourceViews[rid];
		}
		if (type == ShaderType::PIXEL) {
			XASSERT(s->pixelShader != 0, "No pixel shader selected");
			if (_ctx->selectedPSTextures[slot] != rid) {
				_ctx->d3dContext->PSSetShaderResources(slot, 1, &srv);
				_ctx->selectedPSTextures[slot] = rid;
			}
		}
		else if (type == ShaderType::VERTEX) {
			XASSERT(s->vertexShader != 0, "No vertex shader selected");
			if (_ctx->selectedVSTextures[slot] != rid) {
				_ctx->d3dContext->VSSetShaderResources(slot, 1, &srv);
				_ctx->selectedVSTextures[slot] = rid;
			}
		}
		else if (type == ShaderType::GEOMETRY) {
			XASSERT(s->geometryShader != 0, "No geometry shader selected");
			if (_ctx->selectedGSTextures[slot] != rid) {
				_ctx->d3dContext->GSSetShaderResources(slot, 1, &srv);
				_ctx->selectedGSTextures[slot] = rid;
			}
		}
	}

	// ------------------------------------------------------
	// set depth buffer state
	// ------------------------------------------------------
	void setDepthBufferState(DepthBufferState state) {
		if (_ctx->depthBufferState != state) {
			if (state == DepthBufferState::ENABLED) {
				_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthEnabledStencilState, 1);
			}
			else {
				_ctx->d3dContext->OMSetDepthStencilState(_ctx->depthDisabledStencilState, 1);
			}
		}
		_ctx->depthBufferState = state;
	}

	// ------------------------------------------------------
	// create rasterizer state
	// ------------------------------------------------------
	RID createRasterizerState(CullMode cullMode,FillMode fillMode, bool multiSample, bool scissor, float depthBias, float slopeDepthBias) {
		D3D11_RASTERIZER_DESC desc;
		desc.CullMode = (D3D11_CULL_MODE)cullMode;
		desc.FillMode = (D3D11_FILL_MODE)fillMode;
		desc.FrontCounterClockwise = FALSE;
		desc.DepthBias = (INT)depthBias;
		desc.DepthBiasClamp = 0.0f;
		desc.SlopeScaledDepthBias = slopeDepthBias;
		desc.AntialiasedLineEnable = FALSE;
		desc.DepthClipEnable = TRUE;
		desc.MultisampleEnable = (BOOL)multiSample;
		desc.ScissorEnable = (BOOL)scissor;
		ID3D11RasterizerState* state = 0;
		assert_result(_ctx->d3dDevice->CreateRasterizerState(&desc, &state), "Failed to create rasterizer state");
		_ctx->rasterizerStates.push_back(state);
		return (RID)(_ctx->rasterizerStates.size() - 1);
	}

	// ------------------------------------------------------
	// set rasterizer state
	// ------------------------------------------------------
	void setRasterizerState(RID rid) {
		XASSERT(rid != INVALID_RID, "Invalid rasterizer state selected");
		if (rid == NO_RID) {

		}
		else {
			XASSERT(rid < _ctx->rasterizerStates.size(), "Invalid rasterizer state selected");
			ID3D11RasterizerState* state = _ctx->rasterizerStates[rid];
			_ctx->d3dContext->RSSetState(state);
		}
	}

	// ------------------------------------------------------
	// create render target
	// ------------------------------------------------------
	RID createRenderTarget(uint16_t width, uint16_t height) {
		RenderTarget rt;
		
		// Initialize the render target texture description.
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// Setup the render target texture description.
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		assert_result(_ctx->d3dDevice->CreateTexture2D(&textureDesc, NULL, &rt.texture), "Failed to create texture for rendertarget");
		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		// Create the render target view.
		assert_result(_ctx->d3dDevice->CreateRenderTargetView(rt.texture, &renderTargetViewDesc, &rt.view),"Failed to create render target view");
		// Setup the description of the shader resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		assert_result(_ctx->d3dDevice->CreateShaderResourceView(rt.texture, &shaderResourceViewDesc, &rt.srv),"Failed to create shader resource view");

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		assert_result(_ctx->d3dDevice->CreateTexture2D(&depthTexDesc, 0, &rt.depthTexture),"Failed to create depth texture");
		
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		descDSV.Texture2D.MipSlice = 0;

		assert_result(_ctx->d3dDevice->CreateDepthStencilView(rt.depthTexture, &descDSV, &rt.depthStencilView),"Failed to create depth stencil view");

		return INVALID_RID;
	}

}
#endif
