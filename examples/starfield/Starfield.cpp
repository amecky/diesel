#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define DS_BILLBOARDS
#include "..\..\features\ds_billboards.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"

struct Star {
	ds::vec3 pos;
	ds::vec3 velocity;
	int layer;
};

const int STARFIELD_LAYERS = 3;
const int STARS_PER_LAYER = 128;
const int TOTAL_STARS = STARFIELD_LAYERS * STARS_PER_LAYER;

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(ds::TextureDesc()
		.Width(x)
		.Height(y)
		.Channels(n)
		.Data(data)
		.Format(ds::TextureFormat::R8G8B8A8_UNORM)
		.BindFlags(ds::BindFlag::BF_SHADER_RESOURCE)
	);
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	//
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Billboard demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	Star stars[TOTAL_STARS];

	RID textureID = loadImage("particles.png");

	for (int i = 0; i < STARFIELD_LAYERS; ++i) {
		for (int j = 0; j < STARS_PER_LAYER; ++j) {
			Star& s = stars[i * STARS_PER_LAYER + j];
			s.pos = ds::vec3(ds::random(-8.0f, 8.0f), ds::random(-4.0f, 4.0f), static_cast<float>(i));
			float v = (2.0f - static_cast<float>(i)) * 0.5f + ds::random(0.2f, 0.3f);
			s.velocity = ds::vec3(-v, 0.0f, 0.0f);
			s.layer = i;
		}
	}

	billboards::init(textureID, TOTAL_STARS);
	//
	// create render with view and projection matrix
	//
	ds::Camera camera = ds::buildPerspectiveCamera(ds::vec3(0, 0, -10));

	RID vp = ds::createViewport(ds::ViewportDesc()
		.Top(0)
		.Left(0)
		.Width(1024)
		.Height(768)
		.MinDepth(0.0f)
		.MaxDepth(1.0f)
	);
	
	RID basicPass = ds::createRenderPass(ds::RenderPassDesc()
		.Camera(&camera)
		.Viewport(vp)
		.DepthBufferState(ds::DepthBufferState::ENABLED)
		.RenderTargets(0)
		.NumRenderTargets(0));

	while (ds::isRunning()) {

		ds::begin();

		for (int i = 0; i < TOTAL_STARS; ++i) {
			Star& s = stars[i];
			s.pos += s.velocity * ds::getElapsedSeconds();
			if (s.pos.x < -10.0f) {
				s.pos.x = 10.0f;
				s.pos.y = ds::random(-8.0f, 8.0f);
				float v = (2.0f - static_cast<float>(s.layer)) * 0.5f + ds::random(0.2f, 0.3f);
				s.velocity = ds::vec3(-v, 0.0f, 0.0f);
			}
		}

		billboards::begin();

		for (int i = 0; i < TOTAL_STARS; ++i) {
			billboards::add(stars[i].pos, ds::vec2(0.05f, 0.05f), ds::vec4(0, 0, 128, 128));
		}		

		billboards::render(basicPass, camera.viewProjectionMatrix);

		billboards::end();

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());

		ds::end();
	}

	billboards::shutdown();

	ds::shutdown();
	return 0;
}