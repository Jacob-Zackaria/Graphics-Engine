#include "SkyBox.h"
#include "Model.h"
#include "d3dUtil.h"

SkyBox::SkyBox(ID3D11Device* dev, float len)
{
	int nverts = 24;
	StandardVertex* pVerts = new StandardVertex[nverts];

	int ntri = 12;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	// Front Face.
	pVerts[0].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.25f, 0.335f, Colors::AntiqueWhite);
	pVerts[1].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.25f, 0.66f, Colors::Blue);
	pVerts[2].set(0.5f * len, -0.5f * len, 0.5f * len, 0.0f, 0.66f, Colors::Orchid);
	pVerts[3].set(0.5f * len, 0.5f * len, 0.5f * len, 0.0f, 0.335f, Colors::BlanchedAlmond);

	pTriList[0].set(2, 1, 0);
	pTriList[1].set(0, 3, 2);

	// Bottom Face.
	pVerts[4].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.2509f, 0.66f, Colors::Black);
	pVerts[5].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.499f, 0.66f, Colors::Blue);
	pVerts[6].set(0.5f * len, -0.5f * len, -0.5f * len, 0.499f, 1.0f, Colors::Magenta);
	pVerts[7].set(0.5f * len, -0.5f * len, 0.5f * len, 0.2509f, 1.0f, Colors::Red);

	pTriList[2].set(6, 5, 4);
	pTriList[3].set(7, 6, 4);

	// Left Face.
	pVerts[8].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.5f, 0.335f, Colors::Black);
	pVerts[9].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5f, 0.66f, Colors::Blue);
	pVerts[10].set(-0.5f * len, -0.5f * len, 0.5f * len, 0.25f, 0.66f, Colors::Magenta);
	pVerts[11].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.25f, 0.335f, Colors::Red);

	pTriList[4].set(10, 9, 8);
	pTriList[5].set(11, 10, 8);

	// Right Face.
	pVerts[12].set(0.5f * len, 0.5f * len, 0.5f * len, 1.0f, 0.335f, Colors::Black);
	pVerts[13].set(0.5f * len, -0.5f * len, 0.5f * len, 1.0f, 0.66f, Colors::Blue);
	pVerts[14].set(0.5f * len, -0.5f * len, -0.5f * len, 0.75f, 0.66f, Colors::Magenta);
	pVerts[15].set(0.5f * len, 0.5f * len, -0.5f * len, 0.75f, 0.335f, Colors::Red);

	pTriList[6].set(14, 13, 12);
	pTriList[7].set(15, 14, 12);

	// Back Face.
	pVerts[16].set(0.5f * len, 0.5f * len, -0.5f * len, 0.75f, 0.335f, Colors::Black);
	pVerts[17].set(0.5f * len, -0.5f * len, -0.5f * len, 0.75f, 0.66f, Colors::Blue);
	pVerts[18].set(-0.5f * len, -0.5f * len, -0.5f * len, 0.5f, 0.66f, Colors::Magenta);
	pVerts[19].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.5f, 0.335f, Colors::Red);

	pTriList[8].set(18, 17, 16);
	pTriList[9].set(19, 18, 16);

	// Top Face.
	pVerts[20].set(-0.5f * len, 0.5f * len, -0.5f * len, 0.499f, 0.33f, Colors::Black);
	pVerts[21].set(-0.5f * len, 0.5f * len, 0.5f * len, 0.2509f, 0.33f, Colors::Blue);
	pVerts[22].set(0.5f * len, 0.5f * len, 0.5f * len, 0.2509f, 0.0f, Colors::Magenta);
	pVerts[23].set(0.5f * len, 0.5f * len, -0.5f * len, 0.499f, 0.0f, Colors::Red);

	pTriList[10].set(22, 21, 20);
	pTriList[11].set(23, 22, 20);

	pPlane = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

SkyBox::~SkyBox()
{
	delete pPlane;
}


Model* SkyBox::GetModel()
{
	return this->pPlane;
}

void SkyBox::Render(ID3D11DeviceContext* context)
{
	pPlane->SetToContext(context);
	pPlane->Render(context);
}