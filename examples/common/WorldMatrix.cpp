#include "WorldMatrix.h"

WorldMatrix::WorldMatrix() {
	_position = v3(0.0f);
	_scale = v3(1.0f);
	_rotation = v3(0.0f);
	computeMatrix();
}

void WorldMatrix::computeMatrix() {
	_world = mat_Translate(_position);
	_rotationX = mat_RotationX(_rotation.x);
	_rotationY = mat_RotationY(_rotation.y);
	_rotationZ = mat_RotationZ(_rotation.z);
	_scaleMatrix = mat_Scale(_scale);
	_finalMatrix = _rotationZ * _rotationY * _rotationX * _scaleMatrix * _world;
	_transposedFinalMatrix = mat_Transpose(_finalMatrix);
}