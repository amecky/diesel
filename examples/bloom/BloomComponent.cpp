#include "BloomComponent.h"
#include "..\common\Fullscreen_Quad_VS_Main.h"
#include "..\common\Fullscreen_Quad_PS_Main.h"
#include "Bloom_PS_Main.h"
#include "Blurr_PS_Main.h"
#include "BloomCombine_PS_Main.h"
// Name        Thresh  Bloom  Base  BloomSat BaseSat
// Default     0.25f   1.25f   1      1        1
// Soft        0       1       1      1        1
// Desaturated 0.5     2       1      0        1
// Saturated   0.25    2       1      2        0
// Blurry      0       1       0.1    1        1
// Subtle      0.5     1       1      1        1

const static float KERNELS[16] = { -6.0f, -5.0f, -4.0f,	-3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 0.0f, 0.0f, 0.0f };
const static float WEIGHTS[16] = { 0.002216f, 0.008764f, 0.026995f, 0.064759f, 0.120985f, 0.176033f, 0.199471f, 0.176033f, 0.120985f, 0.064759f, 0.026995f, 0.008764f, 0.002216f, 0.0f, 0.0f, 0.0f };

BloomComponent::BloomComponent(ds::Camera* camera,RID renderTarget, RID finalRenderPass, BloomSettings* bloomSettings, BloomExtractSettings* extractSettings)
	: _camera(camera), _renderTarget(renderTarget) , _finalRenderPass(finalRenderPass), _bloomBuffer(bloomSettings) , _bloomExtractBuffer(extractSettings) {
	initialize();
}


BloomComponent::~BloomComponent() {
}

void BloomComponent::initialize() {
	//
	// create three render targets to switch between
	//
	ds::RenderTargetInfo rtInfo = { 1024, 768, ds::Color(0, 0, 0, 0) };
	RID bloomRT1 = ds::createRenderTarget(rtInfo);
	RID bloomRT2 = ds::createRenderTarget(rtInfo);
	//
	// basic pass 
	//
	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 0.0f, -1.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	// render pass using back buffer
	//_backbufferPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::DISABLED);
	ds::ViewportInfo vpInfo = { 1024,768,0.0f,100.0f };
	RID vp = ds::createViewport(vpInfo);
	// render pass using RT1
	RID rt1s[] = { bloomRT1 };
	ds::RenderPassInfo bloomRT1Info = { _camera, vp, ds::DepthBufferState::DISABLED, rt1s, 1 };
	_bloomRT1Pass = ds::createRenderPass(bloomRT1Info);

	// render pass using RT2
	RID rt2s[] = { bloomRT2 };
	ds::RenderPassInfo bloomRT2Info = { _camera, vp, ds::DepthBufferState::DISABLED, rt2s, 1 };
	_bloomRT2Pass = ds::createRenderPass(bloomRT2Info);

	ds::ShaderInfo fsvsInfo = { 0, Fullscreen_Quad_VS_Main, sizeof(Fullscreen_Quad_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID fsVertexShader = ds::createShader(fsvsInfo);
	ds::ShaderInfo psvsInfo = { 0, Fullscreen_Quad_PS_Main, sizeof(Fullscreen_Quad_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID fsPixelShader = ds::createShader(psvsInfo);
	ds::ShaderInfo blurPSInfo = { 0, Blurr_PS_Main, sizeof(Blurr_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID blurPSShader = ds::createShader(blurPSInfo);
	ds::ShaderInfo bloomPSInfo = { 0, BloomCombine_PS_Main, sizeof(BloomCombine_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID bloomPSShader = ds::createShader(bloomPSInfo);
	ds::ShaderInfo bloomExtractPSInfo = { 0, Bloom_PS_Main, sizeof(Bloom_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID bloomExtractPS = ds::createShader(bloomExtractPSInfo);
	//
	// the blur buffer 
	//
	//BlurBuffer blurBuffer;
	for (int i = 0; i < 13; ++i) {
		_blurBuffer.kernels[i] = KERNELS[i];
		_blurBuffer.weights[i] = WEIGHTS[i];
	}
	_blurBuffer.direction = ds::vec2(1, 0);
	_blurBuffer.data = ds::vec2(1024.0f, 768.0f);
	RID blurBufferID = ds::createConstantBuffer(sizeof(BlurBuffer), &_blurBuffer);
	RID bloomExtractBufferID = ds::createConstantBuffer(sizeof(BloomExtractSettings), _bloomExtractBuffer);
	RID bloomBufferID = ds::createConstantBuffer(sizeof(BloomSettings), _bloomBuffer);
	//
	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);
	//
	// the full screen draw command
	//
	ds::DrawCommand fsCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	// 
	// Basic full screen state group
	//
	RID basicFSGroup = ds::StateGroupBuilder()
		.inputLayout(NO_RID)
		.indexBuffer(NO_RID)
		.vertexBuffer(NO_RID)
		.geometryShader(NO_RID)
		.build();
	// 
	// the state group for bloom extract
	//
	RID extractGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(_renderTarget, bloomExtractPS, 0)
		.vertexShader(fsVertexShader)
		.pixelShader(bloomExtractPS)
		.samplerState(ssid, bloomExtractPS)
		.constantBuffer(bloomExtractBufferID, bloomExtractPS, 0)
		.build();

	RID extractGroups[] = { extractGroup, basicFSGroup };
	_extractItem = ds::compile(fsCmd, extractGroups, 2);

	// 
	// the state group for the horizontal blur
	//
	RID blurHGroup = ds::StateGroupBuilder()
		.vertexShader(fsVertexShader)
		.pixelShader(blurPSShader)
		.textureFromRenderTarget(bloomRT1, blurPSShader, 0)
		.samplerState(ssid, blurPSShader)
		.constantBuffer(blurBufferID, blurPSShader, 0)
		.build();
	RID blurHGroups[] = { blurHGroup, basicFSGroup };
	_blurrHItem = ds::compile(fsCmd, blurHGroups, 2);

	// 
	// the state group for the vertical blur
	//
	RID blurVGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(bloomRT2, blurPSShader, 0)
		.vertexShader(fsVertexShader)
		.pixelShader(blurPSShader)
		.samplerState(ssid, blurPSShader)
		.constantBuffer(blurBufferID, blurPSShader, 0)
		.build();

	RID blurVGroups[] = { blurVGroup, basicFSGroup };
	_blurrVItem = ds::compile(fsCmd, blurVGroups, 2);

	// 
	// the state group to get the final render target on screen
	//
	RID bloomCombineGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(_renderTarget, bloomPSShader, 0)
		.textureFromRenderTarget(bloomRT1, bloomPSShader, 1)
		.vertexShader(fsVertexShader)
		.pixelShader(bloomPSShader)
		.constantBuffer(bloomBufferID, bloomPSShader, 1)
		.samplerState(ssid, bloomPSShader)
		.build();

	RID bloomCombineGroups[] = { bloomCombineGroup, basicFSGroup };
	_bloomCombineItem = ds::compile(fsCmd, bloomCombineGroups, 2);
}

void BloomComponent::render() {
	// extract bloom -> rt2
	ds::submit(_bloomRT1Pass, _extractItem);
	// render full screen quad using RT as texture
	_blurBuffer.direction = ds::vec2(1, 0);
	// rt3
	ds::submit(_bloomRT2Pass, _blurrHItem);
	_blurBuffer.direction = ds::vec2(0, 1);
	// rt2
	// FIXME: 
	ds::submit(_bloomRT1Pass, _blurrVItem);
	// render full screen quad using RT as texture
	ds::submit(_finalRenderPass, _bloomCombineItem);
}