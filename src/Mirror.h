#ifndef MIRROR_H
#define MIRROR_H

#include "Matrix.h"
#include "FlatPlane.h"
#include "ShaderColor.h"
#include "d3dUtil.h"
#include "Model.h"
#include "ShaderColor.h"

struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;

class Mirror
{
private:
	FlatPlane* pPlane;
	Matrix mirrorMatrix;
	Matrix reflectionMatrix;
	ShaderColor* pMirrShader;
	Vect color;

	// MIRROR >>>>>>
	// From DX
	ID3D11BlendState* TransparentBS;
	ID3D11RasterizerState* FrontFaceAsCCWRS;

	// Mirror
	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;	
	ID3D11RasterizerState* MirrorFrontFaceAsClockWiseRS;
	ID3D11BlendState* NoWriteToRenderTargetBS;

public:
	Mirror(const Mirror&) = delete;				// Copy constructor
	Mirror(Mirror&&) = delete;						// Move constructor
	Mirror& operator=(const Mirror&) & = delete;  // Copy assignment operator
	Mirror& operator=(Mirror&&) & = delete;       // Move assignment operator
	~Mirror();

	const Matrix& GetReflectionMatrix() const;

	const Matrix& GetMirrorMatrix() const;
	void SetMirrorMatrix(const Matrix newMat);

	const Vect& GetColor() const;
	void SetColor(const Vect newColor);

	// Render Mirror
	void FirstRenderMirror(Matrix& viewMatrix, Matrix& projMatrix);
	void FinalRenderMirror(Matrix& viewMatrix, Matrix& projMatrix);

	Model* GetModel();
	Mirror(ID3D11Device* dev, float len, ShaderColor* pNewMirrShader, ID3D11BlendState* newTransparentBS, ID3D11RasterizerState* newFrontFaceAsCCWRS);
};

#endif MIRROR_H