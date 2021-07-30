#include "TerrainModel.h"
#include "Model.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>

TerrainModel::~TerrainModel()
{
	delete pModTerrain;
}

TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
{
	//-------------- Load file.
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
	assert(SUCCEEDED(hr));

	//-------------- Get pixel array of texture.
	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width);

	//-------------- height of texture.
	size_t side = hgtmap->height;	// the image should be square
	size_t pixel_width = 4;			// 4 bytes RGBA per pixel
	int sideMinusOne = (int)side - 1;

	// ----------- VERTEX CALCULATION -----------------//

	int nverts = (int)(side * side);
	StandardVertex* pVerts = new StandardVertex[nverts];

	{
		int vertVal = 0;
		// rows.
		for (int i = 0; i < (int)side; i++)
		{
			// columns.
			for (int j = 0; j < (int)side; j++)
			{
				// calculate height value.
				uint8_t h_val = hgtmap->pixels[pixel_width * ((i * side) + j)];
				float mappedhVal = h_val * (maxheight / 256.0f);

				// Set vertex.
				pVerts[vertVal++].set(j * len, mappedhVal + ytrans, i * len,
					((float)(RepeatU * j)) / (float)sideMinusOne, ((float)(RepeatV * i)) / (float)sideMinusOne,
					Colors::LimeGreen);
			}
		}
	}

	// ----------- NORMAL CALCULATION -----------------//

	{
		int vertVal = 0;
		int sidePlusOne = side + 1;
		// rows.
		for (int i = 0; i < (int)side; i++)
		{
			// columns.
			for (int j = 0; j < (int)side; j++)
			{
				if (i == 0 || i == sideMinusOne || j == 0 || j == sideMinusOne)
				{
					// edge vertex. { usually blocked from players view, thus a junk value 
					//                useful for terrain with size 1x1 }
					pVerts[vertVal].setNormal(0.0f, 1.0f, 0.0f); // normal pointing up.
				}
				else
				{
					// field vertex, always associated with 6 faces.

					// face 1 normal vector.
					Vect a = pVerts[vertVal].Pos - pVerts[vertVal - 1].Pos;
					Vect b = pVerts[vertVal].Pos - pVerts[vertVal - sidePlusOne].Pos;
					Vect z = b.cross(a);

					// face 2 normal vector.
					a = pVerts[vertVal].Pos - pVerts[vertVal - sidePlusOne].Pos;
					b = pVerts[vertVal].Pos - pVerts[vertVal - side].Pos;
					Vect y = b.cross(a);

					// face 3 normal vector.
					a = pVerts[vertVal].Pos - pVerts[vertVal - side].Pos;
					b = pVerts[vertVal].Pos - pVerts[vertVal + 1].Pos;
					Vect x = b.cross(a);

					// face 4 normal vector.
					a = pVerts[vertVal].Pos - pVerts[vertVal + 1].Pos;
					b = pVerts[vertVal].Pos - pVerts[vertVal + sidePlusOne].Pos;
					Vect w = b.cross(a);

					// face 5 normal vector.
					a = pVerts[vertVal].Pos - pVerts[vertVal + sidePlusOne].Pos;
					b = pVerts[vertVal].Pos - pVerts[vertVal + side].Pos;
					Vect v = b.cross(a);

					// face 6 normal vector.
					a = pVerts[vertVal].Pos - pVerts[vertVal - 1].Pos;
					b = pVerts[vertVal].Pos - pVerts[vertVal + side].Pos;
					Vect u = b.cross(a);

					// calculate average of nearby normals.
					pVerts[vertVal].setNormal(
										(u[X] +v[X] + w[X] + x[X] + y[X] + z[X]) / 6.0f,
										(u[Y] + v[Y] + w[Y] + x[Y] + y[Y] + z[Y]) / 6.0f,
										(u[Z] + v[Z] + w[Z] + x[Z] + y[Z] + z[Z]) / 6.0f
									   );
				}

				vertVal++;
			}
		}
	}

	// ----------- INDEX CALCULATION -----------------//

	int ntri = 2 * (int)(sideMinusOne * sideMinusOne);
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	{
		int triVal = 0;
		int sideVal = 0;
		// rows.
		for (int i = 0; i < sideMinusOne; i++)
		{
			// columns.
			for (int j = 0; j < sideMinusOne; j++)
			{
				sideVal = (side * i) + j;

				// Set triangles.
				pTriList[triVal++].set(sideVal, sideVal + side, sideVal + side + 1);
				pTriList[triVal++].set(sideVal + side + 1, sideVal + 1, sideVal);
			}
		}
	}


	// Create new model.
	pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

Model* TerrainModel::GetModel()
{
	return this->pModTerrain;
}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
	pModTerrain->SetToContext(context);
	pModTerrain->Render(context);
}