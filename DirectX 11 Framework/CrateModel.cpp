#include "CrateModel.h"

CrateModel::CrateModel(float x, float y, float z)
{
	SetPosition(x, y, z);

	Vertex vertices[] =
	{
		// Front Face
		Vertex(POSITION(-1.0f, -1.0f, -1.0f), TEXCOORD(0.0f, 1.0f)),
		Vertex(POSITION(-1.0f,  1.0f, -1.0f), TEXCOORD(0.0f, 0.0f)),
		Vertex(POSITION(1.0f,  1.0f, -1.0f), TEXCOORD(1.0f, 0.0f)),
		Vertex(POSITION(1.0f, -1.0f, -1.0f), TEXCOORD(1.0f, 1.0f)),

		// Back Face
		Vertex(POSITION(-1.0f, -1.0f, 1.0f), TEXCOORD(1.0f, 1.0f)),
		Vertex(POSITION(1.0f, -1.0f, 1.0f), TEXCOORD(0.0f, 1.0f)),
		Vertex(POSITION(1.0f,  1.0f, 1.0f), TEXCOORD(0.0f, 0.0f)),
		Vertex(POSITION(-1.0f,  1.0f, 1.0f), TEXCOORD(1.0f, 0.0f)),

		// Top Face
		Vertex(POSITION(-1.0f, 1.0f, -1.0f), TEXCOORD(0.0f, 1.0f)),
		Vertex(POSITION(-1.0f, 1.0f,  1.0f), TEXCOORD(0.0f, 0.0f)),
		Vertex(POSITION(1.0f, 1.0f,  1.0f), TEXCOORD(1.0f, 0.0f)),
		Vertex(POSITION(1.0f, 1.0f, -1.0f), TEXCOORD(1.0f, 1.0f)),

		// Bottom Face
		Vertex(POSITION(-1.0f, -1.0f, -1.0f), TEXCOORD(1.0f, 1.0f)),
		Vertex(POSITION(1.0f, -1.0f, -1.0f), TEXCOORD(0.0f, 1.0f)),
		Vertex(POSITION(1.0f, -1.0f,  1.0f), TEXCOORD(0.0f, 0.0f)),
		Vertex(POSITION(-1.0f, -1.0f,  1.0f), TEXCOORD(1.0f, 0.0f)),

		// Left Face
		Vertex(POSITION(-1.0f, -1.0f,  1.0f), TEXCOORD(0.0f, 1.0f)),
		Vertex(POSITION(-1.0f,  1.0f,  1.0f), TEXCOORD(0.0f, 0.0f)),
		Vertex(POSITION(-1.0f,  1.0f, -1.0f), TEXCOORD(1.0f, 0.0f)),
		Vertex(POSITION(-1.0f, -1.0f, -1.0f), TEXCOORD(1.0f, 1.0f)),

		// Right Face
		Vertex(POSITION(1.0f, -1.0f, -1.0f), TEXCOORD(0.0f, 1.0f)),
		Vertex(POSITION(1.0f,  1.0f, -1.0f), TEXCOORD(0.0f, 0.0f)),
		Vertex(POSITION(1.0f,  1.0f,  1.0f), TEXCOORD(1.0f, 0.0f)),
		Vertex(POSITION(1.0f, -1.0f,  1.0f), TEXCOORD(1.0f, 1.0f)),
	};

	SetVertices(vertices, ARRAYSIZE(vertices));

	WORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	SetIndices(indices, ARRAYSIZE(indices));

	SetTexture(L"crate.jpg");
}
