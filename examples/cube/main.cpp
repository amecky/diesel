#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define MATH_IMPLEMENTATION
#include "..\..\math.h"

struct Vertex {
	float x;
	float y;
	float z;
	ds::Color color;

	Vertex() : x(0.0f), y(0.0f), z(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(float xp, float yp, float zp, const ds::Color& c) : x(xp), y(yp), z(zp), color(c) {}
	Vertex(const v3& p, const ds::Color& c) : x(p.x), y(p.y), z(p.z), color(c) {}
};

static Vertex s_cubeVertices[8] =
{
	{ -1.0f,  1.0f,  1.0f,ds::Color(1.0f,1.0f,1.0f,1.0f) },
	{ 1.0f,  1.0f,  1.0f, ds::Color(0.0f,1.0f,1.0f,1.0f) },
	{ -1.0f, -1.0f,  1.0f, ds::Color(1.0f,0.0f,1.0f,1.0f) },
	{ 1.0f, -1.0f,  1.0f, ds::Color(1.0f,1.0f,0.0f,1.0f) },
	{ -1.0f,  1.0f, -1.0f, ds::Color(0.0f,0.0f,1.0f,1.0f) },
	{ 1.0f,  1.0f, -1.0f, ds::Color(1.0f,0.0f,0.0f,1.0f) },
	{ -1.0f, -1.0f, -1.0f, ds::Color(0.0f,1.0f,0.0f,1.0f) },
	{ 1.0f, -1.0f, -1.0f, ds::Color(1.0f,0.0f,1.0f,1.0f) },
};

// 12
// 03
// v0 / v2  -1,-1,1 / 1,1,1 v0/v0.x,v2.y,1.0/v2,v2.x,v0.y,1.0

static Vertex s_plane[4] =
{
	{ -1.0f, -1.0f,  1.0f, ds::Color(1.0f,0.0f,0.0f,1.0f) },
	{ -1.0f,  1.0f,  1.0f, ds::Color(1.0f,0.0f,0.0f,1.0f) },
	{  1.0f,  1.0f,  1.0f, ds::Color(1.0f,0.0f,0.0f,1.0f) },
	{  1.0f, -1.0f,  1.0f, ds::Color(1.0f,0.0f,0.0f,1.0f) }
};

void addPlane(const v3& p0, const v3& p2, const ds::Color& color, Vertex* vertices, int side) {
	int idx = side * 4;
	vertices[idx] = Vertex(p0, color);
	vertices[idx + 1] = Vertex(v3(p0.x,p2.y,p0.z), color);
	vertices[idx + 2] = Vertex(p2, color);
	vertices[idx + 3] = Vertex(v3(p2.x,p0.y,p0.z), color);
}

struct CubeCB {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
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

	uint32_t p_indices[6] = {
		0, 1, 2, 2, 3, 0,		
	};

	Vertex v[8];

	CubeCB constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	if (ds::init(rs)) {

		RID bs_id = ds::create_blend_state(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

		RID sid = ds::create_shader();
		ds::load_vertex_shader(sid, "Coloured_vs.cso");
		ds::load_pixel_shader(sid, "Coloured_ps.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
		};

		RID rid = ds::create_vertex_declaration(decl, 2, sid);
		RID cbid = ds::create_consant_buffer(sizeof(CubeCB));
		RID iid = ds::create_index_buffer(ds::BufferType::STATIC, p_indices,6);
		RID vbid = ds::create_vertex_buffer(ds::BufferType::STATIC, 4, 0, s_plane,sizeof(Vertex));
		RID ssid = ds::create_sampler_state(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		
		v3 scale(1.0f, 1.0f, 1.0f);
		v3 rotation(0.0f, 0.0f, 0.0f);
		while (ds::isRunning()) {
			ds::begin();
			t += 0.001f;
			//rotation.z += 0.001f;
			matrix world = mat_identity();
			matrix w = mat_identity();
			//w = mat_Translate(v3(0.0f, 0.0f, 12.0f));
			matrix rotY = mat_RotationY(rotation.y);
			matrix rotX = mat_RotationX(rotation.x);
			matrix rotZ = mat_RotationZ(rotation.z);
			matrix s = mat_Scale(scale);
			w = rotZ * rotY * rotX * s * world;
			unsigned int stride = sizeof(Vertex);
			unsigned int offset = 0;

			ds::set_vertex_buffer(vbid, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
			ds::set_vertex_declaration(rid);
			ds::set_index_buffer(iid);
			ds::set_blend_state(bs_id);
			ds::set_shader(sid);
			constantBuffer.viewProjectionMatrix = mat_Transpose(ds::get_view_projection_matrix());
			constantBuffer.worldMatrix = mat_Transpose(w);
			ds::update_constant_buffer(cbid, &constantBuffer, sizeof(CubeCB));
			ds::set_vertex_constant_buffer(cbid);
			ds::drawIndexed(36);
			ds::end();
		}
		ds::shutdown();
	}
}