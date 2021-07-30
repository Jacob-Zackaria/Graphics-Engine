// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "GraphicObjectColor.h"
#include "GraphicObjectTexture.h"
#include "GraphicObjectColorLight.h"
#include "GraphicObjectTextureLight.h"
#include "FlatPlane.h"
#include "SkyBox.h"
#include "TerrainModel.h"
#include "Mirror.h"
#include "Lerper.h"


class DXApp : public Align16
{
private:	
	// MAIN WINDOW HANDLE >>>>>
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX APP ELEMENTS >>>>>>
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations
	ID3D11BlendState* TransparentBS; // Blend state
	ID3D11RasterizerState* FrontFaceAsCCWRS; // Rasterizer State

	// GAME APP ELEMENTS >>>>>>
	GameTimer mTimer;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	// GAME FUNCTIONS >>>>>>
	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();
	void ActivateTransparency();
	void DeactivateTransparency();

	// DEBUG >>>>>
	void ReportLiveDXObjects();
	
	//// MODELS >>>>>>
	TerrainModel* pTerrainModel;
	FlatPlane* pWaterModel;
	FlatPlane* pWater2Model;
	SkyBox* pSkyModel;
	SkyBox* pSky2Model;

	// MIRROR >>>>>>
	Mirror* pMirror;

	// SHADERS >>>>>>
	ShaderTextureLight* pTerrainShaderTexLight;
	ShaderTextureLight* pWaterShaderTexLight;
	ShaderTextureLight* pWater2ShaderTexLight;

	// GRAPHIC OBJECTS >>>>>>
	GraphicObjectTexture* pSkyGraphTex;
	GraphicObjectTexture* pSky2GraphTex;
	GraphicObjectColorLight* pBoxGraphColLight;
	GraphicObjectTextureLight* pTrollGraphTexLight;
	GraphicObjectTextureLight* pTroll2GraphTexLight;
	GraphicObjectTextureLight* pSmallTrollGraphTexLight;
	GraphicObjectTextureLight* pSeagullGraphTexLight;
	GraphicObjectTextureLight* pTerrainGraphTexLight;
	GraphicObjectTextureLight* pWizardGraphTexLight;
	GraphicObjectTextureLight* pWaterGraphTexLight;
	GraphicObjectTextureLight* pWater2GraphTexLight;
	GraphicObjectTextureLight* pUFOGraphTexLight;
	GraphicObjectTextureLight* pTreeGraphTexLight;
	GraphicObjectTextureLight* pTree2GraphTexLight;
	GraphicObjectTextureLight* pTree3GraphTexLight;
	GraphicObjectTextureLight* pPlanetGraphTexLight;
	GraphicObjectTextureLight* pBoatGraphTexLight;
	GraphicObjectTextureLight* pFlowerGraphTexLight;
	GraphicObjectTextureLight* pFlower2GraphTexLight;
	GraphicObjectTextureLight* pFishGraphTexLight;

	// WORLD >>>>>>
	Matrix trollWorld;
	Matrix troll2World;
	Matrix skyWorld;
	Matrix sky2World;
	Matrix smallTrollWorld;
	Matrix seagullWorld;
	Matrix terrainWorld;
	Matrix wizardWorld;
	Matrix waterWorld;
	Matrix water2World;
	Matrix UFOWorld;
	Matrix treeWorld;
	Matrix tree2World;
	Matrix tree3World;
	Matrix planetWorld;
	Matrix mirrorWorld;
	Matrix boatWorld;
	Matrix boxWorld;
	Matrix flowerWorld;
	Matrix flower2World;
	Matrix fishWorld;

	// CAMERA >>>>>>
	Camera mCam;	

public:
	DXApp(HWND hwnd);
	virtual ~DXApp();
	
	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);


};

#endif _DXApp