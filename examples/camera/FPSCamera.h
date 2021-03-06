#pragma once
#include "..\..\math.h"


class FPSCamera {

public:
	FPSCamera(float screenWidth, float screenHeight);
	virtual ~FPSCamera();
	void setPosition(const ds::vec3& pos, const ds::vec3& lookAt);
	void setSpeed(float spd) {
		_speed = spd;
	}
	// moving the camera
	void move(float unit);
	void strafe(float unit);
	void up(float unit);
	// Y axis
	void setPitch(float angle);
	void resetPitch(float angle);
	// Z axis
	void resetYaw(float angle);
	void setYaw(float angle);
	//void resetYAngle();

	void update(float elapsedTime);

	float getPitch() const {
		return _pitch;
	}
	const ds::vec3& getPosition() const {
		return _position;
	}
	const ds::vec3& getTarget() const {
		return _target;
	}
	float getYaw() const {
		return _yaw;
	}

	const ds::matrix& getViewprojectionMatrix() const {
		return _viewprojectionMatrix;
	}
	const ds::matrix& getviewMatrix() const {
		return _viewMatrix;
	}
	const ds::matrix& getprojectionMatrix() const {
		return _projectionMatrix;
	}
private:
	void buildView();
	// rotation around X axis
	float _roll;
	// rotation around Y axis
	float _pitch;
	// rotation around Z axis
	float _yaw;	

	ds::vec3 _position;
	ds::vec3 _target;
	ds::vec3 _up;
	ds::vec3 _right;

	float _speed;

	ds::vec2 _lastMousePos;

	ds::matrix _viewMatrix;
	ds::matrix _projectionMatrix;
	ds::matrix _viewprojectionMatrix;
};
