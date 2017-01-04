#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <DirectXColors.h>
#include "Structs.h"

using namespace DirectX;

struct Vertex;

// Should be inherited
class Model
{
protected:
	//Vertex* Vertices = nullptr; // points to the array
	//WORD* Indices = nullptr;  // points to the array
	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;
	ID3D11ShaderResourceView* Texture = nullptr;
	UINT IndexCount = 0;
	XMFLOAT3 Scale = { 1.0f, 1.0f, 1.0f };   // 1 = default
	XMFLOAT3 Rotation = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
	XMMATRIX World = XMMatrixIdentity();

	bool SetVertices(Vertex[], UINT);
	bool SetIndices(WORD[], UINT);
	bool SetTexture(LPCWSTR);
	//bool UpdateVertexBuffer(Vertex[]);
	//bool UpdateIndexBuffer(WORD[]);
public:
	FXMMATRIX GetWorldMatrix();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	ID3D11ShaderResourceView* GetTexture();
	UINT GetIndexCount();
	void SetScale(float, float, float);
	void SetRotation(float, float, float);
	void SetPosition(float, float, float);
	XMFLOAT3 GetScale();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetPosition();
	void Release();
	void* operator new(size_t);			// To be sure XMMATRIX is 16-byte aligned
	void operator delete(void*);
};
