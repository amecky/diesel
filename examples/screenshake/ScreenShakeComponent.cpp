#include "ScreenShakeComponent.h"



ScreenShakeComponent::ScreenShakeComponent(ScreenShakeSettings* settings) : _settings(settings) {
	// load shader
	RID screenShakeVS = ds::loadVertexShader("Screenshake_vs.cso");
	RID screenShakePS = ds::loadPixelShader("Screenshake_ps.cso");
	// create state group
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
	// create draw item
	// build render pass
}

ScreenShakeComponent::~ScreenShakeComponent() {
}
