#include "Model.h"
#include "Renderer.h"

bool Model::SetVertices(Vertex vertices[], UINT vertexCount)
{
	//Vertices = vertices;
	VertexBuffer = Renderer::CreateVertexBuffer(vertices, vertexCount);
	if(!VertexBuffer)
	{
		return false;
	}
	return true;
}

bool Model::SetIndices(WORD indices[], UINT indexCount)
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

bool Model::SetTexture(LPCWSTR filepath)
{
	Texture = Renderer::CreateTexture(filepath);
	if(Texture)
	{
		return false;
	}
	return true;
}

FXMMATRIX Model::GetWorldMatrix() // FXMMATRIX to preserve XMMATRIX
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

ID3D11Buffer* Model::GetVertexBuffer()
{
	return VertexBuffer;
}

ID3D11Buffer* Model::GetIndexBuffer()
{
	return IndexBuffer;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return Texture;
}

UINT Model::GetIndexCount()
{
	return IndexCount;
}

void Model::SetScale(float x, float y, float z)
{
	Scale.x = x;
	Scale.y = y;
	Scale.z = z;
}

// In degrees
void Model::SetRotation(float x, float y, float z)
{
	Rotation.x = x;
	Rotation.y = y;
	Rotation.z = z;
}

void Model::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

XMFLOAT3 Model::GetScale()
{
	return Scale;
}

XMFLOAT3 Model::GetRotation()
{
	return Rotation;
}

XMFLOAT3 Model::GetPosition()
{
	return Position;
}

void Model::Release()
{
	if (VertexBuffer) VertexBuffer->Release();
	if (IndexBuffer) IndexBuffer->Release();
	if (Texture) Texture->Release();
}

void* Model::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void Model::operator delete(void * p)
{
	_mm_free(p);
}
