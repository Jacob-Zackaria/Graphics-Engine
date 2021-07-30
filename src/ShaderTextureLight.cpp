#include "ShaderTextureLight.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>

ShaderTextureLight::ShaderTextureLight(ID3D11Device* dev, ID3D11DeviceContext* devcon)
	: DirLightData(nullptr), PointLightData(nullptr), SpotLightData(nullptr), ShaderBase(dev, devcon, L"../Assets/Shaders/ColorTextureLight.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr = S_OK;

	// View Projection buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// light param
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_LightParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightParams);
	assert(SUCCEEDED(hr));

	// World and Material buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_WorldAndMaterial);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordAndMaterial);
	assert(SUCCEEDED(hr));

	// LightCount buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LightCount);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightCount);
	assert(SUCCEEDED(hr));

	this->lightCount = { 0 };

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

ShaderTextureLight::~ShaderTextureLight()
{
	DirectionalLight* pDLink = DirLightData;

	while (pDLink != nullptr)
	{
		DirectionalLight* pTmp = pDLink;
		pDLink = (DirectionalLight*)pDLink->next;
		this->privRemove((LightType*)pTmp, (LightType*&)DirLightData);
		delete pTmp;
	}

	PointLight* pPLink = PointLightData;

	while (pPLink != nullptr)
	{
		PointLight* pTmp = pPLink;
		pPLink = (PointLight*)pPLink->next;
		this->privRemove((LightType*)pTmp, (LightType*&)PointLightData);
		delete pTmp;
	}

	SpotLight* pSLink = SpotLightData;

	while (pSLink != nullptr)
	{
		SpotLight* pTmp = pSLink;
		pSLink = (SpotLight*)pSLink->next;
		this->privRemove((LightType*)pTmp, (LightType*&)SpotLightData);
		delete pTmp;
	}

	ReleaseAndDeleteCOMobject(mpBufferFog);
	ReleaseAndDeleteCOMobject(mpBufferLightCount);
	ReleaseAndDeleteCOMobject(mpBuffWordAndMaterial);
	ReleaseAndDeleteCOMobject(mpBufferLightParams);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
}

void ShaderTextureLight::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}



void ShaderTextureLight::SendLightParameters(const Vect& eyepos)
{
	Data_LightParams dl;

	DirectionalLight* dirPar = DirLightData;
	for (unsigned int i = 0; i < lightCount.num_dir_light; i++)
	{
		dl.DirLight[i] = *dirPar;
		dirPar = (DirectionalLight*)dirPar->next;
	}

	PointLight* pointPar = PointLightData;
	for (unsigned int i = 0; i < lightCount.num_point_light; i++)
	{
		dl.PntLight[i] = *pointPar;
		pointPar = (PointLight*)pointPar->next;
	}

	SpotLight* spotPar = SpotLightData;
	for (unsigned int i = 0; i < lightCount.num_spot_light; i++)
	{
		dl.SptLight[i] = *spotPar;
		spotPar = (SpotLight*)spotPar->next;
	}

	dl.EyePosWorld = eyepos;

	this->GetContext()->UpdateSubresource(mpBufferLightParams, 0, nullptr, &dl, 0, 0);

	// send light count.
	this->GetContext()->UpdateSubresource(mpBufferLightCount, 0, nullptr, &this->lightCount, 0, 0);

	// send fog Data.
	this->GetContext()->UpdateSubresource(mpBufferFog, 0, nullptr, &this->newFog, 0, 0);
}


void ShaderTextureLight::SendWorldAndMaterial(const Matrix& world, const Vect& amb, const Vect& dif, const Vect& sp, const Vect& col)
{
	Data_WorldAndMaterial wm;
	wm.World = world;
	wm.WorlInv = world.getInv();
	wm.Mat.Ambient = amb;
	wm.Mat.Diffuse = dif;
	wm.Mat.Specular = sp;
	wm.color = col;

	this->GetContext()->UpdateSubresource(mpBuffWordAndMaterial, 0, nullptr, &wm, 0, 0);
}

void ShaderTextureLight::SetToContext()
{
	ShaderBase::SetToContext_VS_PS_InputLayout();

	this->GetContext()->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	this->GetContext()->VSSetConstantBuffers(1, 1, &mpBufferLightParams);
	this->GetContext()->VSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
	this->GetContext()->VSSetConstantBuffers(3, 1, &mpBufferLightCount);
	this->GetContext()->VSSetConstantBuffers(4, 1, &mpBufferFog);

	this->GetContext()->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	this->GetContext()->PSSetConstantBuffers(1, 1, &mpBufferLightParams);
	this->GetContext()->PSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
	this->GetContext()->PSSetConstantBuffers(3, 1, &mpBufferLightCount);
	this->GetContext()->PSSetConstantBuffers(4, 1, &mpBufferFog);
}

void ShaderTextureLight::AddFogParameters(const float fogStart, const float fogRange, const Vect fogColor)
{
	this->newFog.fogActive = 1;
	this->newFog.fogStart = fogStart;
	this->newFog.fogRange = fogRange;
	this->newFog.fogColor = fogColor;
}

void ShaderTextureLight::UpdateAllLightPositions(const Matrix& m)
{
	DirectionalLight* pDLink = DirLightData;

	while (pDLink != nullptr)
	{
		Vect temp = pDLink->GetDirection();
		temp[W] = 0.0f;
		pDLink->SetDirection(temp * m);
		pDLink = (DirectionalLight*)pDLink->next;
	}

	PointLight* pPLink = PointLightData;

	while (pPLink != nullptr)
	{
		pPLink->SetPosition(pPLink->GetPosition() * m);
		pPLink = (PointLight*)pPLink->next;
	}

	SpotLight* pSLink = SpotLightData;

	while (pSLink != nullptr)
	{
		pSLink->SetPosition(pSLink->GetPosition() * m);
		Vect temp = pSLink->GetDirection();
		temp[W] = 0.0f;
		pSLink->SetDirection(temp * m);
		pSLink = (SpotLight*)pSLink->next;
	}
}

void ShaderTextureLight::AddDirectionalLight(DirectionalLight::Name newName, DirectionalLight& newLight)
{
	newLight.SetName(newName);

	// Create new light.
	DirectionalLight* pNewDirLight = new DirectionalLight();
	// shallow copy. (since no pointers are in member variables.)
	*pNewDirLight = newLight;

	// add to linked list.
	this->privAddToFront((LightType*)pNewDirLight, (LightType*&)DirLightData);

	// update directional light count.
	this->lightCount.num_dir_light++;
}

void ShaderTextureLight::AddPointLight(PointLight::Name newName, PointLight& newLight)
{
	newLight.SetName(newName);

	// Create new light.
	PointLight* pNewPointLight = new PointLight();
	// shallow copy. (since no pointers are in member variables.)
	*pNewPointLight = newLight;

	// add to linked list.
	this->privAddToFront((LightType*)pNewPointLight, (LightType*&)PointLightData);

	// update point light count.
	this->lightCount.num_point_light++;
}

void ShaderTextureLight::AddSpotLight(SpotLight::Name newName, SpotLight& newLight)
{
	newLight.SetName(newName);

	// Create new light.
	SpotLight* pNewSpotLight = new SpotLight();
	// shallow copy. (since no pointers are in member variables.)
	*pNewSpotLight = newLight;

	// add to linked list.
	this->privAddToFront((LightType*)pNewSpotLight, (LightType*&)SpotLightData);

	// update spot light count.
	this->lightCount.num_spot_light++;
}

SpotLight* ShaderTextureLight::FindSpotLight(SpotLight::Name newName)
{
	SpotLight* pSLink = SpotLightData;

	while (pSLink != nullptr)
	{
		if (pSLink->GetName() == newName)
		{
			return (pSLink);
		}
		pSLink = (SpotLight*)pSLink->next;
	}

	return nullptr;
}

PointLight* ShaderTextureLight::FindPointLight(PointLight::Name newName)
{
	PointLight* pSLink = PointLightData;

	while (pSLink != nullptr)
	{
		if (pSLink->GetName() == newName)
		{
			return (pSLink);
		}
		pSLink = (PointLight*)pSLink->next;
	}

	return nullptr;
}

DirectionalLight* ShaderTextureLight::FindDirectionalLight(DirectionalLight::Name newName)
{
	DirectionalLight* pSLink = DirLightData;

	while (pSLink != nullptr)
	{
		if (pSLink->GetName() == newName)
		{
			return (pSLink);
		}
		pSLink = (DirectionalLight*)pSLink->next;
	}

	return nullptr;
}

void ShaderTextureLight::privAddToFront(LightType* node, LightType*& head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->next = nullptr;
		node->prev = nullptr;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void ShaderTextureLight::privRemove(LightType* pNode, LightType*& poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (LightType*)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}