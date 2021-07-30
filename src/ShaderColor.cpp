#include "ShaderColor.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>


ShaderColor::ShaderColor(ID3D11Device* device, ID3D11DeviceContext* devcon)
	: ShaderBase(device, devcon, L"../Assets/Shaders/ColorSelected3D.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr;

	// View Projection buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_WorldColor);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordColor);
	assert(SUCCEEDED(hr));

	// Fog buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Fog);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferFog);
	assert(SUCCEEDED(hr));

	this->newFog = {};

}

ShaderColor::~ShaderColor()
{
	ReleaseAndDeleteCOMobject(mpBufferFog);
	ReleaseAndDeleteCOMobject(mpBuffWordColor);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
}

void ShaderColor::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderColor::SendWorldColor(const Matrix& world, const Vect& col)
{
	Data_WorldColor wc;
	wc.World = world;
	wc.WorlInv = world.getInv();
	wc.Color = col;

	this->GetContext()->UpdateSubresource(mpBuffWordColor, 0, nullptr, &wc, 0, 0);

	// send fog Data.
	this->GetContext()->UpdateSubresource(mpBufferFog, 0, nullptr, &this->newFog, 0, 0);
}

void ShaderColor::AddFogParameters(const float fogStart, const float fogRange, const Vect fogColor)
{
	this->newFog.fogActive = 1;
	this->newFog.fogStart = fogStart;
	this->newFog.fogRange = fogRange;
	this->newFog.fogColor = fogColor;
}

void ShaderColor::SetEyePos(const Camera& mCam)
{
	mCam.getPos(this->newFog.EyePosWorld);
}

void ShaderColor::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	this->GetContext()->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	this->GetContext()->VSSetConstantBuffers(1, 1, &mpBuffWordColor);
	this->GetContext()->VSSetConstantBuffers(2, 1, &mpBufferFog);

	this->GetContext()->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	this->GetContext()->PSSetConstantBuffers(1, 1, &mpBuffWordColor);
	this->GetContext()->PSSetConstantBuffers(2, 1, &mpBufferFog);
}


