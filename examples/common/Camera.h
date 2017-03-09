#pragma once
#include "..\..\diesel.h"
#include <cmath>
#include "math.h"

class FPSCamera {

public:
	FPSCamera(float screenWidth, float screenHeight);
	virtual ~FPSCamera();
	void setPosition(const v3& pos);
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

	void getViewProjectionMatrix(float* result) const {
		for (int i = 0; i < 16; ++i) {
			result[i] = _viewProjectionMatrix[i];
		}
	}
	void getViewMatrix(float* result) const {
		for (int i = 0; i < 16; ++i) {
			result[i] = _viewMatrix[i];
		}
	}
	void getProjectionMatrix(float* result) const {
		for (int i = 0; i < 16; ++i) {
			result[i] = _projectionMatrix[i];
		}
	}
private:
	void buildView();
	// rotation around X axis
	float _roll;
	// rotation around Y axis
	float _pitch;
	// rotation around Z axis
	float _yaw;	

	float _position[3];
	float _target[3];
	float _up[3];
	float _right[3];
	float _speed;
	float _lastMousePos[2];
	float _viewMatrix[16];
	float _projectionMatrix[16];
	float _viewProjectionMatrix[16];
};
