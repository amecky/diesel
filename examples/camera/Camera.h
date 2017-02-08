#pragma once
#include "..\..\math.h"


class FPSCamera {

public:
	FPSCamera(float screenWidth, float screenHeight);
	virtual ~FPSCamera();
	void setPosition(const v3& pos, const v3& lookAt);
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
	const v3& getPosition() const {
		return _position;
	}
	const v3& getTarget() const {
		return _target;
	}
	float getYaw() const {
		return _yaw;
	}

	const matrix& getViewProjectionMatrix() const {
		return _viewProjectionMatrix;
	}
	const matrix& getViewMatrix() const {
		return _viewMatrix;
	}
	const matrix& getProjectionMatrix() const {
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

	v3 _position;
	v3 _target;
	v3 _up;
	v3 _right;
	float _speed;
	v2 _lastMousePos;
	matrix _viewMatrix;
	matrix _projectionMatrix;
	matrix _viewProjectionMatrix;
};
