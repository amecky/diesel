#define DS_IMPLEMENTATION
#define DS_MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"
#include "..\common\worldMatrix.h"
#include "..\common\Camera.h"
#include "Bump_VS_Main.h"
#include "Bump_PS_Main.h"
#include "..\common\Textured_VS_Main.h"
#include "..\common\Textured_PS_Main.h"
// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	ds::vec3 p;
	ds::vec2 uv;
	ds::vec3 n;
	ds::vec3 t;

	Vertex() : p(0.0f), uv(0.0f) , n(0.0f) , t(0.0f) {}
	Vertex(const ds::vec3& pv, const ds::vec2& uvv, const ds::vec3& nv,const ds::vec3& tv) : p(pv), uv(uvv) , n(nv), t(tv) {}
};

// ---------------------------------------------------------------
// the cube constant buffer
// ---------------------------------------------------------------
struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
	ds::vec3 eyePos;
	float padding;
};

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name,ds::TextureFormat format) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(ds::TextureDesc()
		.Width(x)
		.Height(y)
		.Channels(n)
		.Data(data)
		.Format(ds::TextureFormat::R8G8B8A8_UNORM)
		.BindFlags(ds::BindFlag::BF_SHADER_RESOURCE));
	stbi_image_free(data);
	return textureID;
}

void saveObj(Vertex* vertices, int num) {
	std::vector<int> vertIndices;
	std::vector<int> uvIndices;
	std::vector<ds::vec3> vertCache;
	for (int i = 0; i < num; ++i) {
		bool found = false;
		for (int j = 0; j < vertCache.size(); ++j) {
			if (vertCache[j] == vertices[i].p) {
				found = true;
				vertIndices.push_back(j + 1);
			}
		}
		if (!found) {
			vertCache.push_back(vertices[i].p);
			vertIndices.push_back(vertCache.size());
		}
	}
	std::vector<ds::vec2> uvCache;
	for (int i = 0; i < num; ++i) {
		bool found = false;
		for (int j = 0; j < uvCache.size(); ++j) {
			if (uvCache[j] == vertices[i].uv) {
				found = true;
				uvIndices.push_back(j + 1);
			}
		}
		if (!found) {
			uvCache.push_back(vertices[i].uv);
			uvIndices.push_back(uvCache.size());
		}
	}

	FILE* fp = fopen("test.txt","w");
	if (fp) {
		for (int i = 0; i < vertCache.size(); ++i) {
			const ds::vec3& p = vertCache[i];
			fprintf(fp, "v %g %g %g\n", p.x, p.y, p.z);
		}
		for (int i = 0; i < uvCache.size(); ++i) {
			const ds::vec2& p = uvCache[i];
			fprintf(fp, "vt %g %g\n", p.x, p.y);
		}
		int faces = vertIndices.size() / 4;
		for (int i = 0; i < faces; ++i) {
			int idx = i * 4;
			fprintf(fp, "f %d/%d %d/%d %d/%d %d/%d\n", vertIndices[idx], uvIndices[idx], vertIndices[idx + 1], uvIndices[idx + 1], vertIndices[idx + 2], uvIndices[idx + 2], vertIndices[idx + 3], uvIndices[idx + 3]);
		}
		fclose(fp);
	}
}

void calculateTangents(ds::vec3* positions, ds::vec2* uvs, ds::vec3* tangents, int num) {
	int steps = num / 4;
	for (int i = 0; i < steps; ++i) {
		// Shortcuts for vertices
		ds::vec3& vp0 = positions[i * 4 + 0];
		ds::vec3& vp1 = positions[i * 4 + 1];
		ds::vec3& vp2 = positions[i * 4 + 2];

		// Shortcuts for UVs
		ds::vec2 uv0 = uvs[i * 4 + 0];
		ds::vec2 uv1 = uvs[i * 4 + 1];
		ds::vec2 uv2 = uvs[i * 4 + 2];

		// Edges of the triangle : postion delta
		ds::vec3 deltaPos1 = vp1 - vp0;
		ds::vec3 deltaPos2 = vp2 - vp0;

		// UV delta
		ds::vec2 deltaUV1 = uv1 - uv0;
		ds::vec2 deltaUV2 = uv2 - uv0;
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		ds::vec3 tangent = normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r);
		tangents[i * 4] = tangent;
		tangents[i * 4 + 1] = tangent;
		tangents[i * 4 + 2] = tangent;
		tangents[i * 4 + 3] = tangent;
	}
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	ds::vec3 positions[24];
	ds::vec3 normals[24];
	ds::vec3 tangents[24];
	ds::vec2 uvs[24];
	ds::matrix m = ds::matIdentity();
	geometry::buildCube(m, positions, uvs, normals);
	calculateTangents(positions, uvs, tangents, 24);
	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = Vertex(positions[i], uvs[i], normals[i],tangents[i]);
	}

	geometry::MeshData meshData;
	geometry::buildSphere(0.5f, 20, 20, &meshData);
	Vertex sv[401];
	for (int i = 0; i < 401; ++i) {
		sv[i] = Vertex(meshData.vertices[i], meshData.uvs[i], meshData.normals[i], meshData.tangents[i]);
	}
	int indices[2280];
	for (int i = 0; i < 2280; ++i) {
		indices[i] = meshData.indices[i];
	}

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 1;
	ds::init(rs);

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 2.0f, -3.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	ds::Camera camera = {
		viewMatrix,
		projectionMatrix,
		viewMatrix * projectionMatrix,
		ds::vec3(0,2,-3),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::ViewportInfo vpInfo = { 1024,768,0.0f,1.0f };
	RID vp = ds::createViewport(ds::ViewportDesc()
		.Top(0)
		.Left(0)
		.Width(ds::getScreenWidth())
		.Height(ds::getScreenHeight())
		.MinDepth(0.0f)
		.MaxDepth(1.0f)
	);
	
	RID basicPass = ds::createRenderPass(ds::RenderPassDesc()
		.Camera(&camera)
		.Viewport(vp)
		.DepthBufferState(ds::DepthBufferState::ENABLED)
		.NumRenderTargets(0)
		.RenderTargets(0));

	RID textureID = loadImage("..\\common\\cube_map.png", ds::TextureFormat::R8G8B8A8_UNORM);
	RID cubeTextureID = loadImage("Hex.png", ds::TextureFormat::R8G8B8A8_UNORM_SRGB);
	RID cubeNormalID = loadImage("Hex_Normal.png", ds::TextureFormat::R8G8B8A8_UNORM_SRGB);

	RID bs_id = ds::createBlendState(ds::BlendStateDesc()
		.SrcBlend(ds::BlendStates::SRC_ALPHA)
		.SrcAlphaBlend(ds::BlendStates::SRC_ALPHA)
		.DestBlend(ds::BlendStates::INV_SRC_ALPHA)
		.DestAlphaBlend(ds::BlendStates::INV_SRC_ALPHA)
		.AlphaEnabled(true));

	ds::ShaderInfo texturedVSInfo = { 0, Textured_VS_Main, sizeof(Textured_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID texturedVS = ds::createShader(texturedVSInfo);
	ds::ShaderInfo texturedPSInfo = { 0, Textured_PS_Main, sizeof(Textured_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID texturedPS = ds::createShader(texturedPSInfo);

	ds::ShaderInfo bumpVSInfo = { 0, Bump_VS_Main, sizeof(Bump_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID bumpVS = ds::createShader(bumpVSInfo);
	ds::ShaderInfo bumpPSInfo = { 0, Bump_PS_Main, sizeof(Bump_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID bumpPS = ds::createShader(bumpPSInfo);

	Grid grid(&camera);
	ds::vec3 gridPositions[] = {
		ds::vec3(-4.0f, -1.0f, -3.5f),
		ds::vec3(-4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f,  4.5f),
		ds::vec3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 2, texturedVS, texturedPS, textureID, basicPass);

	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0,ds::BufferAttributeType::FLOAT3 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 },
		{ "NORMAL"  , 0, ds::BufferAttributeType::FLOAT3 },
		{ "TANGENT" , 0 ,ds::BufferAttributeType::FLOAT3 }
	};

	ds::InputLayoutInfo layoutInfo = { decl, 4, bumpVS };
	RID rid = ds::createInputLayout(layoutInfo);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 24, sizeof(Vertex), v };
	RID cubeBuffer = ds::createVertexBuffer(vbInfo);

	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR };
	RID ssid = ds::createSamplerState(samplerInfo);

	ds::VertexBufferInfo sphereInfo = { ds::BufferType::STATIC, 401, sizeof(Vertex), sv };
	RID sphereBuffer = ds::createVertexBuffer(sphereInfo, "sphere_vertex_buffer");
	ds::IndexBufferInfo idxInfo = { 2280, ds::IndexType::UINT_32, ds::BufferType::STATIC, indices };
	RID sphereIndexBuffer = ds::createIndexBuffer(idxInfo, "sphere_index_buffer");

	worldMatrix wm;

	FPSCamera fpsCamera(&camera);
	fpsCamera.setPosition(ds::vec3(0, 2, -8));

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, bumpVS, 0)
		.constantBuffer(cbid, bumpPS, 0)
		.blendState(bs_id)
		.samplerState(ssid, bumpPS)
		.vertexBuffer(cubeBuffer)
		.vertexShader(bumpVS)
		.pixelShader(bumpPS)
		.indexBuffer(indexBuffer)
		.texture(cubeTextureID, bumpPS, 0)
		.texture(cubeNormalID, bumpPS, 1)
		.build();


	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID sphereGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, bumpVS, 0)
		.constantBuffer(cbid, bumpPS, 0)
		.blendState(bs_id)
		.samplerState(ssid, bumpPS)
		.vertexBuffer(sphereBuffer)
		.vertexShader(bumpVS)
		.pixelShader(bumpPS)
		.indexBuffer(sphereIndexBuffer)
		.texture(cubeTextureID, bumpPS, 0)
		.texture(cubeNormalID, bumpPS, 1)
		.build();

	ds::DrawCommand sphereCmd = { 2280, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	RID drawItem = ds::compile(drawCmd, stateGroup);

	RID sphereDrawItem = ds::compile(sphereCmd, sphereGroup);
		
	while (ds::isRunning()) {
		ds::begin();

		fpsCamera.update(static_cast<float>(ds::getElapsedSeconds()));
		ds::matrix vpm = camera.viewProjectionMatrix;

		grid.render();
			
		constantBuffer.viewprojectionMatrix = ds::matTranspose(vpm);
		wm.setPosition(ds::vec3(-2, 0, 0));
		wm.rotateBy(ds::vec3(0.0f, 1.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		constantBuffer.eyePos = camera.position;
		constantBuffer.padding = 0.0f;
		
		ds::submit(basicPass, drawItem);

		wm.setPosition(ds::vec3(2, 0, 0));
		wm.rotateBy(ds::vec3(0.0f, 1.0f  * static_cast<float>(ds::getElapsedSeconds()), 0.0f));
		constantBuffer.worldMatrix = wm.getTransposedMatrix();
		constantBuffer.eyePos = camera.position;
		constantBuffer.padding = 0.0f;

		ds::submit(basicPass, sphereDrawItem);

		ds::end();
	}
	ds::shutdown();
	return 0;
}