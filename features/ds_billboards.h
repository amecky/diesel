#pragma once
#include "..\diesel.h"

//#define DS_BILLBOARDS

namespace billboards {

	void init(RID textureID, int maxItems);

	void render(RID renderPass, const ds::matrix& viewProjectionMatrix);

	void begin();

	void add(const ds::vec3& pos, const ds::vec2& dim, const ds::vec4& rect, float rotation = 0.0f, const ds::vec2& scaling = ds::vec2(1.0f), const ds::Color& clr = ds::Color(255, 255, 255, 255));

	void end();

	void shutdown();

}

#ifdef DS_BILLBOARDS

namespace billboards {

	struct BillboardsConstantBuffer {
		ds::vec4 screenDimension;
		ds::vec4 screenCenter;
		ds::matrix wvp;
	};

	struct Billboard {
		ds::vec3 position;
		ds::vec2 dimension;
		float rotation;
		ds::vec2 scaling;
		ds::vec4 texture;
		ds::Color color;
	};

	struct BillboardsContext {
		Billboard* items;
		int num;
		int capacity;
		RID drawItem;
		ds::DrawCommand drawCmd;
		RID structuredBufferId;
		BillboardsConstantBuffer constantBuffer;
	};

	static BillboardsContext* _ctx = 0;

	const BYTE Billboards_VS_Main[] =
	{
		 68,  88,  66,  67, 164,  11,
		107,  20, 205, 238, 159, 178,
		 96,  77, 232, 153, 177, 104,
		194, 245,   1,   0,   0,   0,
		 20,   5,   0,   0,   4,   0,
		  0,   0,  48,   0,   0,   0,
		100,   0,   0,   0, 216,   0,
		  0,   0,   4,   5,   0,   0,
		 73,  83,  71,  78,  44,   0,
		  0,   0,   1,   0,   0,   0,
		  8,   0,   0,   0,  32,   0,
		  0,   0,   0,   0,   0,   0,
		  6,   0,   0,   0,   1,   0,
		  0,   0,   0,   0,   0,   0,
		  1,   1,   0,   0,  83,  86,
		 95,  86,  69,  82,  84,  69,
		 88,  73,  68,   0,  79,  83,
		 71,  78, 108,   0,   0,   0,
		  3,   0,   0,   0,   8,   0,
		  0,   0,  80,   0,   0,   0,
		  0,   0,   0,   0,   1,   0,
		  0,   0,   3,   0,   0,   0,
		  0,   0,   0,   0,  15,   0,
		  0,   0,  92,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  0,   0,   3,   0,   0,   0,
		  1,   0,   0,   0,   3,  12,
		  0,   0, 101,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  0,   0,   3,   0,   0,   0,
		  2,   0,   0,   0,  15,   0,
		  0,   0,  83,  86,  95,  80,
		 79,  83,  73,  84,  73,  79,
		 78,   0,  84,  69,  88,  67,
		 79,  79,  82,  68,   0,  67,
		 79,  76,  79,  82,   0, 171,
		 83,  72,  69,  88,  36,   4,
		  0,   0,  64,   0,   1,   0,
		  9,   1,   0,   0, 106,  72,
		  0,   1,  89,   0,   0,   4,
		 70, 142,  32,   0,   0,   0,
		  0,   0,   6,   0,   0,   0,
		162,   0,   0,   4,   0, 112,
		 16,   0,   1,   0,   0,   0,
		 64,   0,   0,   0,  96,   0,
		  0,   4,  18,  16,  16,   0,
		  0,   0,   0,   0,   6,   0,
		  0,   0, 103,   0,   0,   4,
		242,  32,  16,   0,   0,   0,
		  0,   0,   1,   0,   0,   0,
		101,   0,   0,   3,  50,  32,
		 16,   0,   1,   0,   0,   0,
		101,   0,   0,   3, 242,  32,
		 16,   0,   2,   0,   0,   0,
		104,   0,   0,   2,   3,   0,
		  0,   0, 105,   0,   0,   4,
		  0,   0,   0,   0,   4,   0,
		  0,   0,   4,   0,   0,   0,
		 85,   0,   0,   7,  18,   0,
		 16,   0,   0,   0,   0,   0,
		 10,  16,  16,   0,   0,   0,
		  0,   0,   1,  64,   0,   0,
		  2,   0,   0,   0, 167,   0,
		  0,   9, 242,   0,  16,   0,
		  1,   0,   0,   0,  10,   0,
		 16,   0,   0,   0,   0,   0,
		  1,  64,   0,   0,  32,   0,
		  0,   0,  70, 126,  16,   0,
		  1,   0,   0,   0,  14,   0,
		  0,   8, 242,   0,  16,   0,
		  1,   0,   0,   0,  70,  14,
		 16,   0,   1,   0,   0,   0,
		230, 142,  32,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		 54,   0,   0,   6,  50,  48,
		 32,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,  70,   0,
		 16,   0,   1,   0,   0,   0,
		 54,   0,   0,   5, 194,   0,
		 16,   0,   2,   0,   0,   0,
		 86,   1,  16,   0,   1,   0,
		  0,   0,   0,   0,   0,   7,
		 50,   0,  16,   0,   2,   0,
		  0,   0, 230,  10,  16,   0,
		  1,   0,   0,   0,  70,   0,
		 16,   0,   1,   0,   0,   0,
		 54,   0,   0,   6,  50,  48,
		 32,   0,   0,   0,   0,   0,
		  1,   0,   0,   0, 134,   0,
		 16,   0,   2,   0,   0,   0,
		 54,   0,   0,   6,  50,  48,
		 32,   0,   0,   0,   0,   0,
		  2,   0,   0,   0, 118,  15,
		 16,   0,   2,   0,   0,   0,
		 54,   0,   0,   6,  50,  48,
		 32,   0,   0,   0,   0,   0,
		  3,   0,   0,   0,  70,   0,
		 16,   0,   2,   0,   0,   0,
		167,   0,   0,   9,  18,   0,
		 16,   0,   1,   0,   0,   0,
		 10,   0,  16,   0,   0,   0,
		  0,   0,   1,  64,   0,   0,
		 28,   0,   0,   0,   6, 112,
		 16,   0,   1,   0,   0,   0,
		  1,   0,   0,  10, 226,   0,
		 16,   0,   0,   0,   0,   0,
		  6,  16,  16,   0,   0,   0,
		  0,   0,   2,  64,   0,   0,
		  0,   0,   0,   0,   3,   0,
		  0,   0,   1,   0,   0,   0,
		  2,   0,   0,   0,  55,   0,
		  0,  15, 194,   0,  16,   0,
		  0,   0,   0,   0, 166,  14,
		 16,   0,   0,   0,   0,   0,
		  2,  64,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  0,   0,   0,  63,   0,   0,
		  0, 191,   2,  64,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0, 191,
		  0,   0,   0,  63,  54,   0,
		  0,   7,  50,  32,  16,   0,
		  1,   0,   0,   0,  70,  48,
		 32,   4,   0,   0,   0,   0,
		 26,   0,  16,   0,   0,   0,
		  0,   0, 167,   0,   0,   9,
		242,   0,  16,   0,   2,   0,
		  0,   0,  10,   0,  16,   0,
		  0,   0,   0,   0,   1,  64,
		  0,   0,  12,   0,   0,   0,
		 70, 126,  16,   0,   1,   0,
		  0,   0,  56,   0,   0,   7,
		 98,   0,  16,   0,   0,   0,
		  0,   0, 166,  11,  16,   0,
		  0,   0,   0,   0,   6,   1,
		 16,   0,   2,   0,   0,   0,
		 56,   0,   0,   7,  66,   0,
		 16,   0,   0,   0,   0,   0,
		 10,   0,  16,   0,   1,   0,
		  0,   0,  42,   0,  16,   0,
		  0,   0,   0,   0,  56,   0,
		  0,   7,  34,   0,  16,   0,
		  0,   0,   0,   0,  58,   0,
		 16,   0,   2,   0,   0,   0,
		 26,   0,  16,   0,   0,   0,
		  0,   0,  77,   0,   0,   7,
		 18,   0,  16,   0,   1,   0,
		  0,   0,  18,   0,  16,   0,
		  2,   0,   0,   0,  42,   0,
		 16,   0,   2,   0,   0,   0,
		 56,   0,   0,   7, 130,   0,
		 16,   0,   0,   0,   0,   0,
		 42,   0,  16,   0,   0,   0,
		  0,   0,  10,   0,  16,   0,
		  1,   0,   0,   0,  56,   0,
		  0,   7,  66,   0,  16,   0,
		  0,   0,   0,   0,  42,   0,
		 16,   0,   0,   0,   0,   0,
		 10,   0,  16,   0,   2,   0,
		  0,   0,  50,   0,   0,   9,
		 66,   0,  16,   0,   0,   0,
		  0,   0,  10,   0,  16,   0,
		  1,   0,   0,   0,  26,   0,
		 16,   0,   0,   0,   0,   0,
		 42,   0,  16,   0,   0,   0,
		  0,   0,  50,   0,   0,  10,
		 34,   0,  16,   0,   0,   0,
		  0,   0,  10,   0,  16,   0,
		  2,   0,   0,   0,  26,   0,
		 16,   0,   0,   0,   0,   0,
		 58,   0,  16, 128,  65,   0,
		  0,   0,   0,   0,   0,   0,
		167,   0,   0,   9, 114,   0,
		 16,   0,   1,   0,   0,   0,
		 10,   0,  16,   0,   0,   0,
		  0,   0,   1,  64,   0,   0,
		  0,   0,   0,   0,  70, 114,
		 16,   0,   1,   0,   0,   0,
		167,   0,   0,   9, 242,  32,
		 16,   0,   2,   0,   0,   0,
		 10,   0,  16,   0,   0,   0,
		  0,   0,   1,  64,   0,   0,
		 48,   0,   0,   0,  70, 126,
		 16,   0,   1,   0,   0,   0,
		  0,   0,   0,   7,  50,   0,
		 16,   0,   1,   0,   0,   0,
		150,   5,  16,   0,   0,   0,
		  0,   0,  70,   0,  16,   0,
		  1,   0,   0,   0,  54,   0,
		  0,   5, 130,   0,  16,   0,
		  1,   0,   0,   0,   1,  64,
		  0,   0,   0,   0, 128,  63,
		 17,   0,   0,   8,  18,  32,
		 16,   0,   0,   0,   0,   0,
		 70,  14,  16,   0,   1,   0,
		  0,   0,  70, 142,  32,   0,
		  0,   0,   0,   0,   2,   0,
		  0,   0,  17,   0,   0,   8,
		 34,  32,  16,   0,   0,   0,
		  0,   0,  70,  14,  16,   0,
		  1,   0,   0,   0,  70, 142,
		 32,   0,   0,   0,   0,   0,
		  3,   0,   0,   0,  17,   0,
		  0,   8,  66,  32,  16,   0,
		  0,   0,   0,   0,  70,  14,
		 16,   0,   1,   0,   0,   0,
		 70, 142,  32,   0,   0,   0,
		  0,   0,   4,   0,   0,   0,
		 17,   0,   0,   8, 130,  32,
		 16,   0,   0,   0,   0,   0,
		 70,  14,  16,   0,   1,   0,
		  0,   0,  70, 142,  32,   0,
		  0,   0,   0,   0,   5,   0,
		  0,   0,  62,   0,   0,   1,
		 83,  70,  73,  48,   8,   0,
		  0,   0,   2,   0,   0,   0,
		  0,   0,   0,   0
	};

	const BYTE Billboards_PS_Main[] =
	{
		 68,  88,  66,  67, 133, 123,
		244, 109,  36, 101, 150, 228,
		 91, 135, 209, 221,  54, 143,
		 33,  28,   1,   0,   0,   0,
		112,   1,   0,   0,   3,   0,
		  0,   0,  44,   0,   0,   0,
		160,   0,   0,   0, 212,   0,
		  0,   0,  73,  83,  71,  78,
		108,   0,   0,   0,   3,   0,
		  0,   0,   8,   0,   0,   0,
		 80,   0,   0,   0,   0,   0,
		  0,   0,   1,   0,   0,   0,
		  3,   0,   0,   0,   0,   0,
		  0,   0,  15,   0,   0,   0,
		 92,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  3,   0,   0,   0,   1,   0,
		  0,   0,   3,   3,   0,   0,
		101,   0,   0,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  3,   0,   0,   0,   2,   0,
		  0,   0,  15,  15,   0,   0,
		 83,  86,  95,  80,  79,  83,
		 73,  84,  73,  79,  78,   0,
		 84,  69,  88,  67,  79,  79,
		 82,  68,   0,  67,  79,  76,
		 79,  82,   0, 171,  79,  83,
		 71,  78,  44,   0,   0,   0,
		  1,   0,   0,   0,   8,   0,
		  0,   0,  32,   0,   0,   0,
		  0,   0,   0,   0,   0,   0,
		  0,   0,   3,   0,   0,   0,
		  0,   0,   0,   0,  15,   0,
		  0,   0,  83,  86,  95,  84,
		 65,  82,  71,  69,  84,   0,
		171, 171,  83,  72,  68,  82,
		148,   0,   0,   0,  64,   0,
		  0,   0,  37,   0,   0,   0,
		 90,   0,   0,   3,   0,  96,
		 16,   0,   0,   0,   0,   0,
		 88,  24,   0,   4,   0, 112,
		 16,   0,   0,   0,   0,   0,
		 85,  85,   0,   0,  98,  16,
		  0,   3,  50,  16,  16,   0,
		  1,   0,   0,   0,  98,  16,
		  0,   3, 242,  16,  16,   0,
		  2,   0,   0,   0, 101,   0,
		  0,   3, 242,  32,  16,   0,
		  0,   0,   0,   0, 104,   0,
		  0,   2,   1,   0,   0,   0,
		 69,   0,   0,   9, 242,   0,
		 16,   0,   0,   0,   0,   0,
		 70,  16,  16,   0,   1,   0,
		  0,   0,  70, 126,  16,   0,
		  0,   0,   0,   0,   0,  96,
		 16,   0,   0,   0,   0,   0,
		 56,   0,   0,   7, 242,  32,
		 16,   0,   0,   0,   0,   0,
		 70,  14,  16,   0,   0,   0,
		  0,   0,  70,  30,  16,   0,
		  2,   0,   0,   0,  62,   0,
		  0,   1
	};


	void init(RID textureID, int maxItems) {
		_ctx = new BillboardsContext;
		_ctx->items = new Billboard[maxItems];
		_ctx->num = 0;
		_ctx->capacity = maxItems;

		RID vertexShader = ds::createShader(ds::ShaderDesc()
			.Data(Billboards_VS_Main)
			.DataSize(sizeof(Billboards_VS_Main))
			.ShaderType(ds::ShaderType::ST_VERTEX_SHADER)
		);

		RID pixelShader = ds::createShader(ds::ShaderDesc()
			.Data(Billboards_PS_Main)
			.DataSize(sizeof(Billboards_PS_Main))
			.ShaderType(ds::ShaderType::ST_PIXEL_SHADER)
		);

		RID cbid = ds::createConstantBuffer(sizeof(BillboardsConstantBuffer), &_ctx->constantBuffer);
		int indices[] = { 0,1,2,1,3,2 };
		RID idxBuffer = ds::createQuadIndexBuffer(maxItems, indices);

		RID ssid = ds::createSamplerState(ds::SamplerStateDesc()
			.AddressMode(ds::TextureAddressModes::CLAMP)
			.Filter(ds::TextureFilters::LINEAR)
		);

		_ctx->structuredBufferId = ds::createStructuredBuffer(ds::StructuredBufferDesc()
			.CpuWritable(true)
			.ElementSize(sizeof(Billboard))
			.NumElements(maxItems)
			.GpuWritable(false)
		);

		ds::vec2 size = ds::getTextureSize(textureID);
		_ctx->constantBuffer.screenDimension = ds::vec4(static_cast<float>(ds::getScreenWidth()), static_cast<float>(ds::getScreenHeight()), size.x, size.y);
		_ctx->constantBuffer.screenCenter = ds::vec4(static_cast<float>(ds::getScreenWidth()) * 0.5f, static_cast<float>(ds::getScreenHeight()) * 0.5f, 0.0f, 0.0f);

		RID stateGroup = ds::StateGroupBuilder()
			.constantBuffer(cbid, vertexShader)
			.structuredBuffer(_ctx->structuredBufferId, vertexShader, 1)
			.vertexBuffer(NO_RID)
			.vertexShader(vertexShader)
			.indexBuffer(idxBuffer)
			.pixelShader(pixelShader)
			.samplerState(ssid, pixelShader)
			.texture(textureID, pixelShader, 0)
			.build();

		_ctx->drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

		_ctx->drawItem = ds::compile(_ctx->drawCmd, stateGroup);

	}

	void render(RID renderPass, const ds::matrix& viewProjectionMatrix) {
		_ctx->constantBuffer.wvp = ds::matTranspose(viewProjectionMatrix);
		ds::mapBufferData(_ctx->structuredBufferId, _ctx->items, _ctx->num * sizeof(Billboard));
		_ctx->drawCmd.size = _ctx->num;
		ds::submit(renderPass, _ctx->drawItem, _ctx->num * 6);
	}

	void begin() {
		_ctx->num = 0;
	}

	void add(const ds::vec3& pos, const ds::vec2& dim, const ds::vec4& rect, float rotation, const ds::vec2& scaling, const ds::Color& clr) {
		if (_ctx->num < _ctx->capacity) {
			Billboard& b = _ctx->items[_ctx->num++];
			b.position = pos;
			b.dimension = dim;
			b.texture = rect;
			b.rotation = rotation;
			b.scaling = scaling;
			b.color = clr;
		}
	}

	void end() {
	}

	void shutdown() {
		delete[] _ctx->items;
		delete _ctx;
	}
}

#endif