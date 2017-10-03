#include "FloorMesh.h"

FloorMesh::FloorMesh(float x, float y, float z)
{
	SetPosition(x, y, z);

	Vertex vertices[] = {
		Vertex(POSITION(200.0f, 0.0f, -200.0f), TEXCOORD(200.0f, 200.0f), NORMAL(0.0f, 1.0f, 0.0f)),
		Vertex(POSITION(-200.0f, 0.0f, -200.0f), TEXCOORD(0.0f, 200.0f), NORMAL(0.0f, 1.0f, 0.0f)),
		Vertex(POSITION(-200.0f, 0.0f, 200.0f), TEXCOORD(0.0f, 0.0f), NORMAL(0.0f, 1.0f, 0.0f)),
		Vertex(POSITION(200.0f, 0.0f, 200.0f), TEXCOORD(200.0f, 0.0f), NORMAL(0.0f, 1.0f, 0.0f)),
	};

	SetVertices(vertices, ARRAYSIZE(vertices));

	WORD indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	SetIndices(indices, ARRAYSIZE(indices));

	SetTexture(L"grass.jpg");
}
