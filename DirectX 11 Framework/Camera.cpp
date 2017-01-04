#include "Camera.h"

XMFLOAT3	Camera::Position	= { 0.0f, 0.0f, 0.0f };
XMFLOAT3	Camera::Rotation	= { 0.0f, 0.0f, 0.0f };
XMMATRIX	Camera::view		= XMMatrixIdentity();

void Camera::SetPosition(float x, float y, float z) 
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	Rotation.x = x;
	Rotation.y = y;
	Rotation.z = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return Position;
}

XMFLOAT3 Camera::GetRotation()
{
	return Rotation;
}

void Camera::Update()
{
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR pos = XMVectorSet(Position.x, Position.y, Position.z, 0.0f);
	XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	float pitch = Rotation.x * 0.0174532925f; // To radians
	float yaw = Rotation.y * 0.0174532925f; 
	float roll = Rotation.z * 0.0174532925f;

	// Create rotation matrix
	XMMATRIX rot = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAt = XMVector3TransformCoord(lookAt, rot);
	up = XMVector3TransformCoord(up, rot);

	lookAt = pos + lookAt;

	view = XMMatrixLookAtLH(pos, lookAt, up);
}

XMMATRIX Camera::GetViewMatrix()
{
	return view;
}
