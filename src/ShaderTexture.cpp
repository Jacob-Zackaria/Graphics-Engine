#include "ShaderTexture.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>
#include "DirectXTex.h"


ShaderTexture::ShaderTexture(ID3D11Device* device, ID3D11DeviceContext* devcon)
	: ShaderBase(device, devcon, L"../Assets/Shaders/Texture.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr;

	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WorldData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWorld);
	assert(SUCCEEDED(hr));
}

ShaderTexture::~ShaderTexture()
{
	ReleaseAndDeleteCOMobject(mpBuffWorld);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
}

void ShaderTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderTexture::SendWorldColor(const Matrix& world, const Vect& col)
{
	WorldData wc;
	wc.World = world;
	wc.color = col;

	this->GetContext()->UpdateSubresource(mpBuffWorld, 0, nullptr, &wc, 0, 0);
}


void ShaderTexture::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	this->GetContext()->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	this->GetContext()->VSSetConstantBuffers(1, 1, &mpBuffWorld);

	this->GetContext()->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	this->GetContext()->PSSetConstantBuffers(1, 1, &mpBuffWorld);
}


