#include "GraphicObjectColor.h"
#include "Model.h"
#include <assert.h>

GraphicObjectColor::GraphicObjectColor(ShaderColor* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	Color = Vect(1, 1, 1);
	World = Matrix(IDENTITY);
}

GraphicObjectColor::~GraphicObjectColor()
{

}

void GraphicObjectColor::SetColor(const Vect& col)
{
	Color = col;
}

void GraphicObjectColor::SetWorld(const Matrix& m)
{
	World = m;
}

Matrix& GraphicObjectColor::GetWorld()
{
	return (this->World);
}


void GraphicObjectColor::SetEyePos(Camera& mCam)
{
	pShader->SetToContext();
	pShader->SendCamMatrices(mCam.getViewMatrix(), mCam.getProjMatrix());

	this->pShader->SetEyePos(mCam);
}

void GraphicObjectColor::Render()
{
	pShader->SendWorldColor(World, Color);
	pModel->SetToContext(pShader->GetContext());
	pModel->Render(pShader->GetContext());
}