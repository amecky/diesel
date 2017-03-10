#pragma once
#include "..\..\diesel.h"

class WorldMatrix {

public:
	WorldMatrix();
	~WorldMatrix() {}
	void reset() {
		_position = v3(0.0f);
		_scale = v3(1.0f);
		_rotation = v3(0.0f);
		computeMatrix();
	}
	const matrix& getMatrix() const {
		return _finalMatrix;
	}
	const matrix& getTransposedMatrix() const {
		return _transposedFinalMatrix;
	}
	void setRotation(const v3& r) {
		_rotation = r;
		computeMatrix();
	}
	void rotateBy(const v3& r) {
		_rotation += r;
		computeMatrix();
	}
	void setScale(const v3& s) {
		_scale = s;
	}
	void scaleBy(const v3& s) {
		_scale += s;
		computeMatrix();
	}
	void scaleBy(float s) {
		_scale.x += s;
		_scale.y += s;
		_scale.z += s;
		computeMatrix();
	}
	void setPosition(const v3& p) {
		_position = p;
		computeMatrix();
	}
	void moveBy(const v3& p) {
		_position += p;
		computeMatrix();
	}
private:
	void computeMatrix();
	v3 _position;
	v3 _scale;
	v3 _rotation;
	matrix _world;
	matrix _rotationX;
	matrix _rotationY;
	matrix _rotationZ;
	matrix _scaleMatrix;
	matrix _finalMatrix;
	matrix _transposedFinalMatrix;
};
