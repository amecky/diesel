#include "FPSCamera.h"
#define MATH_IMPLEMENTATION
#include "..\..\diesel.h"
#include <stdio.h>	



	FPSCamera::FPSCamera(float screenWidth, float screenHeight)  {
		_projectionMatrix = mat_PerspectiveFovLH(ds::PI * 0.25f, screenWidth / screenHeight, 0.01f, 100.0f);
		_position = v3(3, 3, -12);
		_lastMousePos = ds::getMousePosition();
		_target = v3(0.0f, 0.0f, 1.0f);
		_up = v3(0.0f, 1.0f, 0.0f);
		_right = v3(1.0f, 0.0f, 0.0f);
		_speed = 10.0f;
		_yaw = 0.0f;
		_pitch = 0.0f;
		_viewMatrix = mat_LookAtLH(_position, _target, _up);
		_viewProjectionMatrix = _viewMatrix * _projectionMatrix;
		buildView();
	}

	FPSCamera::~FPSCamera() {

	}

	void FPSCamera::setPosition(const v3& pos, const v3& target) {
		_position = pos;
		_target = target;
		_viewMatrix =mat_LookAtLH(_position, _target, _up);
		_viewProjectionMatrix = _viewMatrix * _projectionMatrix;
		buildView();
	}

	void FPSCamera::move(float unit) {
		_position += _target * unit;
		buildView();
	}

	void FPSCamera::strafe(float unit) {
		_position += _right * unit;
		buildView();
	}

	void FPSCamera::up(float unit) {
		v3 tmp = _up * unit;
		_position = _position + tmp;
		buildView();
	}

	void FPSCamera::setPitch(float angle) {		

		matrix R = mat_Rotation(_right, angle);
		_up = mat_TransformNormal(_up, R);
		_target = mat_TransformNormal(_target, R);
		buildView();
	}

	void FPSCamera::resetYaw(float angle) {

		_yaw = angle;
		matrix R = mat_RotationZ(_yaw);
		_right = R * v3(1, 0, 0);
		_target = R * v3(0, 0, 1);
		buildView();
	}

	void FPSCamera::setYaw(float angle) {
		_yaw += angle;
		matrix R = mat_RotationZ(_yaw);
		_right = R * v3(1, 0, 0);
		_target = R * v3(0, 0, 1);
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
	void FPSCamera::resetPitch(float angle) {
		_pitch = angle;
		matrix R = mat_RotationY(_pitch);
		_right = R * v3(1, 0, 0);
		_target = R * v3(0, 0, 1);
		buildView();
	}

	void FPSCamera::update(float elapsedTime) {
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

	void FPSCamera::buildView() {

		v3 R = _right;
		v3 U = _up;
		v3 L = _target;
		v3 P = _position;

		// Keep camera's axes orthogonal to each other and of unit length.
		L = normalize(L);
		U = normalize(cross(L, R));

		// U, L already ortho-normal, so no need to normalize cross product.
		R = cross(U, L);

		// Fill in the view matrix entries.
		float x = -dot(P, R);
		float y = -dot(P, U);
		float z = -dot(P, L);

		_right = R;
		_up = U;
		_target = L;

		_viewMatrix[0, 0] = _right.x;
		_viewMatrix[0, 1] = _right.y;
		_viewMatrix[0, 2] = _right.z;
		_viewMatrix[0, 3] = x;

		_viewMatrix[1, 0] = _up.x;
		_viewMatrix[1, 1] = _up.y;
		_viewMatrix[1, 2] = _up.z;
		_viewMatrix[1, 3] = y;

		_viewMatrix[2, 0] = _target.x;
		_viewMatrix[2, 1] = _target.y;
		_viewMatrix[2, 2] = _target.z;
		_viewMatrix[2, 3] = z;

		_viewMatrix[3, 0] = 0.0f;
		_viewMatrix[3, 1] = 0.0f;
		_viewMatrix[3, 2] = 0.0f;
		_viewMatrix[3, 3] = 1.0f;


		_viewMatrix = {
			_right.x, _up.x, _target.x, 0,
			_right.y, _up.y, _target.y, 0,
			_right.z, _up.z, _target.z, 0,
			x, y, z, 1
		};
		//_viewMatrix = FPSViewRH(_position, _pitch, _yaw);
		_viewProjectionMatrix = _viewMatrix * _projectionMatrix;
	}
