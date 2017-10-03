#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include <Windows.h>
#include <d3dcompiler.h>
//#include <DxErr.h>
#include "Renderer.h"
#include "Window.h"
#include "Mesh.h"
#include "Camera.h"
#include "Game.h"
#include <comdef.h>

int							Renderer::BackBufferWidth			= 0;
int							Renderer::BackBufferHeight			= 0;
bool						Renderer::UsingWireFrame			= false;
UINT						Renderer::CurrentIndexCount			= 0;
XMMATRIX					Renderer::CurrentWorldMatrix		= XMMatrixIdentity();
XMMATRIX					Renderer::Projection				= XMMatrixIdentity();
IDXGISwapChain*				Renderer::SwapChain					= nullptr;
ID3D11Device*				Renderer::Device					= nullptr;
ID3D11DeviceContext*		Renderer::ImmediateContext			= nullptr;
ID3D11Texture2D*			Renderer::RenderTarget				= nullptr;
ID3D11RenderTargetView*		Renderer::RenderTargetView			= nullptr;
ID3D11Texture2D*			Renderer::DepthStencil				= nullptr;
ID3D11DepthStencilView*		Renderer::DepthStencilView			= nullptr;
ID3D11RasterizerState*		Renderer::WireFrame					= nullptr;
ID3DBlob*					Renderer::PixelBlob					= nullptr;
ID3DBlob*					Renderer::VertexBlob				= nullptr;
ID3D11InputLayout*			Renderer::InputLayout				= nullptr;
ID3D11VertexShader*			Renderer::VertexShader				= nullptr;
ID3D11PixelShader*			Renderer::PixelShader				= nullptr;
ID3D11Buffer*				Renderer::VertexConstantBuffer		= nullptr;
ID3D11Buffer*				Renderer::CurrentVertexBuffer		= nullptr;
ID3D11Buffer*				Renderer::CurrentIndexBuffer		= nullptr;
ID3D11ShaderResourceView*	Renderer::CurrentTexture			= nullptr;
ID3D11SamplerState*			Renderer::SamplerState				= nullptr;

D3D11_INPUT_ELEMENT_DESC	Renderer::InputLayoutDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


void Renderer::Initialize(int width, int height)
{
	BackBufferWidth = width;
	BackBufferHeight = height;

	DXGI_SWAP_CHAIN_DESC scdesc;
	ZeroMemory(&scdesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	scdesc.BufferCount = 1;
	scdesc.BufferDesc.Width = BackBufferWidth;
	scdesc.BufferDesc.Height = BackBufferHeight;
	scdesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//scdesc.BufferDesc.RefreshRate.Numerator = 60;
	//scdesc.BufferDesc.RefreshRate.Denominator = 1;
	scdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scdesc.OutputWindow = Window::Handle;
	scdesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scdesc.SampleDesc.Count = 1;
	scdesc.SampleDesc.Quality = 0;
	scdesc.Windowed = true;
	const D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;

	HR(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		&fl, 1, D3D11_SDK_VERSION, &scdesc, &SwapChain, &Device, nullptr, &ImmediateContext));

	CreateBuffers();

	ImmediateContext->VSSetShader(VertexShader, nullptr, 0);
	ImmediateContext->PSSetShader(PixelShader, nullptr, 0);

	UINT numElements = ARRAYSIZE(InputLayoutDesc);

	HR(Device->CreateInputLayout(InputLayoutDesc, numElements, VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), &InputLayout));

	D3D11_SAMPLER_DESC smpdesc;
	ZeroMemory(&smpdesc, sizeof(smpdesc));
	smpdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpdesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpdesc.MinLOD = 0;
	smpdesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(Device->CreateSamplerState(&smpdesc, &SamplerState));

	ImmediateContext->IASetInputLayout(InputLayout);
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;
	HR(Device->CreateRasterizerState(&wfdesc, &WireFrame));

	/*
	constant buffers
	*/
	D3D11_BUFFER_DESC vcbdesc;
	vcbdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vcbdesc.ByteWidth = sizeof(VertexPerModel);
	vcbdesc.CPUAccessFlags = 0;
	vcbdesc.MiscFlags = 0;
	vcbdesc.StructureByteStride = 0;
	vcbdesc.Usage = D3D11_USAGE_DEFAULT;
	HR(Device->CreateBuffer(&vcbdesc, NULL, &VertexConstantBuffer));

	// put pixel const. buff here

	Projection = XMMatrixPerspectiveFovLH((float) XM_PI / 3, float(BackBufferWidth) / float(BackBufferHeight), 0.001f, 1000);
}

void Renderer::CreateBuffers()
{
	//Get backbuffer and create view
	HR(SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTarget));

	HR(Device->CreateRenderTargetView(RenderTarget, nullptr, &RenderTargetView));

	// Create depth-stencil buffer and a view of it and set depth-stenciling options
	D3D11_TEXTURE2D_DESC dsdesc;
	ZeroMemory(&dsdesc, sizeof(D3D11_TEXTURE2D_DESC));
	dsdesc.ArraySize = 1;
	dsdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsdesc.CPUAccessFlags = 0;
	dsdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsdesc.Height = BackBufferHeight;
	dsdesc.Width = BackBufferWidth;
	dsdesc.MipLevels = 1;
	dsdesc.MiscFlags = 0;
	dsdesc.SampleDesc.Count = 1;
	dsdesc.SampleDesc.Quality = 0;
	dsdesc.Usage = D3D11_USAGE_DEFAULT;
	HR(Device->CreateTexture2D(&dsdesc, nullptr , &DepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc;
	ZeroMemory(&dsvdesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvdesc.Texture2D.MipSlice = 0;
	HR(Device->CreateDepthStencilView(DepthStencil, &dsvdesc, &DepthStencilView));

	ImmediateContext->OMSetRenderTargets(1, &RenderTargetView, DepthStencilView);

	// Setup viewport
	D3D11_VIEWPORT vp;
	vp.Height = (float) BackBufferHeight;
	vp.Width = (float) BackBufferWidth;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	ImmediateContext->RSSetViewports(1, &vp);

	// Load shaders
	HR(LoadVertexShader(L"Vertex.cso", &VertexShader, &VertexBlob));

	HR(LoadPixelShader(L"Pixel.cso", &PixelShader, &PixelBlob));
}

void Renderer::Dispose()
{
	DisposeBuffers();
	if (VertexShader) VertexShader->Release();
	if (PixelShader) PixelShader->Release();
	if (VertexBlob) VertexBlob->Release();
	if (PixelBlob) PixelBlob->Release();
	if (InputLayout) InputLayout->Release();
	if (VertexConstantBuffer) VertexConstantBuffer->Release();
	if (SamplerState) SamplerState->Release();
	if (WireFrame) WireFrame->Release();
	if (ImmediateContext) {
		ImmediateContext->Release();
		ImmediateContext = nullptr;
	}
	if (Device) {
		Device->Release();
		Device = nullptr;
	}
	if (SwapChain) {
		SwapChain->Release();
		SwapChain = nullptr;
	}
}

void Renderer::Resize(int backBufferWidth, int backBufferHeight)
{
	DisposeBuffers();
	BackBufferWidth = backBufferWidth;
	BackBufferHeight = backBufferHeight;
	HR(SwapChain->ResizeBuffers(0, BackBufferWidth, BackBufferHeight, DXGI_FORMAT_UNKNOWN, 0));

	CreateBuffers();

	Projection = XMMatrixPerspectiveFovLH((float)XM_PI / 3, float(BackBufferWidth) / float(BackBufferHeight), 0.001f, 1000);
}

HRESULT Renderer::LoadVertexShader(WCHAR* filename, ID3D11VertexShader** vertexShader, ID3DBlob** rawBlob)
{
	ID3DBlob* rawData = nullptr;
	HR(D3DReadFileToBlob(filename, &rawData));

	HR(Device->CreateVertexShader(rawData->GetBufferPointer(), rawData->GetBufferSize(), nullptr, vertexShader));
	*rawBlob = rawData;

	return S_OK;
}

HRESULT Renderer::LoadPixelShader(WCHAR* filename, ID3D11PixelShader** pixelShader, ID3DBlob** rawBlob)
{
	ID3DBlob* rawData = nullptr;
	HR(D3DReadFileToBlob(filename, &rawData));

	HR(Device->CreatePixelShader(rawData->GetBufferPointer(), rawData->GetBufferSize(), nullptr, pixelShader));
	*rawBlob = rawData;

	return S_OK;
}

void Renderer::DisposeBuffers() {
	if(ImmediateContext) ImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	if (RenderTarget) {
		RenderTarget->Release();
		RenderTarget = nullptr;
	}
	if (RenderTargetView) {
		RenderTargetView->Release();
		RenderTargetView = nullptr;
	}
	if (DepthStencilView) {
		DepthStencilView->Release();
		DepthStencilView = nullptr;
	}
	if (DepthStencil) {
		DepthStencil->Release();
		DepthStencil = nullptr;
	}
}

void Renderer::Clear(const float color[4])
{
	ImmediateContext->ClearRenderTargetView(RenderTargetView, color);
	ImmediateContext->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::SetVertexBuffer(ID3D11Buffer* vertexBuffer)
{
	CurrentVertexBuffer = vertexBuffer;
}

void Renderer::SetIndexBuffer(ID3D11Buffer* indexBuffer, UINT indexCount)
{
	CurrentIndexBuffer = indexBuffer;
	CurrentIndexCount = indexCount;
}

void __vectorcall Renderer::SetWorldMatrix(FXMMATRIX worldMatrix)
{
	CurrentWorldMatrix = worldMatrix;
}

void Renderer::SetTexture(ID3D11ShaderResourceView* texture)
{
	CurrentTexture = texture;
}

void Renderer::SetWireFrame(bool isWireframe)
{
	UsingWireFrame = isWireframe;
}

/*
Buffers must be set first before calling this method.
*/
void Renderer::Render()
{
	Camera::Update();

	VertexPerModel vcs;
	vcs.World = XMMatrixTranspose(CurrentWorldMatrix);
	vcs.View = XMMatrixTranspose(Camera::GetViewMatrix());
	vcs.Projection = XMMatrixTranspose(Projection);
	ImmediateContext->UpdateSubresource(VertexConstantBuffer, 0, nullptr, &vcs, 0, 0);
	ImmediateContext->VSSetConstantBuffers(0, 1, &VertexConstantBuffer);

	UINT vstride = sizeof(Vertex);
	UINT voffset = 0;
	ImmediateContext->IASetVertexBuffers(0, 1, &CurrentVertexBuffer, &vstride, &voffset);

	ImmediateContext->IASetIndexBuffer(CurrentIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	ImmediateContext->PSSetShaderResources(0, 1, &CurrentTexture);
	ImmediateContext->PSSetSamplers(0, 1, &SamplerState);

	if (!UsingWireFrame)
		ImmediateContext->RSSetState(nullptr);
	else
		ImmediateContext->RSSetState(WireFrame);

	ImmediateContext->DrawIndexed(CurrentIndexCount, 0, 0);
}

void Renderer::RenderMesh(Mesh* mesh)
{
	SetVertexBuffer(mesh->GetVertexBuffer());
	SetIndexBuffer(mesh->GetIndexBuffer(), mesh->GetIndexCount());
	SetWorldMatrix(mesh->GetWorldMatrix());
	SetTexture(mesh->GetTexture());
	Render();
}

void Renderer::Present()
{
	SwapChain->Present(0, 0);
}

ID3D11Buffer* Renderer::CreateVertexBuffer(Vertex vertices[], UINT vertexCount)
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof( Vertex ) * vertexCount;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	HR(Device->CreateBuffer(&bd, &InitData, &buffer));

	return buffer;
}

ID3D11Buffer* Renderer::CreateIndexBuffer(WORD indices[], UINT indexCount)
{
	ID3D11Buffer* buffer = nullptr;

	D3D11_BUFFER_DESC bd;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = sizeof( WORD ) * indexCount;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
	HR(Device->CreateBuffer(&bd, &InitData, &buffer));

	return buffer;
}

ID3D11ShaderResourceView* Renderer::CreateTexture(LPCWSTR filepath)
{
	ID3D11ShaderResourceView* texture = nullptr;
	//HR(D3DX11CreateShaderResourceViewFromFile(Device, filepath, nullptr, nullptr, &texture, nullptr));
	//TODO: Find a way to create
	return texture;
}

bool Renderer::IsInitialized()
{
	return Device != nullptr && ImmediateContext != nullptr;
}
