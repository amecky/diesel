#pragma once
#include "..\..\diesel.h"

class worldMatrix {

public:
	worldMatrix();
	~worldMatrix() {}
	void reset() {
		_position = ds::vec3(0.0f);
		_scale = ds::vec3(1.0f);
		_rotation = ds::vec3(0.0f);
		computeMatrix();
	}
	const ds::matrix& getMatrix() const {
		return _finalMatrix;
	}
	const ds::matrix& getTransposedMatrix() const {
		return _transposedFinalMatrix;
	}
	void setRotation(const ds::vec3& r) {
		_rotation = r;
		computeMatrix();
	}
	void rotateBy(const ds::vec3& r) {
		_rotation += r;
		computeMatrix();
	}
	void setScale(const ds::vec3& s) {
		_scale = s;
	}
	void scaleBy(const ds::vec3& s) {
		_scale += s;
		computeMatrix();
	}
	void scaleBy(float s) {
		_scale.x += s;
		_scale.y += s;
		_scale.z += s;
		computeMatrix();
	}
	void setPosition(const ds::vec3& p) {
		_position = p;
		computeMatrix();
	}
	void moveBy(const ds::vec3& p) {
		_position += p;
		computeMatrix();
	}
private:
	void computeMatrix();
	ds::vec3 _position;
	ds::vec3 _scale;
	ds::vec3 _rotation;
	ds::matrix _world;
	ds::matrix _rotationX;
	ds::matrix _rotationY;
	ds::matrix _rotationZ;
	ds::matrix _scaleMatrix;
	ds::matrix _finalMatrix;
	ds::matrix _transposedFinalMatrix;
};
