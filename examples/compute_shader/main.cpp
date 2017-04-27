#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
int main(int argc, char *argv[]) {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Spinning cube";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	RID computeShader = ds::loadComputeShader("desaturate.cso", "Desaturate");

	while (ds::isRunning()) {
		ds::begin();
		
		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Simple spinning cube demo");

		ds::end();
	}
	ds::shutdown();
}