#include "Mesh.h"
#include "..\..\diesel.h"
#include "..\tigre\AmbientLightning_PS_Main.h"
#include "..\tigre\AmbientLightning_VS_Main.h"
#define DS_OBJ_LOADER
#include "..\..\features\ds_obj_loader.h"

// The tigre https://sketchfab.com/models/95c4008c4c764c078f679d4c320e7b18# is taken from https://sketchfab.com/jeremielouvetz


namespace ds {

	void createMesh(Mesh* mesh, const char* fileName) {
		ds::ObjLoader objReader;
		int num = objReader.load(fileName);

		mesh->lightBuffer.ambientColor = ds::Color(0.15f, 0.15f, 0.15f, 1.0f);
		mesh->lightBuffer.diffuseColor = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
		mesh->lightBuffer.more = 0.0f;
		ds::vec3 lightPos = ds::vec3(1.0f, -0.5f, 1.0f);
		mesh->lightBuffer.lightDirection = normalize(lightPos);

		RID lightBufferID = ds::createConstantBuffer(sizeof(LightBuffer), &mesh->lightBuffer);

		mesh->vertices = new PNCVertex[num];

		for (size_t i = 0; i < objReader.size(); ++i) {
			const ds::ObjVertex ov = objReader.get(i);
			mesh->vertices[i] = PNCVertex{ ov.position ,ov.normal, ov.color };
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
			.Data(mesh->vertices)
			.NumVertices(num)
			.VertexSize(sizeof(PNCVertex))
		);

		delete[] mesh->vertices;

		RID basicGroup = ds::StateGroupBuilder()
			.inputLayout(nid)
			.constantBuffer(lightBufferID, vertexShader, 0)
			.constantBuffer(lightBufferID, pixelShader, 0)
			.vertexShader(vertexShader)
			.pixelShader(pixelShader)
			.vertexBuffer(cubeBuffer)
			.build();

		ds::DrawCommand drawCmd = { num, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST };

		mesh->drawItem = ds::compile(drawCmd, basicGroup);

		mesh->scale = ds::vec3(1.0f, 1.0f, 1.0f);
		mesh->rotation = ds::vec3(0.0f, 0.0f, 0.0f);
		mesh->pos = ds::vec3(0.0f, 0.0f, 0.0f);
	}

	void renderMesh(Mesh* mesh, RID renderPass, const ds::matrix& viewProjectionMatrix) {
		mesh->lightBuffer.viewProjectionMatrix = ds::matTranspose(viewProjectionMatrix);
		//ds::matrix world = ds::matIdentity();
		ds::matrix rotY = ds::matRotationY(mesh->rotation.y);
		ds::matrix rotX = ds::matRotationX(mesh->rotation.x);
		ds::matrix rotZ = ds::matRotationZ(mesh->rotation.z);
		ds::matrix s = ds::matScale(mesh->scale);
		ds::matrix world = ds::matTranslate(mesh->pos);
		ds::matrix w = rotZ * rotY * rotX * s * world;
		mesh->lightBuffer.worldMatrix = ds::matTranspose(w);
		ds::submit(renderPass, mesh->drawItem);
	}

	void moveMeshByKeyboard(Mesh* mesh, float velocity, float dt) {
		ds::vec3 vel;
		if (ds::isKeyPressed('W')) {
			vel.y = 1.0f;
		}
		if (ds::isKeyPressed('S')) {
			vel.y = -1.0f;
		}
		if (ds::isKeyPressed('A')) {
			vel.x = -1.0f;
		}
		if (ds::isKeyPressed('D')) {
			vel.x = 1.0f;
		}
		mesh->pos += vel * velocity * dt;
	}

}