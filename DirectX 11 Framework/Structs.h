#pragma once
#include <DirectXMath.h>

using namespace DirectX;

#define POSITION XMFLOAT3
#define TEXCOORD XMFLOAT2
#define NORMAL XMFLOAT3

struct Vertex
{
	Vertex(POSITION position, TEXCOORD texcoord, NORMAL normal)
		: position(position), texcoord(texcoord), normal(normal) {}

	POSITION position;
	TEXCOORD texcoord;
	NORMAL normal;
};