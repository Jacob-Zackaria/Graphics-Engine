#include "GraphicObjectColorLight.h"
#include "Model.h"
#include <assert.h>

GraphicObjectColorLight::GraphicObjectColorLight(ShaderColorLight* shader,  Model* mod)
{
	SetModel(mod );
	pShader = shader;
	World = Matrix(IDENTITY);
}

GraphicObjectColorLight::~GraphicObjectColorLight()
{

}

void GraphicObjectColorLight::UpdateAllLightPositions(const Matrix& m)
{
	this->pShader->UpdateAllLightPositions(m);
}

void GraphicObjectColorLight::SetWorld(const Matrix& m)
{ 
	World = m;
}

Matrix& GraphicObjectColorLight::GetWorld()
{
	return (this->World);
}

void GraphicObjectColorLight::SetEyePos(Camera& mCam)
{
	pShader->SetToContext();
	pShader->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	mCam.getPos(this->mEyePos);
}

void GraphicObjectColorLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp)
{
	mMaterial = { amb, dif, sp };
}

void GraphicObjectColorLight::Render()
{
	pShader->SendLightParameters(mEyePos);
	pShader->SendWorldAndMaterial(World, mMaterial.Ambient, mMaterial.Diffuse, mMaterial.Specular);
	pModel->SetToContext(pShader->GetContext());
	pModel->Render(pShader->GetContext());
}