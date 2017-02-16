#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID shaderID = ds::createShader();
	ds::loadVertexShader(shaderID, "Fullscreen_vs.cso");
	ds::loadPixelShader(shaderID, "Fullscreen_ps.cso");
		
	RID rasterizerStateID = ds::createRasterizerState(ds::CullMode::BACK, ds::FillMode::SOLID, true, false, 0.0f, 0.0f);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);
		
	while (ds::isRunning()) {
		ds::begin();
			
		ds::setVertexDeclaration(NO_RID);
		ds::setIndexBuffer(NO_RID);
		ds::setVertexBuffer(NO_RID, 0, 0, ds::PrimitiveTypes::TRIANGLE_LIST);
		ds::setBlendState(bs_id);
		ds::setShader(shaderID);
		ds::setSamplerState(ssid);
		ds::setRasterizerState(rasterizerStateID);
		ds::draw(3);
		ds::end();
	}
	ds::shutdown();
}