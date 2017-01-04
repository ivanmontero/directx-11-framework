#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
	static XMFLOAT3 Position;
	static XMFLOAT3 Rotation;
	static XMMATRIX view;

public:
	static void SetPosition(float, float, float);
	static void SetRotation(float, float, float);

	static XMFLOAT3 GetPosition();
	static XMFLOAT3 GetRotation();

	static void Update();

	static XMMATRIX GetViewMatrix();
};