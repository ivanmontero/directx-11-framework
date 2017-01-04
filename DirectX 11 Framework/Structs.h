#pragma once
#include <DirectXMath.h>

using namespace DirectX;

#define POSITION XMFLOAT3
#define TEXCOORD XMFLOAT2

struct Vertex
{
	Vertex(POSITION position, TEXCOORD texcoord)
		: position(position), texcoord(texcoord) {}

	POSITION position;
	TEXCOORD texcoord;
};