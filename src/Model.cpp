#include <assert.h>
#include "File.h"
#include "Model.h"
#include "ModelTools.h"
#include "d3dUtil.h"
#include "AzulFileHdr.h"

Model::Model(ID3D11Device* dev, StandardVertex *pVerts, int nverts, TriangleByIndex *ptlist, int ntri)
	:
	lodNext(nullptr),
	lodPrev(nullptr),
	lodLength(0),
	name(Model::Name::UNDEFINED),
	pNext(nullptr),
	pPrev(nullptr)
{
	assert(nverts > 0);
	assert(ntri > 0);

	mDevice = dev;

	// Copy Data
	numVerts = nverts;
	pStdVerts = new StandardVertex[numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		pStdVerts[i] = pVerts[i];
	}
	numTris = ntri;
	pTriList = new TriangleByIndex[numTris];
	for (int i = 0; i < numTris; i++)
	{
		pTriList[i] = ptlist[i];
	}

	privLoadDataToGPU();
}

Model::Model(ID3D11Device* dev, Model::PreMadeModels pm)
	:
	lodNext(nullptr),
	lodPrev(nullptr),
	lodLength(0)
{
	mDevice = dev;
	pStdVerts = nullptr;
	pTriList = nullptr;
	numVerts = 0;
	numTris = 0;

	switch (pm)
	{
	case Model::PreMadeModels::UnitBox:
		ModelTools::CreateUnitBox(pStdVerts, numVerts, pTriList, numTris);
		break;
	case  Model::PreMadeModels::UnitBoxRepeatedTexture:
		ModelTools::CreateUnitBoxRepTexture(pStdVerts, numVerts, pTriList, numTris);
		break;
	case  Model::PreMadeModels::UnitBoxSixFaceTexture:
		ModelTools::CreateUnitBoxSixFaceTexture(pStdVerts, numVerts, pTriList, numTris);
		break;
	case  Model::PreMadeModels::UnitPyramid:
		ModelTools::CreateUnitPyramid(pStdVerts, numVerts, pTriList, numTris);
		break;
	case  Model::PreMadeModels::UnitPyramidRepeatedTexture:
		ModelTools::CreateUnitPyramidRepTexture(pStdVerts, numVerts, pTriList, numTris);
		break;
	case Model::PreMadeModels::UnitSphere:
		ModelTools::CreateUnitSphere(20, 20, pStdVerts, numVerts, pTriList, numTris);
		break;
	default:
		assert(false && "Invalid option");
	}

	privLoadDataToGPU();
}

Model::Model(ID3D11Device* dev, const char* const _modelName)
	:
	lodNext(nullptr),
	lodPrev(nullptr),
	lodLength(0)
{
	mDevice = dev;
	pStdVerts = nullptr;
	pTriList = nullptr;
	numVerts = 0;
	numTris = 0;

	privLoadDataFromFile(_modelName, pStdVerts, numVerts, pTriList, numTris);
	privLoadDataToGPU();
}

Model::~Model()
{
	delete[] pTriList;
	delete[] pStdVerts;

	ReleaseAndDeleteCOMobject(mpVertexBuffer);
	ReleaseAndDeleteCOMobject(mpIndexBuffer);
}

void Model::privLoadDataFromFile(const char* const _modelName, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& ptlist, int& ntri)
{
	// Read from file
	FileHandle fh;
	FileError  ferror;

	ferror = File::open(fh, _modelName, FILE_READ);
	assert(ferror == FileError::FILE_SUCCESS);

	// Get the file format for the data
	AzulFileHdr  azulFileHdr;

	// **** Overly complicated detour Part 1:  The vertex data in the file is packed efficiently
	// whereas our StandardVertex is not. Something I shall fix form the start next time.
	// For time time: we will read in the data into a temp array and then manually copy the data into
	// the StandardVertex array. 

	struct VertexStride_VUN
	{
		float x;    // Vert - V
		float y;
		float z;

		float u;    // UV tex coor - U
		float v;

		float txt;	// texture trickery 

		float nx;   // Norm verts - N
		float ny;
		float nz;

		float r;
		float g;
		float b;

		void set(float _x, float _y, float _z,
			float _u, float _v,
			float _nx, float _ny, float _nz,
			float _txt = 0,
			float _r = 1, float _g = 1, float _b = 1)
		{
			x = _x;
			y = _y;
			z = _z;
			u = _u;
			v = _v;
			nx = _nx;
			ny = _ny;
			nz = _nz;

			txt = _txt;
			r = _r;
			g = _g;
			b = _b;
		}

	};

	// ******************


	// Read the header
	ferror = File::read(fh, &azulFileHdr, sizeof(AzulFileHdr));
	assert(ferror == FileError::FILE_SUCCESS);

	// create the vertex buffer
	nverts = azulFileHdr.numVerts;
	VertexStride_VUN* pTmpVerts = new VertexStride_VUN[nverts];

	// load the verts
	// seek to the location
	ferror = File::seek(fh, FILE_SEEK_BEGIN, azulFileHdr.vertBufferOffset);
	assert(ferror == FileError::FILE_SUCCESS);

	// read it
	ferror = File::read(fh, pTmpVerts, nverts * sizeof(VertexStride_VUN));
	assert(ferror == FileError::FILE_SUCCESS);

	// create the triLists buffer
	ntri = azulFileHdr.numTriangles;
	ptlist = new TriangleByIndex[ntri];

	// load the triList
	// seek to the location
	ferror = File::seek(fh, FILE_SEEK_BEGIN, azulFileHdr.triangleListBufferOffset);
	assert(ferror == FileError::FILE_SUCCESS);

	// read it
	ferror = File::read(fh, ptlist, ntri * sizeof(TriangleByIndex));
	assert(ferror == FileError::FILE_SUCCESS);

	// close
	ferror = File::close(fh);
	assert(ferror == FileError::FILE_SUCCESS);

	// **** Overly complicated detour Part 2: now we copy the vertex data into our StandardVertex
	pVerts = new StandardVertex[nverts];
	StandardVertex* v;
	VertexStride_VUN* tmp;
	for (int i = 0; i < nverts; i++)
	{
		tmp = &(pTmpVerts[i]);
		v = &(pVerts[i]);
		v->set(tmp->x, tmp->y, tmp->z, tmp->u, tmp->v, tmp->nx, tmp->ny, tmp->nz, tmp->r, tmp->g, tmp->b);
	}

	delete[] pTmpVerts;

	// *************************

}

void  Model::privLoadDataToGPU()
{
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(StandardVertex) * numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = pStdVerts;
	HRESULT hr = mDevice->CreateBuffer(&bd, &InitData, &mpVertexBuffer);
	assert(SUCCEEDED(hr));

	// Index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TriangleByIndex) * numTris;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = pTriList;
	hr = mDevice->CreateBuffer(&bd, &InitData, &mpIndexBuffer);
	assert(SUCCEEDED(hr));

}

void Model::SetToContext(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(StandardVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mpVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Render(ID3D11DeviceContext* context)
{
	context->DrawIndexed(numTris * 3, 0, 0);
}

//-----links----
void Model::setLodNext(Model* newNext)
{
	this->lodNext = newNext;
}

void Model::setLodPrev(Model* newPrev)
{
	this->lodPrev = newPrev;
}

Model* Model::getLodNext() const
{
	return (this->lodNext);
}

Model* Model::getLodPrev() const
{
	return (this->lodPrev);
}

const float Model::GetLodLength() const
{
	return (this->lodLength);
}

void Model::SetLodLength(const float newLength)
{
	this->lodLength = newLength;
}

Model::Name Model::getName() const
{
	return this->name;
}

void Model::setName(Model::Name inName)
{
	this->name = inName;
}

//-----links----
void Model::setNext(Model* newNext)
{
	this->pNext = newNext;
}

void Model::setPrev(Model* newPrev)
{
	this->pPrev = newPrev;
}

Model* Model::getNext() const
{
	return (this->pNext);
}

Model* Model::getPrev() const
{
	return (this->pPrev);
}
