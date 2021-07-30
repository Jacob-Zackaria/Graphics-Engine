#include "GraphicObjectTexture.h"
#include "Model.h"
#include <assert.h>

GraphicObjectTexture::GraphicObjectTexture(ShaderTexture* shader,  Model* mod, Texture* pNewTex)
{
	SetModel(mod );
	pShader = shader;
	this->mTex = pNewTex;
	World = Matrix(IDENTITY);
	Color = Vect(1, 1, 1);
}

GraphicObjectTexture::~GraphicObjectTexture()
{

}

void GraphicObjectTexture::SetEyePos(Camera& mCam)
{
	pShader->SetToContext();
	pShader->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());
}

void GraphicObjectTexture::SetTexture(Texture* newTex)
{
	this->mTex = newTex;
}

void GraphicObjectTexture::SetWorld(const Matrix& m)
{ 
	World = m;
}

void GraphicObjectTexture::SetColor(const Vect& col)
{
	Color = col;
}


Matrix& GraphicObjectTexture::GetWorld()
{
	return (this->World);
}


void GraphicObjectTexture::Render()
{
	this->mTex->SetToContext(pShader->GetContext());
	pShader->SendWorldColor(World, Color);
	pModel->SetToContext(pShader->GetContext());
	pModel->Render(pShader->GetContext());
}