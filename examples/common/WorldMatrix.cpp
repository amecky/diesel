#include "worldMatrix.h"

worldMatrix::worldMatrix() {
	_position = ds::vec3(0.0f);
	_scale = ds::vec3(1.0f);
	_rotation = ds::vec3(0.0f);
	computeMatrix();
}

void worldMatrix::computeMatrix() {
	_world = ds::matTranslate(_position);
	_rotationX = ds::matRotationX(_rotation.x);
	_rotationY = ds::matRotationY(_rotation.y);
	_rotationZ = ds::matRotationZ(_rotation.z);
	_scaleMatrix = ds::matScale(_scale);
	_finalMatrix = _rotationZ * _rotationY * _rotationX * _scaleMatrix * _world;
	_transposedFinalMatrix = ds::matTranspose(_finalMatrix);
}