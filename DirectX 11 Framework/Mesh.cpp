#include "Mesh.h"
#include "Renderer.h"

bool Mesh::SetVertices(Vertex vertices[], UINT vertexCount)
{
	//Vertices = vertices;
	VertexBuffer = Renderer::CreateVertexBuffer(vertices, vertexCount);
	if(!VertexBuffer)
	{
		return false;
	}
	return true;
}

bool Mesh::SetIndices(WORD indices[], UINT indexCount)
{
	//Indices = indices;
	IndexCount = indexCount;
	IndexBuffer = Renderer::CreateIndexBuffer(indices, indexCount);
	if (!IndexBuffer)
	{
		return false;
	}
	return true;
}

bool Mesh::SetTexture(LPCWSTR filepath)
{
	Texture = Renderer::CreateTexture(filepath);
	if(Texture)
	{
		return false;
	}
	return true;
}

FXMMATRIX Mesh::GetWorldMatrix() // FXMMATRIX to preserve XMMATRIX
{
	World = XMMatrixIdentity();

	World = XMMatrixMultiply(World, XMMatrixScaling(Scale.x, Scale.y, Scale.z));

	float pitch = XMConvertToRadians(Rotation.x);
	float yaw = XMConvertToRadians(Rotation.y);
	float roll = XMConvertToRadians(Rotation.z);
	World = XMMatrixMultiply(World, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));

	World = XMMatrixMultiply(World, XMMatrixTranslation(Position.x, Position.y, Position.z));

	return World;
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
	return VertexBuffer;
}

ID3D11Buffer* Mesh::GetIndexBuffer()
{
	return IndexBuffer;
}

ID3D11ShaderResourceView* Mesh::GetTexture()
{
	return Texture;
}

UINT Mesh::GetIndexCount()
{
	return IndexCount;
}

void Mesh::SetScale(float x, float y, float z)
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
}

// In degrees
void Mesh::SetRotation(float x, float y, float z)
{
	Rotation.x = x;
	Rotation.y = y;
	Rotation.z = z;
}

void Mesh::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

XMFLOAT3 Mesh::GetScale()
{
	return Scale;
}

XMFLOAT3 Mesh::GetRotation()
{
	return Rotation;
}

XMFLOAT3 Mesh::GetPosition()
{
	return Position;
}

void Mesh::Release()
{
	if (VertexBuffer) VertexBuffer->Release();
	if (IndexBuffer) IndexBuffer->Release();
	if (Texture) Texture->Release();
}

void* Mesh::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void Mesh::operator delete(void * p)
{
	_mm_free(p);
}
