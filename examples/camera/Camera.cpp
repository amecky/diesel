#include "Camera.h"
#include "..\..\diesel.h"
#include <stdio.h>	
#include <cmath>
	// Pitch should be in the range of [-90 ... 90] degrees and yaw
	// should be in the range of [0 ... 360] degrees.
	matrix FPSViewRH(const v3& eye, float pitch, float yaw)
	{
		// If the pitch and yaw angles are in degrees,
		// they need to be converted to radians. Here
		// I assume the values are already converted to radians.
		float cosPitch = cos(pitch);
		float sinPitch = sin(pitch);
		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);

		v3 xaxis = { cosYaw, 0, -sinYaw };
		v3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
		v3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

		// Create a 4x4 view matrix from the right, up, forward and eye position vectors
		matrix viewMatrix = {
			xaxis.x, yaxis.x, zaxis.x, 0,
			xaxis.y, yaxis.y, zaxis.y, 0,
			xaxis.z, yaxis.z, zaxis.z, 0,
			-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1
		};

		return viewMatrix;
	}


	OldFPSCamera::OldFPSCamera(float screenWidth, float screenHeight)  {
		_projectionMatrix = mat_PerspectiveFovLH(ds::PI * 0.25f, screenWidth / screenHeight, 0.01f, 100.0f);
		_position = v3(3, 3, -12);
		_lastMousePos = ds::getMousePosition();
		_target = v3(0, 0, 1);
		_up = v3(0, 1, 0);
		_right = v3(1, 0, 0);
		_speed = 10.0f;
		_yaw = 0.0f;
		_pitch = 0.0f;
		_viewMatrix = mat_LookAtLH(_position, _target, _up);
		_viewProjectionMatrix = _viewMatrix * _projectionMatrix;
		buildView();
	}

	OldFPSCamera::~OldFPSCamera() {

	}

	void OldFPSCamera::setPosition(const v3& pos, const v3& target) {
		_position = pos;
		_target = target;
		_viewMatrix = mat_LookAtLH(_position, _target, _up);
		_viewProjectionMatrix = _viewMatrix * _projectionMatrix;
		buildView();
	}

	void OldFPSCamera::move(float unit) {
		//v3 tmp = _target * unit;
		//matrix R = mat_RotationX(_pitch);
		//tmp = R * tmp;
		_position += _target * unit;
		buildView();
	}

	void OldFPSCamera::strafe(float unit) {
		//v3 tmp = _right * unit;
		_position += _right * unit;
		buildView();
	}

	void OldFPSCamera::up(float unit) {
		v3 tmp = _up * unit;
		_position = _position + tmp;
		buildView();
	}

	void OldFPSCamera::setPitch(float angle) {
		_pitch += angle;
		matrix R = mat_Rotation(_right,angle);
		_up = mat_TransformNormal(_up, R);
		_target = mat_TransformNormal(_target, R);
		buildView();
	}

	void OldFPSCamera::resetYaw(float angle) {
		_yaw = angle;
		matrix R = mat_RotationZ(_yaw);
		_right = R * v3(1, 0, 0);
		_target = R * v3(0, 0, 1);
		buildView();
	}

	void OldFPSCamera::setYaw(float angle) {
		_yaw += angle;

		matrix R = mat_RotationY(angle);
		_right = mat_TransformNormal(_right, R);
		_up = mat_TransformNormal(_up, R);
		_target = mat_TransformNormal(_target, R);

		//matrix R = mat_RotationZ(_yaw);
		//_right = R * v3(1, 0, 0);
		//_target = R * v3(0, 0, 1);
		buildView();
	}
	/*
	void FPSCamera::setPitch(float angle) {
		_pitch -= angle;
		matrix R = mat_RotationX(_pitch);
		_right = R * v3(1, 0, 0);
		_target = R * v3(0, 0, 1);
		buildView();
	}
	*/
	void OldFPSCamera::resetPitch(float angle) {
		_pitch = angle;
		matrix R = mat_RotationY(_pitch);
		_right = R * v3(1, 0, 0);
		_target = R * v3(0, 0, 1);
		buildView();
	}

	void OldFPSCamera::update(float elapsedTime) {
		if (ds::isKeyPressed('W')) {
			move(5.0f*elapsedTime);
		}
		if (ds::isKeyPressed('S')) {
			move(-5.0f*elapsedTime);
		}
		if (ds::isKeyPressed('A')) {
			strafe(-5.0f*elapsedTime);
		}
		if (ds::isKeyPressed('D')) {
			strafe(5.0f*elapsedTime);
		}
		if (ds::isKeyPressed('Q')) {
			up(5.0f*elapsedTime);
		}
		if (ds::isKeyPressed('E')) {
			up(-5.0f*elapsedTime);
		}
		v2 mp = ds::getMousePosition();
		if (ds::isMouseButtonPressed(0)) {
			
			// Make each pixel correspond to a quarter of a degree.
			float dx = DEGTORAD(0.25f*(mp.x - _lastMousePos.x));
			float dy = DEGTORAD(0.25f*(mp.y - _lastMousePos.y));
			//printf("dx %g dy %g\n", dx, dy);
			setPitch(dy);
			setYaw(dx);			
		}
		_lastMousePos.x = mp.x;
		_lastMousePos.y = mp.y;
		
	}

	void OldFPSCamera::buildView() {
		//_viewMatrix = FPSViewRH(_position, _pitch, _yaw);
		_viewMatrix = mat_LookAtLH(_position, _target, _up);
		_viewProjectionMatrix = _viewMatrix * _projectionMatrix;
	}
