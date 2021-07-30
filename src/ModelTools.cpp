#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 0, 0, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 0, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 0, 0, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	//*
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 0, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 0, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	//*/
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 0, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 0, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 0, 0, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 0, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 0, 0, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 0, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 0, 0, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 0, 0, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 0, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 0, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 1, 0, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 1, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 1, 1, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	//*
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 1, 0, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 1, 1, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 1, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	//*/
	/*
	// used to demo texture address modes
	pVerts[vind].set(0.5f, 0.5f, -0.5f, -3, -3);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 3, -3);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 3, 3);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, -3, 3);
	//*/
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 1, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 1, 0, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 1, 1, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 1, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 1, 1, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxSixFaceTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	// back face
	pVerts[0].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.376f, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[1].set(-0.5f, +0.5f, -0.5f, 0.501f, 0.376f, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[2].set(+0.5f, +0.5f, -0.5f, 0.501f, 0.627f, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pVerts[3].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.627f, 0.0f, 0.0f, -1.0f, Colors::Aqua);
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// front face
	pVerts[4].set(-0.5f, -0.5f, +0.5f, 0.0f, 0.376f, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[5].set(-0.5f, +0.5f, +0.5f, 0.2509f, 0.376f, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[6].set(+0.5f, +0.5f, +0.5f, 0.2509f, 0.627f, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, 0.0f, 0.627f, 0.0f, 0.0f, 1.0f, Colors::Aqua);
	pTriList[2].set(4, 6, 5);
	pTriList[3].set(4, 7, 6);

	// right face
	pVerts[8].set(-0.5f, -0.5f, -0.5f, 0.501f, 0.125f, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[9].set(-0.5f, +0.5f, -0.5f, 0.501f, 0.376f, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[10].set(-0.5f, -0.5f, +0.5f, 0.2509f, 0.125f, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[11].set(-0.5f, +0.5f, +0.5f, 0.2509f, 0.376f, -1.0f, 0.0f, 0.0f, Colors::Aqua);
	pTriList[4].set(10, 11, 9);
	pTriList[5].set(10, 9, 8);

	// left face
	pVerts[12].set(+0.5f, +0.5f, -0.5f, 0.501f, 0.627f, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[13].set(+0.5f, -0.5f, -0.5f, 0.501f, 0.87f, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[14].set(+0.5f, +0.5f, +0.5f, 0.2509f, 0.627f, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pVerts[15].set(+0.5f, -0.5f, +0.5f, 0.2509f, 0.87f, 1.0f, 0.0f, 0.0f, Colors::Aqua);
	pTriList[6].set(13, 12, 14);
	pTriList[7].set(13, 14, 15);

	// top face
	pVerts[16].set(-0.5f, +0.5f, -0.5f, 0.501f, 0.376f, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[17].set(+0.5f, +0.5f, -0.5f, 0.501f, 0.627f, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[18].set(-0.5f, +0.5f, +0.5f, 0.2509f, 0.376f, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[19].set(+0.5f, +0.5f, +0.5f, 0.2509f, 0.627f, 0.0f, 1.0f, 0.0f, Colors::Aqua);
	pTriList[8].set(16, 18, 19);
	pTriList[9].set(16, 19, 17);

	// bottom face
	pVerts[20].set(-0.5f, -0.5f, -0.5f, 0.75f, 0.376f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[21].set(+0.5f, -0.5f, -0.5f, 0.75f, 0.627f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[22].set(-0.5f, -0.5f, +0.5f, 1.0f, 0.376f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[23].set(+0.5f, -0.5f, +0.5f, 1.0f, 0.627f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pTriList[10].set(22, 20, 21);
	pTriList[11].set(22, 21, 23);

}

void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 13;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Colors::White);
	pVerts[1].set(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Lime);
	pVerts[2].set(+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Yellow);
	pVerts[3].set(+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Red);
	pVerts[4].set(-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Blue);

	pVerts[5].set(-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, Colors::Blue);
	pVerts[6].set(+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, Colors::Red);

	pVerts[7].set(+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, Colors::Yellow);
	pVerts[8].set(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, Colors::Lime);

	pVerts[9].set(+1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, Colors::Red);
	pVerts[10].set(+1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, Colors::Yellow);

	pVerts[11].set(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, Colors::Lime);
	pVerts[12].set(-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, Colors::Blue);

	// front face
	pTriList[0].set(0, 5, 6);

	//back face
	pTriList[1].set(0, 7, 8);

	// left face
	pTriList[2].set(0, 9, 10);

	// right face
	pTriList[3].set(0, 11, 12);

	// bottom face
	pTriList[4].set(1, 2, 3);
	pTriList[5].set(1, 3, 4);
}

void ModelTools::CreateUnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 13;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	//top vertex.
	pVerts[0].set(0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, Colors::Aqua);

	// front face
	pVerts[1].set(+1.0f, -1.0f, +1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, Colors::Aqua);
	pVerts[2].set(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, Colors::Aqua);
	pTriList[0].set(0, 2, 1);

	// back face
	pVerts[3].set(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, Colors::Aqua);
	pVerts[4].set(+1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, Colors::Aqua);
	pTriList[1].set(0, 4, 3);

	//left face
	pVerts[5].set(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[6].set(+1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, Colors::Aqua);
	pTriList[2].set(0, 6, 5);

	// right face
	pVerts[7].set(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f, Colors::Aqua);
	pVerts[8].set(-1.0f, -1.0f, +1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, Colors::Aqua);
	pTriList[3].set(0, 7, 8);

	// bottom face
	pVerts[9].set(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[10].set(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[11].set(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pVerts[12].set(-1.0f, -1.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Aqua);
	pTriList[4].set(9, 10, 11);
	pTriList[5].set(9, 11, 12);

}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	// top + bottom + other vertices in middle.
	nverts = 2 + (hslice * (vslice - 1));
	pVerts = new StandardVertex[nverts];

	// hslice faces on top and bottom with faces from middle.
	ntri = (2 * hslice) * (1 + (vslice - 2));
	pTriList = new TriangleByIndex[ntri];

	//------------ VERTEX CALCULATION -------------//
	// vertex count.
	int vertVal = 0;

	// texturing with angle value. { One texture covering whole sphere, weird on poles }
	//pVerts[vertVal++].set(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, Colors::Beige);

	//  Using texturing with normal { One texture covering 2 sides of sphere. }
	//   tu = asinf(normal[x]) / MATH_PI + 0.5f;
	//	 tv = asinf(normal[y]) / MATH_PI + 0.5f;
	//       OR { faster alternmative } 
	//	 tu = normal[x] / 2.0f + 0.5f;
	//   tv = normal[y] / 2.0f + 0.5f;

	// top vertex. 
	pVerts[vertVal++].set(0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, Colors::Beige);

	// phi angle according to number of vertical stacks (vertical angle from 180).
	float phiCount = MATH_PI / vslice;
	// theta angle according to number of horizontal slices (horizontal angle from 360).
	float thetaCount = (2.0f * MATH_PI) / hslice;

	// loop other vertices in a latitude except top and bottom.
	for (int i = 1; i <= (vslice - 1); i++)
	{
		// each vertical angle.
		float phi = i * phiCount;

		// loop through each vertex in a longitude.
		for (int j = 1; j <= hslice; j++)
		{
			// each horizontal angle.
			float theta = j * thetaCount;

			// Position of vertex.
			Vect loc = Vect(
				sinf(phi) * cosf(theta),
				cosf(phi),
				sinf(phi) * sinf(theta)
			);

			// normal is normalized position.
			Vect normal = loc.norm();

			// set vertex. // using texturing with normals.
			pVerts[vertVal++].set(loc[x], loc[y], loc[z],
				(normal[x] / 2.0f) + 0.5f,
				(normal[y] / 2.0f) + 0.5f,		// Texturing with angle value.-->//theta / (2.0f * MATH_PI),
				normal[x], normal[y], normal[z],								 //phi / MATH_PI,
				Colors::Beige
			);
		}
	}

	// bottom vertex.
	pVerts[vertVal++].set(0.0f, -1.0f, 0.0f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, Colors::Beige);

	// texturing with angle value.
	//pVerts[vertVal++].set(0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, Colors::Beige);

	//------------ INDEX CALCULATION -------------//

	// index count.
	int triVal = 0;

	// top first triangle.
	pTriList[triVal++].set(1, hslice, 0);

	// rest top triangles.
	for (int i = 1; i < hslice; i++)
	{
		pTriList[triVal++].set(i + 1, i, 0);
	}

	// middle triangle strips.
	for (int i = 0; i < (vslice - 2); i++)
	{
		// middle first triangle strip
		pTriList[triVal++].set((i + 2) * hslice, (i + 1) * hslice, 1 + (i * hslice));
		pTriList[triVal++].set(1 + (i * hslice), ((i + 1) * hslice) + 1, (i + 2) * hslice);

		// rest middle triangle strips.
		for (int j = 1; j < hslice; j++)
		{
			int t = j + (i * hslice);
			pTriList[triVal++].set(t + hslice, t, t + 1);
			pTriList[triVal++].set(t + 1, (t + hslice) + 1, t + hslice);
		}
	}

	int t = vertVal - 1;
	// lower first triangle.
	pTriList[triVal++].set(t - hslice, t, t - 1);

	//rest lower triangles.
	for (int i = 1; i < hslice; i++)
	{
		pTriList[triVal++].set(t - hslice + i, t, t - hslice + (i - 1));
	}
}