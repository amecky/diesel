#define DS_IMPLEMENTATION
#include "..\..\diesel.h"

struct Vertex {
	float x;
	float y;
	float z;
	ds::Color color;
};

static Vertex s_cubeVertices[8] =
{
	{ -1.0f,  1.0f,  1.0f,ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ 1.0f,  1.0f,  1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ -1.0f, -1.0f,  1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ 1.0f, -1.0f,  1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ -1.0f,  1.0f, -1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ 1.0f,  1.0f, -1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ -1.0f, -1.0f, -1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ 1.0f, -1.0f, -1.0f, ds::Color(1.0f,1.0f,1.0f,1.0f) },
};


struct CBTest {
	float one;
	float two;
	float three;
	float four;
};

int main(const char** args) {

	uint32_t indices[36] = {
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};

	CBTest test;

	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	if (ds::init(rs)) {

		RID bs_id = ds::create_blend_state(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::TEXCOORD0,ds::BufferAttributeType::FLOAT,2 },
			{ ds::BufferAttribute::COLOR0,ds::BufferAttributeType::FLOAT,4 }
		};

		RID rid = ds::create_vertex_declaration(decl, 3);
		RID cbid = ds::create_consant_buffer(sizeof(CBTest));
		printf("CB: %d\n", cbid);
		RID iid = ds::create_index_buffer(ds::BufferType::STATIC, indices);
		printf("IndexBuffer: %d (%zd)\n", iid,sizeof(indices));
		RID vbid = ds::create_vertex_buffer(ds::BufferType::STATIC, 8, 0, s_cubeVertices);
		printf("VertexBuffer: %d\n", vbid);
		RID ssid = ds::create_sampler_state(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		printf("sampler state: %d\n", ssid);
		RID sid = ds::create_shader();
		ds::load_vertex_shader(sid, "Coloured_vs.cso");
		ds::load_pixel_shader(sid, "Coloured_ps.cso");
		while (ds::isRunning()) {
			ds::begin();
			/*
			mat4 w = matrix::m4identity();
			mat4 rotY = matrix::mat4RotationY(rotation.y);
			mat4 rotX = matrix::mat4RotationX(rotation.x);
			mat4 rotZ = matrix::mat4RotationZ(rotation.z);
			mat4 s = matrix::mat4Scale(scale);
			w = rotZ * rotY * rotX * s * world;
			*/
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			ds::set_vertex_buffer(vbid, &stride, &offset, ds::PrimitiveTypes::POINT_LIST);
			ds::set_index_buffer(iid);
			ds::set_blend_state(bs_id);
			/*
			setShader(m->shader);
			if (m->texture != INVALID_RID) {
				setPixelShaderResourceView(m->texture);
			}
			Camera* camera = graphics::getCamera();
			ds::mat4 mvp = w * camera->getViewProjectionMatrix();
			_buffer.viewProjectionMatrix = ds::matrix::mat4Transpose(mvp);
			_buffer.worldMatrix = ds::matrix::mat4Transpose(w);
			_buffer.cameraPos = camera->getPosition();
			_buffer.lightPos = _lightPos;
			_buffer.diffuseColor = color;// Color(192, 0, 0, 255);
			//graphics::mapData(_descriptor.vertexBuffer, mesh->vertices.data(), mesh->vertices.size() * sizeof(PNTCVertex));
			*/
			ds::update_constant_buffer(cbid, &test, sizeof(CBTest));
			
			ds::set_vertex_constant_buffer(cbid);
			ds::set_pixel_constant_buffer(cbid);
			//ds::drawIndexed(36);

			ds::end();
		}
		ds::shutdown();
	}
}