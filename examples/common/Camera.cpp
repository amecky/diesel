#include "Camera.h"

FPSCamera::FPSCamera(float screenWidth, float screenHeight)  {
	ds::matPerspectiveFovLH(_projectionMatrix, ds::PI * 0.25f, screenWidth / screenHeight, 0.01f, 100.0f);
	ds::vec3(_position, 0.0f, 2.0f, -4.0f);
	ds::getMousePosition(_lastMousePos);
	ds::vec3(_target, 0, 0, 1);
	ds::vec3(_up, 0, 1, 0 );
	ds::vec3(_right, 1, 0, 0 );
	_speed = 10.0f;
	_yaw = 0.0f;
	_pitch = 0.0f;
	ds::matLookAtLH(_viewMatrix,_position, _target, _up);
	ds::setProjectionMatrix(_projectionMatrix);
	ds::matMultiply(_viewProjectionMatrix, _viewMatrix, _projectionMatrix);
	buildView();
}

FPSCamera::~FPSCamera() {

}

void FPSCamera::setPosition(const v3& pos, const v3& target) {
	ds::vec3(_position,pos());
	ds::vec3(_target,target());
	buildView();
}

void FPSCamera::setPosition(const v3& pos) {
	ds::vec3(_position,pos());		
	buildView();
}

void FPSCamera::move(float unit) {
	for (int i = 0; i < 3; ++i) {
		_position[i] += _target[i] * unit;
	}
	buildView();
}

void FPSCamera::strafe(float unit) {
	for (int i = 0; i < 3; ++i) {
		_position[i] += _right[i] * unit;
	}
	buildView();
}

void FPSCamera::up(float unit) {
	float tmp[3] = { 0.0f };
	for (int i = 0; i < 3; ++i) {
		tmp[i] = _up[i] * unit;
	}
	for (int i = 0; i < 3; ++i) {
		_position[i] = _position[i] + tmp[i];
	}
	buildView();
}

void FPSCamera::setPitch(float angle) {
	float R[16];
	ds::matRotation(R,_right,angle);
	ds::matTransformNormal(_up, _up, R);
	ds::matTransformNormal(_target, _target, R);
	buildView();
}

void FPSCamera::resetYaw(float angle) {
	_yaw = angle;
	float R[16];
	ds::matRotationZ(R, _yaw);
	float right[3] = { 1,0,0 };
	ds::matVec3Multiply(_right, R, right);
	float target[3] = { 0,0,1 };
	ds::matVec3Multiply(_target, R, target);
	buildView();
}

void FPSCamera::setYaw(float angle) {
	float R[16];
	ds::matRotationY(R, angle);
	ds::matTransformNormal(_right,_right, R);
	ds::matTransformNormal(_up, _up, R);
	ds::matTransformNormal(_target,_target, R);
	buildView();
}
	
void FPSCamera::resetPitch(float angle) {
	_pitch = angle;
	float R[16];
	ds::matRotationY(R, _pitch);
	float right[3] = { 1,0,0 };
	ds::matVec3Multiply(_right, R, right);
	float target[3] = { 0,0,1 };
	ds::matVec3Multiply(_target, R, target);
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
	float mp[2];
	ds::getMousePosition(mp);
	if (ds::isMouseButtonPressed(0)) {
		// Make each pixel correspond to a quarter of a degree.
		float dx = DEGTORAD(0.25f*(mp[0] - _lastMousePos[0]));
		float dy = DEGTORAD(0.25f*(mp[1] - _lastMousePos[1]));
		setPitch(dy);
		setYaw(-dx);			
	}
	_lastMousePos[0] = mp[0];
	_lastMousePos[1] = mp[1];
		
}

void FPSCamera::buildView() {
	v3 R = _right;
	v3 U = _up;
	v3 L = _target;
	v3 P = _position;
		
	L = normalize(L);
	U = normalize(cross(L, R));
	R = cross(U, L);

	float x = -dot(P, R);
	float y = -dot(P, U);
	float z = -dot(P, L);

	ds::vec3(_right,R());
	ds::vec3(_up, U());
	ds::vec3(_target, L());

	_viewMatrix[0] = _right[0];
	_viewMatrix[1] = _right[1];
	_viewMatrix[2] = _right[2];
	_viewMatrix[3] = x;

	_viewMatrix[4] = _up[0];
	_viewMatrix[5] = _up[1];
	_viewMatrix[6] = _up[2];
	_viewMatrix[7] = y;

	_viewMatrix[8] = _target[0];
	_viewMatrix[9] = _target[1];
	_viewMatrix[10] = _target[2];
	_viewMatrix[11] = z;

	_viewMatrix[12] = 0.0f;
	_viewMatrix[13] = 0.0f;
	_viewMatrix[14] = 0.0f;
	_viewMatrix[15] = 1.0f;

	ds::matMultiply(_viewProjectionMatrix, _viewMatrix, _projectionMatrix);

	ds::setViewMatrix(_viewMatrix);
}
