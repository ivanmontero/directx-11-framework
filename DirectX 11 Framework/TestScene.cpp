#include <directxcolors.h>
#include <cmath>
#include "TestScene.h"
#include "Models.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"

TestScene::TestScene()
{
	Crate = new CrateModel(0.0f, -1.0f, 0.0f);
	Floor = new FloorModel(0.0f, -2.0f, 0.0f);
	Camera::SetPosition(0.0f, 0.0f, -10.0f);
}

void TestScene::Update(float delta)
{

	//***************CAMERA ROTATION CODE***************//

	float dRotY = Mouse::GetDX() * 0.02f;
	float dRotX = Mouse::GetDY() * 0.02f;
	XMFLOAT3 camRot = Camera::GetRotation();
	float rotX = camRot.x + dRotX;
	if (rotX > 90) rotX = 90;
	if (rotX < -90) rotX = -90;
	Camera::SetRotation(rotX, camRot.y + dRotY, camRot.z);

	//**************************************************//

	//***************CAMERA MOVEMENT CODE***************//

	float frontBackSpeed = 0.0f;
	if (Keyboard::IsKeyDown(DIK_S))
	{
		frontBackSpeed = -3.5f;
	}
	if (Keyboard::IsKeyDown(DIK_W))
	{
		frontBackSpeed = 3.5f;
	}
	float rightLeftSpeed = 0.0f;
	if (Keyboard::IsKeyDown(DIK_A))
	{
		rightLeftSpeed = -3.5f;
	}
	if (Keyboard::IsKeyDown(DIK_D))
	{
		rightLeftSpeed = 3.5f;
	}
	if (Keyboard::IsKeyDown(DIK_LSHIFT))
	{
		rightLeftSpeed *= 2.5f;
		frontBackSpeed *= 2.5f;
	}

	bool isCrouch = false;
	if (Keyboard::IsKeyDown(DIK_LCONTROL))
	{
		isCrouch = true;
	}

	XMFLOAT3 camPos = Camera::GetPosition();
	double yAxisRotRad = XMConvertToRadians(Camera::GetRotation().y);
	// Forward back
	camPos.x += frontBackSpeed * (float)sin(yAxisRotRad) * delta;
	camPos.z += frontBackSpeed * (float)cos(yAxisRotRad) * delta;
	// Side
	camPos.x += rightLeftSpeed * (float)sin(yAxisRotRad + XM_PI / 2) * delta;
	camPos.z += rightLeftSpeed * (float)cos(yAxisRotRad + XM_PI / 2) * delta;

	//**************************************************//

	//***************CAMERA JUMP AND CROUCH CODE***************//

	//Jump and crouch
	currentVelY -= .005f * delta;
	camPos.y += currentVelY;

	if (isCrouch)
		camPos.y -= 1.0f;

	if(camPos.y < -.0001f && !isCrouch)
	{
		camPos.y = 0.0f;
		currentVelY = 0.0f;
	}
	if(camPos.y < -1.0001f && isCrouch)
	{
		camPos.y = -1.0f;
		currentVelY = 0.0f;
	}

	if (Keyboard::IsKeyDown(DIK_SPACE) && camPos.y < .0001f)
	{
		currentVelY = .002f;
	}

	Camera::SetPosition(camPos.x, camPos.y, camPos.z);

	//**************************************************//

	//***************CUBE ANIMATION***************//
	time += delta;
	XMFLOAT3 cratePos = Crate->GetPosition();
	cratePos.y = -0.25f + 0.75f *(float)sin(XMConvertToRadians(fmod(time * 100.0f, 360.0f))); //+ 0.5f *(float)sin(XMConvertToRadians(time/1000.0f));
	Crate->SetPosition(cratePos.x, cratePos.y, cratePos.z);
	
	XMFLOAT3 crateRot = Crate->GetRotation();
	crateRot.y = fmod(time * 50.0f, 360.0f);
	Crate->SetRotation(crateRot.x, crateRot.y, crateRot.z);

}

void TestScene::Render()
{
	//Renderer::SetWireFrame(true);
	Renderer::Clear(Colors::Black);
	Renderer::RenderModel(Crate);
	Renderer::RenderModel(Floor);
	Renderer::Present();
}

void TestScene::Release()
{
	if (Crate) Crate->Release();
	if (Floor) Floor->Release();
}
