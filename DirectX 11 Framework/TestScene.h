#pragma once
#include "Scene.h"

class CrateMesh;
class FloorMesh;

class TestScene : public Scene
{
private:
	CrateMesh* Crate = nullptr;
	FloorMesh* Floor = nullptr;
	float x = 0.0f, scaleY = 1.0f, scaleX = 1.0f, currentY = 0.0f, currentVelY = 0.0f,
		time = 0.0f;
public:
	TestScene();
	void Update(float) override;
	void Render() override;
	void Release() override;
};
