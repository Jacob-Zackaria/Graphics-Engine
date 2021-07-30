#include "FlatPlane.h"
#include "Model.h"
#include "d3dUtil.h"

FlatPlane::FlatPlane(ID3D11Device* dev, float len, float urep, float vrep)
{
	int nverts = 4;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 2;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	float c = len * 0.5f;
	pVerts[0].set(-c, 0.0f, -c, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Colors::Black);
	pVerts[1].set(-c, 0.0f, c, 0.0f, vrep, 0.0f, 1.0f, 0.0f, Colors::Blue);
	pVerts[2].set(c, 0.0f, c, urep, vrep, 0.0f, 1.0f, 0.0f, Colors::Magenta);
	pVerts[3].set(c, 0.0f, -c, urep, 0.0f, 0.0f, 1.0f, 0.0f, Colors::Red);

	pTriList[0].set(0, 1, 2);
	pTriList[1].set(2, 3, 0);

	pPlane = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

FlatPlane::~FlatPlane()
{
	delete pPlane;
}

Model* FlatPlane::GetModel()
{
	return this->pPlane;
}

void FlatPlane::Render(ID3D11DeviceContext* context)
{
	pPlane->SetToContext(context);
	pPlane->Render(context);
}