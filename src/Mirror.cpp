#include "Mirror.h"

Mirror::~Mirror()
{
	delete this->pPlane;

	ReleaseAndDeleteCOMobject(MarkMirrorDSS);
	ReleaseAndDeleteCOMobject(DrawReflectionDSS);
	ReleaseAndDeleteCOMobject(MirrorFrontFaceAsClockWiseRS);
	ReleaseAndDeleteCOMobject(NoWriteToRenderTargetBS);
}

const Matrix& Mirror::GetReflectionMatrix() const
{
	return this->reflectionMatrix;
}

const Matrix& Mirror::GetMirrorMatrix() const
{
	return (this->mirrorMatrix);
}

void Mirror::SetMirrorMatrix(const Matrix newMat)
{
	this->mirrorMatrix = newMat;

	//>>> Create Reflection matrix for mirror plane.
	// Uses the mirror's normal N and position
	Vect N = this->mirrorMatrix.get(ROW_1); // Mirror normal
	Vect p = this->mirrorMatrix.get(ROW_3); // point on mirror
	float d = -p.dot(N);
	Matrix NTN(N[x] * N, N[y] * N, N[z] * N, d * N);  // Set the 4 rows
	NTN[m3] = 0;	// Zero out the 1 that's defaulted in the w position
	NTN[m7] = 0;
	NTN[m11] = 0;
	this->reflectionMatrix = Matrix(IDENTITY) - 2 * NTN;
	this->reflectionMatrix[m15] = 1; // correcting the corner entry
}

const Vect& Mirror::GetColor() const
{
	return (this->color);
}

void Mirror::SetColor(const Vect newColor)
{
	this->color = newColor;
}

void Mirror::FirstRenderMirror(Matrix& viewMatrix, Matrix& projMatrix)
{
	//>>>>> STEP 2: Marking the mirror pixels
	//*
	// BLEND STATE: Stop writing to the render target (but keep depth writing to depth buffer)
	pMirrShader->GetContext()->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);
	// STENCIL: Set up the stencil for marking ('1' for all pixels that passed the test. See comment at line 35)
	pMirrShader->GetContext()->OMSetDepthStencilState(MarkMirrorDSS, 1);

	// Render the mirror 
	pMirrShader->SetToContext();
	pMirrShader->SendCamMatrices(viewMatrix, projMatrix);

	pMirrShader->SendWorldColor(mirrorMatrix, color);
	this->GetModel()->SetToContext(pMirrShader->GetContext());
	this->GetModel()->Render(pMirrShader->GetContext());

	// STENCIL: stop using the stencil
	pMirrShader->GetContext()->OMSetDepthStencilState(0, 0);
	// BLEND STATE: Return the blend state to normal (writing to render target)
	pMirrShader->GetContext()->OMSetBlendState(0, nullptr, 0xffffffff);


	//>>>> STEP 3:  Render objects (to render target) that should be reflected in the mirror

	// WINDINGS: face winding will appear inside out after reflection. Switching to CW front facing
	pMirrShader->GetContext()->RSSetState(MirrorFrontFaceAsClockWiseRS);
	// STENCIL: Use the stencil test (reference value 1) and only pass the test if the stencil already had a one present
	pMirrShader->GetContext()->OMSetDepthStencilState(DrawReflectionDSS, 1);
}

void Mirror::FinalRenderMirror(Matrix& viewMatrix, Matrix& projMatrix)
{
	// STENCIL: Stop using the stencil
	pMirrShader->GetContext()->OMSetDepthStencilState(0, 0);
	// WINDING: back to normal windings
	pMirrShader->GetContext()->RSSetState(FrontFaceAsCCWRS);

	//>>>>>>> STEP 4: Render the mirror again, using alpha blending to 'see' the reflection 'through' it

	// BLENDING: Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	pMirrShader->GetContext()->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);

	// Render the mirror 
	pMirrShader->SetToContext();
	pMirrShader->SendCamMatrices(viewMatrix, projMatrix);

	color = Vect(0, .5, 1, .3f); // CAUTION!  You _must_ have alpha < 1 to have transparency
	pMirrShader->SendWorldColor(mirrorMatrix, color);
	this->GetModel()->SetToContext(pMirrShader->GetContext());
	this->GetModel()->Render(pMirrShader->GetContext());

	// BLENDING: Back to normal blending (turn off transparent blending)
	pMirrShader->GetContext()->OMSetBlendState(0, nullptr, 0xffffffff);
}

Model* Mirror::GetModel()
{
	return this->pPlane->GetModel();
}

Mirror::Mirror(ID3D11Device* dev, float len,  ShaderColor* pNewMirrShader, ID3D11BlendState* newTransparentBS, ID3D11RasterizerState* newFrontFaceAsCCWRS)
	:
	TransparentBS(newTransparentBS),
	FrontFaceAsCCWRS(newFrontFaceAsCCWRS),
	mirrorMatrix(Matrix(IDENTITY)),
	reflectionMatrix(Matrix(IDENTITY)),
	pMirrShader(pNewMirrShader),
	color(Vect())
{
	this->pPlane = new FlatPlane(dev, len, 1, 1);

	// System
	HRESULT hr = 0;
	D3D11_RASTERIZER_DESC rd = {  };
	rd.FillMode = D3D11_FILL_SOLID;   // D3D11_FILL_WIREFRAME
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = false;
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;
	rd.AntialiasedLineEnable = false;

	hr = dev->CreateRasterizerState(&rd, &MirrorFrontFaceAsClockWiseRS);
	assert(SUCCEEDED(hr));

	D3D11_BLEND_DESC bd = { };
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = false;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0;

	hr = dev->CreateBlendState(&bd, &NoWriteToRenderTargetBS);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	ZeroMemory(&mirrorDesc, sizeof(mirrorDesc));
	mirrorDesc.DepthEnable = true;								// Use the depth test
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	// but don't write to the depth buffer
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;							// Use the stencil test
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// When passing the test (Always) replace
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;	// the color value with the reference value (int 1)


	// Back faces are culled, so these don't matter 
	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = dev->CreateDepthStencilState(&mirrorDesc, &MarkMirrorDSS);
	assert(SUCCEEDED(hr));

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	ZeroMemory(&drawReflectionDesc, sizeof(drawReflectionDesc));
	drawReflectionDesc.DepthEnable = true;								// Use the depth test
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// normally
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;							// Use the stencil test
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;		// When passing the stencil comparison test,
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// do not modify the stencil

	// Back faces are culled, so these don't matter
	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	hr = dev->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDSS);
	assert(SUCCEEDED(hr));
}
