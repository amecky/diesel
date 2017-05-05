#include "ScreenShakeComponent.h"



ScreenShakeComponent::ScreenShakeComponent(ScreenShakeSettings* settings) : _settings(settings), _active(false) , _timer(0.0f) {
	//
	// build render pass
	//
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
	ds::Camera camera = {
		orthoView,
		orthoProjection,
		orthoView * orthoProjection,
		ds::vec3(0,3,-6),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::RenderPassInfo rpInfo = { &camera, ds::DepthBufferState::DISABLED, 0, 0 };
	_orthoPass = ds::createRenderPass(rpInfo);
	//
	// load shader
	//
	RID screenShakeVS = ds::loadVertexShader("Screenshake_vs.cso");
	RID screenShakePS = ds::loadPixelShader("Screenshake_ps.cso");
	RID bufferID = ds::createConstantBuffer(sizeof(ScreenShakeBuffer), &_buffer);
	//
	// create state group
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
	RID screenShakeGroup = ds::StateGroupBuilder()
		.textureFromRenderTarget(_settings->renderTarget, screenShakePS, 0)
		.vertexShader(screenShakeVS)
		.pixelShader(screenShakePS)
		.constantBuffer(bufferID, screenShakeVS, 1)
		.build();
	//
	// create draw item
	//
	ds::DrawCommand fsCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };
	RID screenShakeGroups[] = { screenShakeGroup, basicFSGroup };
	_screenShakeItem = ds::compile(fsCmd, screenShakeGroups, 2);	
}

ScreenShakeComponent::~ScreenShakeComponent() {
}

// -----------------------------------------------------
// activate if not already active
// -----------------------------------------------------
void ScreenShakeComponent::activate() {
	if (!_active) {
		_active = true;
		_timer = 0.0f;
	}
}

// -----------------------------------------------------
// tick
// -----------------------------------------------------
void ScreenShakeComponent::tick(float elapsed) {
	if (_active) {
		_timer += elapsed;		
	}
}

// -----------------------------------------------------
// render
// -----------------------------------------------------
void ScreenShakeComponent::render() {
	if (_timer > _settings->ttl) {
		_active = false;
		_timer = 1.0f;
	}
	_buffer.amplitude = _settings->amplitude;
	_buffer.frequency = _settings->frequency;
	_buffer.timer = _timer;
	_buffer.ttl = _settings->ttl;
	ds::submit(_orthoPass, _screenShakeItem);
}