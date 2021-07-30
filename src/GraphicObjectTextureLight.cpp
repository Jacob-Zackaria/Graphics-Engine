#include "GraphicObjectTextureLight.h"
#include "Model.h"
#include <assert.h>

GraphicObjectTextureLight::GraphicObjectTextureLight(ShaderTextureLight* shader,  Model* mod, Texture* pNewTex)
{
	SetModel(mod );
	pShader = shader;
	this->mTex = pNewTex;
	World = Matrix(IDENTITY);
	Color = Vect(1, 1, 1);
}

GraphicObjectTextureLight::~GraphicObjectTextureLight()
{

}

void GraphicObjectTextureLight::UpdateAllLightPositions(const Matrix& m)
{
	this->pShader->UpdateAllLightPositions(m);
}

void GraphicObjectTextureLight::SetTexture(Texture* pNewTex)
{
	this->mTex = pNewTex;
}

void GraphicObjectTextureLight::SetWorld(const Matrix& m)
{ 
	World = m;
}

void GraphicObjectTextureLight::SetColor(const Vect& col)
{
	Color = col;
}

Matrix& GraphicObjectTextureLight::GetWorld()
{
	return (this->World);
}

void GraphicObjectTextureLight::SetEyePos(Camera& mCam)
{
	pShader->SetToContext();
	pShader->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	mCam.getPos(this->mEyePos);
}

void GraphicObjectTextureLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp)
{
	mMaterial = { amb, dif, sp };
}

void GraphicObjectTextureLight::Render()
{
	this->mTex->SetToContext(pShader->GetContext());
	pShader->SendLightParameters(mEyePos);
	pShader->SendWorldAndMaterial(World, mMaterial.Ambient, mMaterial.Diffuse, mMaterial.Specular, Color);
	pModel->SetToContext(pShader->GetContext());
	pModel->Render(pShader->GetContext());
}