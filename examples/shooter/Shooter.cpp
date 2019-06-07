#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define DS_BILLBOARDS
#include "..\..\features\ds_billboards.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "Starfield.h"
#include "Mesh.h"
#include "Bullets.h"
#include "enemies.h"

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
	rs.title = "Shooter";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	RID textureID = loadImage("particles.png");

	Starfield starField;

	ds::Mesh player;
	ds::createMesh(&player, "ring.obj");
	player.scale = ds::vec3(0.6f);

	ds::Mesh pulsator;
	ds::createMesh(&pulsator, "pulsator.obj");

	Enemies enemines;

	Bullets bullets;
	bullets.num = 0;
	bullets.capacity = 512;

	billboards::init(textureID, 4096);
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

	bool shooting = false;
	float shootTimer = 0.0f;
	float shootDelay = 0.4f;

	while (ds::isRunning()) {

		ds::begin();

		starField.moveStars(ds::getElapsedSeconds());

		billboards::begin();

		starField.createBillboards();

		billboards::render(basicPass, camera.viewProjectionMatrix);

		billboards::end();

		if (ds::isMouseButtonPressed(0) && !shooting) {
			shooting = true;
		}
		if (!ds::isMouseButtonPressed(0) && shooting) {
			shooting = false;
		}

		shootTimer += ds::getElapsedSeconds();
		if (shooting && shootTimer >= shootDelay) {
			if (bullets.capacity > (bullets.num + 1)) {
				Bullet& b = bullets.bullet[bullets.num++];
				b.pos = player.pos;
				b.velocity = ds::vec3(9.0f, 0.0f);
			}
			shootTimer = 0.0f;
		}
		
		move_bullets(&bullets, ds::getElapsedSeconds());

		billboards::begin();

		createBulletBillboards(&bullets);

		billboards::render(basicPass, camera.viewProjectionMatrix);

		billboards::end();

		ds::moveMeshByKeyboard(&player, 5.0f, ds::getElapsedSeconds());

		player.rotation.x += 5.0f  * static_cast<float>(ds::getElapsedSeconds());

		ds::renderMesh(&player, basicPass, camera.viewProjectionMatrix);

		pulsator.rotation.x += ds::getElapsedSeconds() * 3.0f;

		ds::renderMesh(&pulsator, basicPass, camera.viewProjectionMatrix);

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());

		ds::dbgPrint(0, 1, "Bullets: %d", bullets.num);

		ds::end();
	}

	billboards::shutdown();

	ds::shutdown();
	return 0;
}