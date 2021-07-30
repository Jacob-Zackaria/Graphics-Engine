#include "GraphicObjectBase.h"
#include "Model.h"
#include <assert.h>


GraphicObjectBase::GraphicObjectBase()
{
	pModel = nullptr;
}


void GraphicObjectBase::SetModel(Model* mod)
{
	pModel = mod;
}


