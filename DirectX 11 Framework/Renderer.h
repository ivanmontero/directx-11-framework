#pragma once

#include <d3d11.h>
//#include <D3DX11tex.h>
#include <string>
#include <directxmath.h>
#include "Structs.h"

//TODO: Get SDK
//#define HR(x) { if(FAILED(x)){ MessageBoxW(NULL, DXGetErrorDescription(x), DXGetErrorString(x), MB_OK); Game::Exit(); }}
#define HR(x) { if(FAILED(x)){_com_error err(x); MessageBoxW(NULL, err.ErrorMessage(), err.ErrorMessage(), MB_OK); Game::Exit(); }}

using namespace DirectX;

class Mesh;

class Renderer
{
private:
	static int BackBufferWidth;
	static int BackBufferHeight;

	static bool UsingWireFrame;

	static XMMATRIX CurrentWorldMatrix;
	static XMMATRIX Projection;

	struct VertexPerModel
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Projection;
	};

	static D3D11_INPUT_ELEMENT_DESC InputLayoutDesc[];

	static IDXGISwapChain* SwapChain;
	static ID3D11Device* Device;
	static ID3D11DeviceContext* ImmediateContext;
	static ID3D11Texture2D* RenderTarget;
	static ID3D11RenderTargetView* RenderTargetView;
	static ID3D11Texture2D* DepthStencil;
	static ID3D11DepthStencilView* DepthStencilView;
	static ID3D11RasterizerState* WireFrame;

	static ID3DBlob* PixelBlob;
	static ID3DBlob* VertexBlob;
	static ID3D11InputLayout* InputLayout;
	static ID3D11VertexShader* VertexShader;
	static ID3D11PixelShader* PixelShader;
	static ID3D11Buffer* VertexConstantBuffer;
	static ID3D11Buffer* CurrentVertexBuffer;
	static ID3D11Buffer* CurrentIndexBuffer;
	static ID3D11ShaderResourceView* CurrentTexture;
	static ID3D11SamplerState* SamplerState;

	static UINT CurrentIndexCount;

	static HRESULT LoadVertexShader(WCHAR*, ID3D11VertexShader**, ID3DBlob**);
	static HRESULT LoadPixelShader(WCHAR*, ID3D11PixelShader**, ID3DBlob**);

	static void DisposeBuffers();
public:
	static void Initialize(int, int);
	static void CreateBuffers();
	static void Dispose();
	static void Resize(int, int);
	static void Clear(const float[4]);
	static void SetVertexBuffer(ID3D11Buffer*);
	static void SetIndexBuffer(ID3D11Buffer*, UINT);
	static void __vectorcall SetWorldMatrix(FXMMATRIX worldMatrix);
	static void SetTexture(ID3D11ShaderResourceView*);
	static void SetWireFrame(bool);

	static void Render();
	static void RenderMesh(Mesh*);
	static void Present();

	static ID3D11Buffer* CreateVertexBuffer(Vertex[], UINT);
	static ID3D11Buffer* CreateIndexBuffer(WORD[], UINT);
	static ID3D11ShaderResourceView* CreateTexture(LPCWSTR);

	static bool IsInitialized();
};
