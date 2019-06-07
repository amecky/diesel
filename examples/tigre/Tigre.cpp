#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "AmbientLightning_PS_Main.h"
#include "AmbientLightning_VS_Main.h"
#define DS_OBJ_LOADER
#include "..\..\features\ds_obj_loader.h"

// The tigre https://sketchfab.com/models/95c4008c4c764c078f679d4c320e7b18# is taken from https://sketchfab.com/jeremielouvetz
struct PNCVertex {
	ds::vec3 p;
	ds::vec3 n;
	ds::Color c;
};

struct LightBuffer {
	ds::matrix viewProjectionMatrix;
	ds::matrix worldMatrix;
	ds::Color ambientColor;
	ds::vec3 lightDirection;
	float more;
	ds::Color diffuseColor;	
};


RID buildTigreDrawItem(RID lightBufferID) {
	ds::ObjLoader objReader;
	int num = objReader.load("tigre_sumatra.obj");

	PNCVertex* v = new PNCVertex[num];

	for (size_t i = 0; i < objReader.size(); ++i) {
		const ds::ObjVertex ov = objReader.get(i);
		v[i] = PNCVertex{ ov.position ,ov.normal, ov.color };
	}

	RID vertexShader = ds::createShader(ds::ShaderDesc()
		.Data(AmbientLightning_VS_Main)
		.DataSize(sizeof(AmbientLightning_VS_Main))
		.ShaderType(ds::ShaderType::ST_VERTEX_SHADER));

	RID pixelShader = ds::createShader(ds::ShaderDesc()
		.Data(AmbientLightning_PS_Main)
		.DataSize(sizeof(AmbientLightning_PS_Main))
		.ShaderType(ds::ShaderType::ST_PIXEL_SHADER));

	ds::InputLayoutDefinition tigreDecl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "NORMAL"  , 0, ds::BufferAttributeType::FLOAT3 },
		{ "COLOR"   , 0, ds::BufferAttributeType::FLOAT4 }
	};
	
	RID nid = ds::createInputLayout(ds::InputLayoutDesc()
		.Declarations(tigreDecl)
		.NumDeclarations(3)
		.VertexShader(vertexShader));


	RID cubeBuffer = ds::createVertexBuffer(ds::VertexBufferDesc()
		.BufferType(ds::BufferType::STATIC)
		.Data(v)
		.NumVertices(num)
		.VertexSize(sizeof(PNCVertex))
	);

	delete[] v;
	
	RID basicGroup = ds::StateGroupBuilder()
		.inputLayout(nid)
		.constantBuffer(lightBufferID, vertexShader, 0)
		.constantBuffer(lightBufferID, pixelShader, 0)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.vertexBuffer(cubeBuffer)
		.build();

	ds::DrawCommand drawCmd = { num, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID cubeItem = ds::compile(drawCmd, basicGroup);
	return cubeItem;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	LightBuffer lightBuffer;
	lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f, 1.0f);
	lightBuffer.diffuseColor = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
	lightBuffer.more = 0.0f;
	ds::vec3 lightPos = ds::vec3(1.0f, -0.5f, 1.0f);
	lightBuffer.lightDirection = normalize(lightPos);

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 1.0f, -2.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,1,-2),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0)
	};

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
	
	RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer), &lightBuffer);
	
	RID tigreDrawItem = buildTigreDrawItem(lightBufferID);

	ds::vec3 scale(1.0f, 1.0f, 1.0f);
	ds::vec3 rotation(0.0f, 0.0f, 0.0f);
	ds::vec3 pos(0.0f, 0.0f, 0.0f);
	
	while (ds::isRunning()) {
		ds::begin();

		t += static_cast<float>(ds::getElapsedSeconds());

		lightBuffer.viewProjectionMatrix = ds::matTranspose(camera.viewProjectionMatrix);
		ds::matrix world = ds::matIdentity();
		rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		//rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		ds::matrix rotY = ds::matRotationY(rotation.y);
		ds::matrix rotX = ds::matRotationX(rotation.x);
		ds::matrix rotZ = ds::matRotationZ(rotation.z);
		ds::matrix s = ds::matScale(scale);
		ds::matrix w = rotZ * rotY * rotX * s * world;
		lightBuffer.worldMatrix = ds::matTranspose(w);
		ds::submit(basicPass, tigreDrawItem);

		ds::end();
	}
	ds::shutdown();
	return 0;
}