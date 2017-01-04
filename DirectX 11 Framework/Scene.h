#pragma once

class Scene
{
public:
	virtual void Update(float) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};
