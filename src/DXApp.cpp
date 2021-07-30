#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include "d3dUtil.h"


// needed to load shaders from file
#include <d3dcompiler.h>
#include "DirectXTex.h"

#include "GodCam.h"
#include "LODManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "CamSplineControl.h"

void DXApp::InitDemo()
{
	// <<<<<<-------------   MANAGERS ------------------>>>> //
	LODManager::Create();
	TextureManager::Create();
	ShaderManager::Create();
	ModelManager::Create();
	CamSplineControl::Create();

	// <<<<<<-------------   LIGHTS ------------------>>>> //
	DirectionalLight newDir1;
	newDir1.SetDirectionalLightParameters(Vect(-1, -1, 1).getNorm(),
		.5 * Vect(1, 1, 1), .5 * Vect(1, 1, 1), Vect(1, 1, 1));

	SpotLight newSpot1;
	newSpot1.SetSpotLightParameters(Vect(600.0f, 350.0f, -300.0f), 4000, .5 * Vect(1, 0, 0),
		Vect(0.0f, 0.0f, -1.0f), 50,
		.2 * Vect(1, 1, 1), 3 * Vect(1, 1, 1), 1 * Vect(1, 1, 1, 150));

	SpotLight newSpot2;
	newSpot2.SetSpotLightParameters(Vect(-30.0f, 20.0f, -30.0f), 40, .5 * Vect(1, 0, 0),
		Vect(0.0f, -1.0f, 0.0f), 50,
		.2 * Vect(1, 1, 1), 2 * Colors::AliceBlue, 1 * Vect(1, 1, 1, 150));

	SpotLight newSpotUFO;
	newSpotUFO.SetSpotLightParameters(Vect(-30.0f, 20.0f, -30.0f), 40, .5 * Vect(1, 0, 0),
		Vect(0.0f, -1.0f, -1.0f).getNorm(), 50,
		.2 * Vect(1, 1, 1), 2 * Colors::Red, 1 * Vect(1, 1, 1, 150));

	SpotLight newSpotUFO2;
	newSpotUFO2.SetSpotLightParameters(Vect(-30.0f, 20.0f, -30.0f), 40, .5 * Vect(1, 0, 0),
		Vect(0.0f, -1.0f, -1.0f).getNorm(), 50,
		.2 * Vect(1, 1, 1), 2 * Colors::Yellow, 1 * Vect(1, 1, 1, 150));

	PointLight newPoint1;
	newPoint1.SetPointLightParameters(Vect(-0.5f, 14.5f, -61.5f), 15.0f, .2 * Vect(0, 1, 0),
		.1 * Vect(0, 1, 0), Vect(0, 1, 0), Vect(0.1, 0.1, 0.1, 150));
	

	// <<<<<<-------------   TEXTURE ------------------>>>> //	
	Texture* pSkyTexture = new Texture(md3dDevice, L"../Assets/CubeMap/sky3.tga");
	Texture* pSky2Texture = new Texture(md3dDevice, L"../Assets/CubeMap/web.tga");
	Texture* pTrollTexture = new Texture(md3dDevice, L"../Assets/Models/Troll.tga");
	Texture* pSmallTrollTexture = new Texture(md3dDevice, L"../Assets/Models/SmallTroll.tga");
	Texture* pSeagullTexture = new Texture(md3dDevice, L"../Assets/Models/Seagull.tga");
	Texture* pTerrainTexture = new Texture(md3dDevice, L"../Assets/Textures/Concrete.tga", D3D11_FILTER_ANISOTROPIC, 4, true, 0, DirectX::TEX_FILTER_LINEAR);
	Texture* pWizardTexture = new Texture(md3dDevice, L"../Assets/Models/Wizard.tga");
	Texture* pWaterTexture = new Texture(md3dDevice, L"../Assets/Textures/CartoonWater.tga");
	Texture* pWater2Texture = new Texture(md3dDevice, L"../Assets/Textures/CartoonRotate.tga");
	Texture* pUFOTexture = new Texture(md3dDevice, L"../Assets/Models/UFO.tga");
	Texture* pTreeTexture = new Texture(md3dDevice, L"../Assets/Models/OrigamiTree.tga");
	Texture* pTrollMetalTexture = new Texture(md3dDevice, L"../Assets/Models/TrollMetal.tga");
	Texture* pPlanetTexture = new Texture(md3dDevice, L"../Assets/Textures/Mars.tga");
	Texture* pBoatTexture = new Texture(md3dDevice, L"../Assets/Models/Boat.tga");
	Texture* pFlowerTexture = new Texture(md3dDevice, L"../Assets/Models/OrigamiFlower.tga");
	Texture* pFishTexture = new Texture(md3dDevice, L"../Assets/Models/OrigamiFish.tga");

	// <<<<<<-------------   TEXTURE MANAGER ------------------>>>> //
	TextureManager::Add(pTrollTexture, Texture::Name::TROLL);
	TextureManager::Add(pSkyTexture, Texture::Name::SKY);
	TextureManager::Add(pSky2Texture, Texture::Name::SKY);
	TextureManager::Add(pSmallTrollTexture, Texture::Name::TROLL);
	TextureManager::Add(pSeagullTexture, Texture::Name::SEAGULL);
	TextureManager::Add(pTerrainTexture, Texture::Name::TERRAIN);
	TextureManager::Add(pWizardTexture, Texture::Name::WIZARD);
	TextureManager::Add(pWaterTexture, Texture::Name::WATER);
	TextureManager::Add(pWater2Texture, Texture::Name::WATER);
	TextureManager::Add(pUFOTexture, Texture::Name::UFO);
	TextureManager::Add(pTreeTexture, Texture::Name::ORIGAMI_TREE);
	TextureManager::Add(pTrollMetalTexture, Texture::Name::TROLL);
	TextureManager::Add(pPlanetTexture, Texture::Name::PLANET);
	TextureManager::Add(pBoatTexture, Texture::Name::BOAT);
	TextureManager::Add(pFlowerTexture, Texture::Name::FLOWER);
	TextureManager::Add(pFishTexture, Texture::Name::FISH);
	
	// <<<<<<-------------   SHADERS ------------------>>>> //

	// Troll
	ShaderTextureLight* pTrollShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pTrollShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);
	pTrollShaderTexLight->AddPointLight(PointLight::Name::POINT_1, newPoint1);

	// Skybox
	ShaderTexture* pSkyShaderTex = new ShaderTexture(md3dDevice, md3dImmediateContext);

	// Skybox 2
	ShaderTexture* pSky2ShaderTex = new ShaderTexture(md3dDevice, md3dImmediateContext);

	// Small Troll
	ShaderTextureLight* pSmallTrollShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pSmallTrollShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Seagull
	ShaderTextureLight* pSeagullShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pSeagullShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Terrain
	pTerrainShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pTerrainShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);
	pTerrainShaderTexLight->AddPointLight(PointLight::Name::POINT_1, newPoint1);
	pTerrainShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_1, newSpot2);
	pTerrainShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_2, newSpotUFO);
	pTerrainShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_3, newSpotUFO2);
	
	// Wizard
	ShaderTextureLight* pWizardShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pWizardShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Water
	pWaterShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pWaterShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);
	pWaterShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_2, newSpotUFO);
	pWaterShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_3, newSpotUFO2);

	// Water 2
	pWater2ShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pWater2ShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);
	pWater2ShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_2, newSpotUFO);
	pWater2ShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_3, newSpotUFO2);

	// UFO
	ShaderTextureLight* pUFOShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pUFOShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Origami Tree 
	ShaderTextureLight* pTreeShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pTreeShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Planet
	ShaderTextureLight* pPlanetShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pPlanetShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_1, newSpot1);

	// Mirror
	ShaderColor* pMirrorShaderColor= new ShaderColor(md3dDevice, md3dImmediateContext);

	// Boat
	ShaderTextureLight* pBoatShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pBoatShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);
	pBoatShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_2, newSpotUFO);
	pBoatShaderTexLight->AddSpotLight(SpotLight::Name::SPOT_3, newSpotUFO2);

	// Mirrror Box
	ShaderColorLight* pBoxShaderColLight = new ShaderColorLight(md3dDevice, md3dImmediateContext);
	pBoxShaderColLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Flower
	ShaderTextureLight* pFlowerShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pFlowerShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);

	// Fish
	ShaderTextureLight* pFishShaderTexLight = new ShaderTextureLight(md3dDevice, md3dImmediateContext);
	pFishShaderTexLight->AddDirectionalLight(DirectionalLight::Name::DIR_1, newDir1);


	// <<<<<<-------------   SHADER MANAGER ------------------>>>> //
	ShaderManager::Add(pTrollShaderTexLight, "pTrollShaderTexLight");
	ShaderManager::Add(pSkyShaderTex, "pSkyShaderTex");
	ShaderManager::Add(pSky2ShaderTex, "pSky2ShaderTex");
	ShaderManager::Add(pSmallTrollShaderTexLight, "pSmallTrollShaderTexLight");
	ShaderManager::Add(pSeagullShaderTexLight, "pSeagullShaderTexLight");
	ShaderManager::Add(pTerrainShaderTexLight, "pTerrainShaderTexLight");
	ShaderManager::Add(pWizardShaderTexLight, "pWizardShaderTexLight");
	ShaderManager::Add(pWaterShaderTexLight, "pWaterShaderTexLight");
	ShaderManager::Add(pWater2ShaderTexLight, "pWater2ShaderTexLight");
	ShaderManager::Add(pUFOShaderTexLight, "pUFOShaderTexLight");
	ShaderManager::Add(pTreeShaderTexLight, "pTreeShaderTexLight");
	ShaderManager::Add(pPlanetShaderTexLight, "pPlanetShaderTexLight");
	ShaderManager::Add(pMirrorShaderColor, "pMirrorShaderColor");
	ShaderManager::Add(pBoatShaderTexLight, "pBoatShaderTexLight");
	ShaderManager::Add(pBoxShaderColLight, "pBoxShaderColLight");
	ShaderManager::Add(pFlowerShaderTexLight, "pFlowerShaderTexLight");
	ShaderManager::Add(pFishShaderTexLight, "pFishShaderTexLight");

	// <<<<<<-------------   MODELS ------------------>>>> //

	// Troll
	Model* pTrollModel0 = new Model(md3dDevice, "../Assets/Models/TrollLOD0.azul");
	Model* pTrollModel1 = new Model(md3dDevice, "../Assets/Models/TrollLOD1.azul");
	Model* pTrollModel2 = new Model(md3dDevice, "../Assets/Models/TrollLOD2.azul");

	// Skybox
	pSkyModel = new SkyBox(md3dDevice, 5000);

	// Skybox 2
	pSky2Model = new SkyBox(md3dDevice, 6000);

	// Small Troll
	Model* pSmallTrollModel0 = new Model(md3dDevice, "../Assets/Models/SmallTrollLOD0.azul");
	Model* pSmallTrollModel1 = new Model(md3dDevice, "../Assets/Models/SmallTrollLOD1.azul");
	Model* pSmallTrollModel2 = new Model(md3dDevice, "../Assets/Models/SmallTrollLOD2.azul");

	// Seagull
	Model* pSeagullModel0 = new Model(md3dDevice, "../Assets/Models/SeagullLOD0.azul");
	Model* pSeagullModel1 = new Model(md3dDevice, "../Assets/Models/SeagullLOD1.azul");
	Model* pSeagullModel2 = new Model(md3dDevice, "../Assets/Models/SeagullLOD2.azul");

	// Terrain
	pTerrainModel = new TerrainModel(md3dDevice, L"../Assets/HeightMap/HeightMap200px.tga", 0.5f, 20.0f, 0.0f, 12, 12);

	// Wizard
	Model* pWizardModel0 = new Model(md3dDevice, "../Assets/Models/WizardLOD0.azul");
	Model* pWizardModel1 = new Model(md3dDevice, "../Assets/Models/WizardLOD1.azul");
	Model* pWizardModel2 = new Model(md3dDevice, "../Assets/Models/WizardLOD2.azul");

	// Water
	pWaterModel = new FlatPlane(md3dDevice, 250.0f, 15, 15);

	// Water 2
	pWater2Model = new FlatPlane(md3dDevice, 250.0f, 15, 15);

	// UFO
	Model* pUFOModel0 = new Model(md3dDevice, "../Assets/Models/UFOLOD0.azul");
	Model* pUFOModel1 = new Model(md3dDevice, "../Assets/Models/UFOLOD1.azul");
	Model* pUFOModel2 = new Model(md3dDevice, "../Assets/Models/UFOLOD2.azul");

	// Tree
	Model* pTreeModel0 = new Model(md3dDevice, "../Assets/Models/OrigamiTreeLOD0.azul");
	Model* pTreeModel1 = new Model(md3dDevice, "../Assets/Models/OrigamiTreeLOD1.azul");
	Model* pTreeModel2 = new Model(md3dDevice, "../Assets/Models/OrigamiTreeLOD2.azul");

	// Sphere
	Model* pPlanetModel = new Model(md3dDevice, Model::PreMadeModels::UnitSphere);

	// Mirror object
	pMirror = new Mirror(md3dDevice, 5.0f, pMirrorShaderColor, TransparentBS, FrontFaceAsCCWRS);

	// Boat
	Model* pBoatModel0 = new Model(md3dDevice, "../Assets/Models/BoatLOD0.azul");
	Model* pBoatModel1 = new Model(md3dDevice, "../Assets/Models/BoatLOD1.azul");
	Model* pBoatModel2 = new Model(md3dDevice, "../Assets/Models/BoatLOD2.azul");

	// Mirror Box
	Model* pBoxModel = new Model(md3dDevice, Model::PreMadeModels::UnitBox);

	// Flower
	Model* pFlowerModel0 = new Model(md3dDevice, "../Assets/Models/OrigamiFlowerLOD0.azul");
	Model* pFlowerModel1 = new Model(md3dDevice, "../Assets/Models/OrigamiFlowerLOD1.azul");

	// Fish
	Model* pFishModel0 = new Model(md3dDevice, "../Assets/Models/OrigamiFishLOD0.azul");
	Model* pFishModel1 = new Model(md3dDevice, "../Assets/Models/OrigamiFishLOD1.azul");

	// <<<<<<-------------   MODEL MANAGER ------------------>>>> //
	ModelManager::Add(pTrollModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pTrollModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pTrollModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pSmallTrollModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pSmallTrollModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pSmallTrollModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pSeagullModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pSeagullModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pSeagullModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pWizardModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pWizardModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pWizardModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pUFOModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pUFOModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pUFOModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pTreeModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pTreeModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pTreeModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pPlanetModel, Model::Name::SPHERE);
	ModelManager::Add(pBoatModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pBoatModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pBoatModel2, Model::Name::EXTERNAL);
	ModelManager::Add(pBoxModel, Model::Name::BOX);
	ModelManager::Add(pFlowerModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pFlowerModel1, Model::Name::EXTERNAL);
	ModelManager::Add(pFishModel0, Model::Name::EXTERNAL);
	ModelManager::Add(pFishModel1, Model::Name::EXTERNAL);

	// <<<<<<-------------   GRAPHIC OBJECTS ------------------>>>> //

	// Troll
	pTrollGraphTexLight = new GraphicObjectTextureLight(pTrollShaderTexLight, pTrollModel0, pTrollTexture);

	// Troll 2
	pTroll2GraphTexLight = new GraphicObjectTextureLight(pTrollShaderTexLight, pTrollModel0, pTrollMetalTexture);

	// SkyBox
	pSkyGraphTex = new GraphicObjectTexture(pSkyShaderTex, pSkyModel->GetModel(), pSkyTexture);

	// SkyBox 2
	pSky2GraphTex = new GraphicObjectTexture(pSky2ShaderTex, pSky2Model->GetModel(), pSky2Texture);

	// Small Troll
	pSmallTrollGraphTexLight = new GraphicObjectTextureLight(pSmallTrollShaderTexLight, pSmallTrollModel0, pSmallTrollTexture);

	// Seagull
	pSeagullGraphTexLight = new GraphicObjectTextureLight(pSeagullShaderTexLight, pSeagullModel0, pSeagullTexture);

	// Terrain
	pTerrainGraphTexLight = new GraphicObjectTextureLight(pTerrainShaderTexLight, pTerrainModel->GetModel(), pTerrainTexture);

	// Wizard
	pWizardGraphTexLight = new GraphicObjectTextureLight(pWizardShaderTexLight, pWizardModel0, pWizardTexture);

	// Water
	pWaterGraphTexLight = new GraphicObjectTextureLight(pWaterShaderTexLight, pWaterModel->GetModel(), pWaterTexture);

	// Water 2
	pWater2GraphTexLight = new GraphicObjectTextureLight(pWater2ShaderTexLight, pWater2Model->GetModel(), pWater2Texture);

	// UFO
	pUFOGraphTexLight = new GraphicObjectTextureLight(pUFOShaderTexLight, pUFOModel0, pUFOTexture);

	// Tree
	pTreeGraphTexLight = new GraphicObjectTextureLight(pTreeShaderTexLight, pTreeModel0, pTreeTexture);

	// Tree 2
	pTree2GraphTexLight = new GraphicObjectTextureLight(pTreeShaderTexLight, pTreeModel0, pTreeTexture);

	// Tree 3
	pTree3GraphTexLight = new GraphicObjectTextureLight(pTreeShaderTexLight, pTreeModel0, pTreeTexture);

	// Planet
	pPlanetGraphTexLight = new GraphicObjectTextureLight(pPlanetShaderTexLight, pPlanetModel, pPlanetTexture);

	// Boat
	pBoatGraphTexLight = new GraphicObjectTextureLight(pBoatShaderTexLight, pBoatModel0, pBoatTexture);

	// Mirror Box
	pBoxGraphColLight = new GraphicObjectColorLight(pBoxShaderColLight, pBoxModel);

	// Flower
	pFlowerGraphTexLight = new GraphicObjectTextureLight(pFlowerShaderTexLight, pFlowerModel0, pFlowerTexture);

	// Flower 2
	pFlower2GraphTexLight = new GraphicObjectTextureLight(pFlowerShaderTexLight, pFlowerModel0, pFlowerTexture);

	// Fish
	pFishGraphTexLight = new GraphicObjectTextureLight(pFishShaderTexLight, pFishModel0, pFishTexture);

	
	// <<<<<<-------------   WORLD ------------------>>>> //

	// Troll
	trollWorld = Matrix(ROT_Y, -1.2) * Matrix(TRANS, -8.0f, 8.7f, -20.0f);
	pTrollGraphTexLight->SetWorld(trollWorld);

	// Troll 2
	troll2World = Matrix(SCALE, 8.0f, 8.0f, 8.0f) * Matrix(TRANS, -0.5f, 14.5f, -61.5f);
	pTroll2GraphTexLight->SetWorld(troll2World);

	// Skybox
	skyWorld = Matrix(IDENTITY);
	pSkyGraphTex->SetWorld(skyWorld);

	// Skybox 2
	sky2World = Matrix(IDENTITY);
	pSky2GraphTex->SetWorld(sky2World);

	// Small Troll
	smallTrollWorld = Matrix(SCALE, 0.4f, 0.4f, 0.4f) * Matrix(ROT_Y, 1.4f) * Matrix(TRANS, 1.5f, 8.7f, -20.0f);
	pSmallTrollGraphTexLight->SetWorld(smallTrollWorld);

	// Seagull
	seagullWorld = Matrix(SCALE, 0.4f, 0.4f, 0.4f) * Matrix(ROT_Y, -2.5f) * Matrix(TRANS, 3.0f, 8.7f, -16.5f);
	pSeagullGraphTexLight->SetWorld(seagullWorld);

	// Terrain
	terrainWorld = Matrix(TRANS, -58, 0, -75);
	pTerrainGraphTexLight->SetWorld(terrainWorld);

	// Wizard
	wizardWorld = Matrix(SCALE, 0.15f, 0.15f, 0.15f) * Matrix(ROT_Y, 0.8) *Matrix(TRANS, -30.0f, 9.0f, -30.0f);
	pWizardGraphTexLight->SetWorld(wizardWorld);

	// Water
	waterWorld = Matrix(TRANS, 0.0f, 7.5f, 0.0f);
	pWaterGraphTexLight->SetWorld(waterWorld);

	// Water 2
	water2World = Matrix(TRANS, 0.0f, 7.45f, 0.0f);
	pWater2GraphTexLight->SetWorld(water2World);

	// UFO
	UFOWorld = Matrix(SCALE, 0.015f, 0.015f, 0.015f) * Matrix(TRANS, -30.0f, 20.0f, -30.0f);
	pUFOGraphTexLight->SetWorld(UFOWorld);

	// Tree
	treeWorld = Matrix(SCALE, 0.5f, 0.5f, 0.5f) * Matrix(TRANS, -35.0f, 9.0f, -17.0f);
	pTreeGraphTexLight->SetWorld(treeWorld);

	// Tree 2
	tree2World = Matrix(SCALE, 0.5f, 0.5f, 0.5f) * Matrix(TRANS, -8.0f, 8.0f, -22.0f);
	pTree2GraphTexLight->SetWorld(tree2World);

	// Tree 3
	tree3World = Matrix(SCALE, 0.6f, 0.6f, 0.6f) * Matrix(ROT_Y, 1.4) * Matrix(TRANS, 4.0f, 8.0f, -18.0f);
	pTree3GraphTexLight->SetWorld(tree3World);

	// Planet
	planetWorld = Matrix(SCALE, 100.0f, 100.0f, 100.0f) * Matrix(TRANS, 600.0f, 350.0f, -700.0f);
	pPlanetGraphTexLight->SetWorld(planetWorld);

	// mirror
	mirrorWorld =   Matrix(ROT_X, -1.51) * Matrix(ROT_Y, 1.4) * Matrix(TRANS, 3.5f, 11.0f, -20.0f);
	pMirror->SetMirrorMatrix(mirrorWorld);

	// Boat
	boatWorld = Matrix(SCALE, 0.4f, 0.4f, 0.4f) * Matrix(ROT_Y, 1.1f) * Matrix(TRANS, -13.0f, 7.0f, -18.0f);
	pBoatGraphTexLight->SetWorld(boatWorld);

	// Mirror Box
	boxWorld = Matrix(SCALE, 5.0f, 5.0f, 0.1f) * Matrix(ROT_X, 0.06f) * Matrix(ROT_Y, 1.4f) * Matrix(TRANS, 3.6f, 11.0f, -20.0f);
	pBoxGraphColLight->SetWorld(boxWorld);

	// Flower
	flowerWorld = Matrix(SCALE, 0.1f, 0.1f, 0.1f) * Matrix(TRANS, -32.0f, 9.0f, -17.0f);
	pFlowerGraphTexLight->SetWorld(flowerWorld);

	// Flower 2
	flower2World = Matrix(SCALE, 0.1f, 0.1f, 0.1f) * Matrix(TRANS, 0.0f, 9.0f, 4.0f);
	pFlower2GraphTexLight->SetWorld(flower2World);

	// Fish
	fishWorld = Matrix(SCALE, 0.1f, 0.1f, 0.1f) * Matrix(TRANS, -12.0f, 6.5f, -15.0f);
	pFishGraphTexLight->SetWorld(fishWorld);


	// <<<<<<-------------   LOD  ------------------>>>> //

	// Troll
	LODModel* pTrollLOD = new LODModel(pTrollGraphTexLight, trollWorld);
	pTrollLOD->Add(pTrollModel2);  // low poly first with no switch distance.
	pTrollLOD->Add(pTrollModel1, 20.0f);
	pTrollLOD->Add(pTrollModel0, 10.0f); // High poly last
	LODManager::Add(pTrollLOD, LODModel::Name::TROLL);

	// Troll 2
	LODModel* pTroll2LOD = new LODModel(pTroll2GraphTexLight, troll2World);
	pTroll2LOD->Add(pTrollModel2);  // low poly first with no switch distance.
	pTroll2LOD->Add(pTrollModel1, 20.0f);
	pTroll2LOD->Add(pTrollModel0, 10.0f); // High poly last
	LODManager::Add(pTroll2LOD, LODModel::Name::TROLL);

	// Small Troll
	LODModel* pSmallTrollLOD = new LODModel(pSmallTrollGraphTexLight, smallTrollWorld);
	pSmallTrollLOD->Add(pSmallTrollModel2);  // low poly first with no switch distance.
	pSmallTrollLOD->Add(pSmallTrollModel1, 20.0f);
	pSmallTrollLOD->Add(pSmallTrollModel0, 10.0f); // High poly last
	LODManager::Add(pSmallTrollLOD, LODModel::Name::SMALL_TROLL);

	// Seagull
	LODModel* pSeagullLOD = new LODModel(pSeagullGraphTexLight, seagullWorld);
	pSeagullLOD->Add(pSeagullModel2);  // low poly first with no switch distance.
	pSeagullLOD->Add(pSeagullModel1, 20.0f);
	pSeagullLOD->Add(pSeagullModel0, 10.0f); // High poly last
	LODManager::Add(pSeagullLOD, LODModel::Name::SEAGULL);

	// Wizard
	LODModel* pWizardLOD = new LODModel(pWizardGraphTexLight, wizardWorld);
	pWizardLOD->Add(pWizardModel2);  // low poly first with no switch distance.
	pWizardLOD->Add(pWizardModel1, 20.0f);
	pWizardLOD->Add(pWizardModel0, 10.0f); // High poly last
	LODManager::Add(pWizardLOD, LODModel::Name::WIZARD);

	// UFO
	LODModel* pUFOLOD = new LODModel(pUFOGraphTexLight, UFOWorld);
	pUFOLOD->Add(pUFOModel2);  // low poly first with no switch distance.
	pUFOLOD->Add(pUFOModel1, 45.0f);
	pUFOLOD->Add(pUFOModel0, 25.0f); // High poly last
	LODManager::Add(pUFOLOD, LODModel::Name::UFO);

	// Tree
	LODModel* pTreeLOD = new LODModel(pTreeGraphTexLight, treeWorld);
	pTreeLOD->Add(pTreeModel2);  // low poly first with no switch distance.
	pTreeLOD->Add(pTreeModel1, 25.0f);
	pTreeLOD->Add(pTreeModel0, 10.0f); // High poly last
	LODManager::Add(pTreeLOD, LODModel::Name::ORIGAMI_TREE);

	// Tree 2
	LODModel* pTree2LOD = new LODModel(pTree2GraphTexLight, tree2World);
	pTree2LOD->Add(pTreeModel2);  // low poly first with no switch distance.
	pTree2LOD->Add(pTreeModel1, 25.0f);
	pTree2LOD->Add(pTreeModel0, 10.0f); // High poly last
	LODManager::Add(pTree2LOD, LODModel::Name::ORIGAMI_TREE);

	// Tree 3
	LODModel* pTree3LOD = new LODModel(pTree3GraphTexLight, tree3World);
	pTree3LOD->Add(pTreeModel2);  // low poly first with no switch distance.
	pTree3LOD->Add(pTreeModel1, 25.0f);
	pTree3LOD->Add(pTreeModel0, 10.0f); // High poly last
	LODManager::Add(pTree3LOD, LODModel::Name::ORIGAMI_TREE);

	// Boat
	LODModel* pBoatLOD = new LODModel(pBoatGraphTexLight, boatWorld);
	pBoatLOD->Add(pBoatModel2);  // low poly first with no switch distance.
	pBoatLOD->Add(pBoatModel1, 20.0f);
	pBoatLOD->Add(pBoatModel0, 10.0f); // High poly last
	LODManager::Add(pBoatLOD, LODModel::Name::BOAT);

	// Flower
	LODModel* pFlowerLOD = new LODModel(pFlowerGraphTexLight, flowerWorld);
	pFlowerLOD->Add(pFlowerModel1);  // low poly first with no switch distance.
	pFlowerLOD->Add(pFlowerModel0, 7.0f);
	LODManager::Add(pFlowerLOD, LODModel::Name::FLOWER);

	// Flower 2
	LODModel* pFlower2LOD = new LODModel(pFlower2GraphTexLight, flower2World);
	pFlower2LOD->Add(pFlowerModel1);  // low poly first with no switch distance.
	pFlower2LOD->Add(pFlowerModel0, 7.0f);
	LODManager::Add(pFlower2LOD, LODModel::Name::FLOWER);

	// Fish
	LODModel* pFishLOD = new LODModel(pFishGraphTexLight, fishWorld);
	pFishLOD->Add(pFishModel1);  // low poly first with no switch distance.
	pFishLOD->Add(pFishModel0, 7.0f);
	LODManager::Add(pFishLOD, LODModel::Name::FISH);

	// <<<<<<-------------   CAMERA ------------------>>>> //
	mCam.setPerspective(45, mClientWidth / (float)mClientHeight, 0.1f, 12000.0f);
	mCam.setOrientAndPosition(Vect(0, 1, 0), Vect(-2.0f, 15.0f, -42.0f), Vect(-30.0f, 11.0f, 4.0f));

	mTimer.Reset();
	
	//Camera Spline Points
	CamSplineControl::Add(Vect(-30.0f, 11.0f, 4.0f), Vect(-2.0f, 15.0f, -42.0f), 0.0f);
	CamSplineControl::Add(Vect(-30.0f, 15.0f, 6.0f), Vect(-2.0f, 15.0f, -42.0f), 15.0f);
	CamSplineControl::Add(Vect(-30.0f, 10.0f, 3.0f), Vect(-2.0f, 15.0f, -42.0f), 25.0f);
	CamSplineControl::Add(Vect(-35.0f, 11.0f, -18.0f), Vect(-2.0f, 15.0f, -22.0f), 45.0f);
	CamSplineControl::Add(Vect(-37.0f, 12.0f, -32.0f), Vect(-2.0f, 15.0f, -22.0f), 55.0f);
	CamSplineControl::Add(Vect(-8.0f, 10.0f, -23.0f), Vect(-2.0f, 20.0f, -42.0f), 65.0f);
	CamSplineControl::Add(Vect(-6.0f, 16.0f, -23.0f), Vect(-2.0f, 20.0f, -42.0f), 75.0f); // statue look
	CamSplineControl::Add(Vect(-5.0f, 10.0f, -21.0f), Vect(2.0f, 12.0f, -20.0f), 85.0f);
	CamSplineControl::Add(Vect(2.0f, 10.0f, -17.0f), Vect(2.0f, 10.0f, -20.0f), 95.0f);  // small troll
	CamSplineControl::Add(Vect(1.0f, 11.0f, -17.0f), Vect(4.0f, 11.0f, -17.0f), 105.0f);
	CamSplineControl::Add(Vect(0.0f, 13.0f, -12.0f), Vect(-5.0f, 13.0f, -20.0f), 115.0f);
	CamSplineControl::Add(Vect(-11.0f, 10.0f, -13.0f), Vect(-11.0f, 9.0f, -16.0f), 125.0f); // boat
	CamSplineControl::Add(Vect(-28.0f, 11.0f, -28.0f), Vect(-30.0f, 11.0f, -30.0f), 126.0f);
	CamSplineControl::Add(Vect(-28.0f, 11.0f, -28.0f), Vect(-30.0f, 11.0f, -30.0f), 128.0f);
	CamSplineControl::Add(Vect(-10.0f, 13.0f, -18.0f), Vect(-8.0f, 13.0f, -20.0f), 129.0f);
	CamSplineControl::Add(Vect(-10.0f, 13.0f, -18.0f), Vect(-8.0f, 13.0f, -20.0f), 131.0f);
	CamSplineControl::Add(Vect(-30.0f, 11.0f, 4.0f), Vect(-2.0f, 15.0f, -42.0f), 139.0f); // fin

}


void DXApp::UpdateScene()
{
	// LOD Update.
	LODManager::Update(mCam);


	// Skybox
	skyWorld *= Matrix(ROT_Y, 0.00003f);
	pSkyGraphTex->SetWorld(skyWorld);

	// Skybox 2
	sky2World *= Matrix(ROT_Y, 0.00003f);
	pSky2GraphTex->SetWorld(sky2World);

	//// Water 1
	waterWorld *= Matrix(ROT_Y, 0.00004f);
	pWaterGraphTexLight->SetWorld(waterWorld);

	// Water 2
	water2World *= Matrix(ROT_Y, 0.00004f);
	pWater2GraphTexLight->SetWorld(water2World);

	// Rate of change
	static float z = 0.0001f;

	static float trans = 1.0f;
	trans += z;
	if (trans > 1.0f)
	{
		z *= -1.0f;
	}
	else if (trans < 0.0f)
	{
		z *= -1.0f;
	}

	// Set sky and water color.
	pSkyGraphTex->SetColor(Vect(1.0f, 1.0f, 1.0f, trans));
	pWaterGraphTexLight->SetColor(Vect(1.0f, 1.0f, 1.0f, trans));
	pPlanetGraphTexLight->SetColor(Vect(1.0f, 1.0f, 1.0f, 1.0f - trans));

	// Fish to surface
	fishWorld = Matrix(SCALE, 0.1f, 0.1f, 0.1f) * Matrix(TRANS, -12.0f, 6.5f - (2.0f * trans), -15.0f);
	pFishGraphTexLight->SetWorld(fishWorld);

	// Set Terrain and Water Fog.
	pTerrainShaderTexLight->AddFogParameters(20.0f +  (50.0f * trans), 40.0f, Colors::Black);
	pWaterShaderTexLight->AddFogParameters(20.0f + (50.0f * trans), 40.0f, Colors::Black);
	pWater2ShaderTexLight->AddFogParameters(20.0f + (50.0f * trans), 40.0f, Colors::Black);


	// Light Control.
	float cosmoveTime = cosf(mTimer.TotalTime());
	float sinMoveTime = sinf(mTimer.TotalTime());
	SpotLight* pTestLight = pTerrainShaderTexLight->FindSpotLight(SpotLight::Name::SPOT_2);
	pTestLight->SetDirection(Vect(sinMoveTime, -1.0f, cosmoveTime).getNorm());

	pTestLight = pWaterShaderTexLight->FindSpotLight(SpotLight::Name::SPOT_2);
	pTestLight->SetDirection(Vect(sinMoveTime, -1.0f, cosmoveTime).getNorm());

	pTestLight = pWater2ShaderTexLight->FindSpotLight(SpotLight::Name::SPOT_2);
	pTestLight->SetDirection(Vect(sinMoveTime, -1.0f, cosmoveTime).getNorm());

	pTestLight = pTerrainShaderTexLight->FindSpotLight(SpotLight::Name::SPOT_3);
	pTestLight->SetDirection(Vect(cosmoveTime, -1.0f, sinMoveTime).getNorm());

	pTestLight = pWaterShaderTexLight->FindSpotLight(SpotLight::Name::SPOT_3);
	pTestLight->SetDirection(Vect(cosmoveTime, -1.0f, sinMoveTime).getNorm());

	pTestLight = pWater2ShaderTexLight->FindSpotLight(SpotLight::Name::SPOT_3);
	pTestLight->SetDirection(Vect(cosmoveTime, -1.0f, sinMoveTime).getNorm());

	// UFO Rotate
	UFOWorld = Matrix(SCALE, 0.015f, 0.015f, 0.015f) * Matrix(ROT_Y, cosmoveTime) * Matrix(TRANS, -30.0f, 20.0f, -30.0f);
	pUFOGraphTexLight->SetWorld(UFOWorld);

	// Cam Control
	float camSpeed = 0.051f;
	float rotSpeed = 0.005f;
	static bool keyPressed = false;
	GodCam(mCam, camSpeed, rotSpeed, keyPressed);

	// Continue motion till any cam key pressed.
	if (!keyPressed)
	{
		CamSplineControl::Update(mCam, mTimer.TotalTime());
	}
}

void DXApp::DrawScene()
{
	// Clear the back buffer
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	// Clear the depth stencil;
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//>>>>> STEP 1: Render Objects in front of mirror.

	// Troll
	pTrollGraphTexLight->SetEyePos(mCam);
	pTrollGraphTexLight->SetWorld(trollWorld);
	pTrollGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.4, 0.4, 0.4, 100));
	pTrollGraphTexLight->Render();
		
	// Troll 2
	pTroll2GraphTexLight->SetEyePos(mCam);
	pTroll2GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(1.0f, 1.0f, 1.0f, 30));
	pTroll2GraphTexLight->Render();

	// Small Troll
	pSmallTrollGraphTexLight->SetEyePos(mCam);
	pSmallTrollGraphTexLight->SetWorld(smallTrollWorld);
	pSmallTrollGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.4, 0.4, 0.4, 100));
	pSmallTrollGraphTexLight->Render();

	// Wizard
	pWizardGraphTexLight->SetEyePos(mCam);
	pWizardGraphTexLight->SetWorld(wizardWorld);
	pWizardGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.2f, 0.2f, 0.2f, 100));
	pWizardGraphTexLight->Render();

	// UFO
	pUFOGraphTexLight->SetEyePos(mCam);
	pUFOGraphTexLight->SetWorld(UFOWorld);
	pUFOGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(1.0f, 1.0f, 1.0f, 100));
	pUFOGraphTexLight->Render();

	// Tree
	pTreeGraphTexLight->SetEyePos(mCam);
	pTreeGraphTexLight->SetWorld(treeWorld);
	pTreeGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.6f, 0.6f, 0.6f, 100));
	pTreeGraphTexLight->Render();

	// Tree 2
	pTree2GraphTexLight->SetEyePos(mCam);
	pTree2GraphTexLight->SetWorld(tree2World);
	pTree2GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.6f, 0.6f, 0.6f, 100));
	pTree2GraphTexLight->Render();

	// Boat
	pBoatGraphTexLight->SetEyePos(mCam);
	pBoatGraphTexLight->SetWorld(boatWorld);
	pBoatGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.8f, 0.8f, 0.8f, 100));
	pBoatGraphTexLight->Render();

	// Flower
	pFlowerGraphTexLight->SetEyePos(mCam);
	pFlowerGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.8f, 0.8f, 0.8f, 100));
	pFlowerGraphTexLight->Render();

	// Flower 2
	pFlower2GraphTexLight->SetEyePos(mCam);
	pFlower2GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.8f, 0.8f, 0.8f, 100));
	pFlower2GraphTexLight->Render();

	// Fish
	pFishGraphTexLight->SetEyePos(mCam);
	pFishGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.8f, 0.8f, 0.8f, 100));
	pFishGraphTexLight->Render();
	

	////>>>>> STEP 2: Marking the mirror pixels

	pMirror->FirstRenderMirror(mCam.getViewMatrix(), mCam.getProjMatrix());

    //>>>> STEP 3:  Render objects that should be reflected in the mirror

	// Wizard
	pWizardGraphTexLight->SetEyePos(mCam);
	pWizardGraphTexLight->SetWorld(wizardWorld * pMirror->GetReflectionMatrix());
	pWizardGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.2f, 0.2f, 0.2f, 100));
	pWizardGraphTexLight->Render();

	// UFO
	pUFOGraphTexLight->SetEyePos(mCam);
	pUFOGraphTexLight->SetWorld(UFOWorld * pMirror->GetReflectionMatrix());
	pUFOGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(1.0f, 1.0f, 1.0f, 100));
	pUFOGraphTexLight->Render();

	// Tree
	pTreeGraphTexLight->SetEyePos(mCam);
	pTreeGraphTexLight->SetWorld(treeWorld * pMirror->GetReflectionMatrix());
	pTreeGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.6f, 0.6f, 0.6f, 100));
	pTreeGraphTexLight->Render();

	// Tree 2
	pTree2GraphTexLight->SetEyePos(mCam);
	pTree2GraphTexLight->SetWorld(tree2World * pMirror->GetReflectionMatrix());
	pTree2GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.6f, 0.6f, 0.6f, 100));
	pTree2GraphTexLight->Render();

	// Boat
	pBoatGraphTexLight->SetEyePos(mCam);
	pBoatGraphTexLight->SetWorld(boatWorld * pMirror->GetReflectionMatrix());
	pBoatGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.8f, 0.8f, 0.8f, 100));
	pBoatGraphTexLight->Render();

	// Troll
	pTrollGraphTexLight->SetEyePos(mCam);
	pTrollGraphTexLight->SetWorld(trollWorld * pMirror->GetReflectionMatrix());
	pTrollGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.4, 0.4, 0.4, 100));
	pTrollGraphTexLight->Render();

	// Small Troll
	pSmallTrollGraphTexLight->SetEyePos(mCam);
	pSmallTrollGraphTexLight->SetWorld(smallTrollWorld * pMirror->GetReflectionMatrix());
	pSmallTrollGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.4, 0.4, 0.4, 100));
	pSmallTrollGraphTexLight->Render();

	// Skybox 2
	pSky2GraphTex->SetEyePos(mCam);
	pSky2GraphTex->SetWorld(sky2World * pMirror->GetReflectionMatrix());
	pSky2GraphTex->Render();

	// Terrain
	pTerrainGraphTexLight->SetEyePos(mCam);
	pTerrainGraphTexLight->UpdateAllLightPositions(pMirror->GetReflectionMatrix());
	pTerrainGraphTexLight->SetWorld(terrainWorld * pMirror->GetReflectionMatrix());
	pTerrainGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.2f, 0.2f, 0.2f, 30));
	pTerrainGraphTexLight->Render();
	pTerrainGraphTexLight->UpdateAllLightPositions(pMirror->GetReflectionMatrix().getInv());

	// Water 2
	pWater2GraphTexLight->SetEyePos(mCam);
	pWater2GraphTexLight->UpdateAllLightPositions(pMirror->GetReflectionMatrix());
	pWater2GraphTexLight->SetWorld(water2World * pMirror->GetReflectionMatrix());
	pWater2GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0, 0, 0, 150));
	pWater2GraphTexLight->Render();
	pWater2GraphTexLight->UpdateAllLightPositions(pMirror->GetReflectionMatrix().getInv());


	this->ActivateTransparency();

	// Skybox
	pSkyGraphTex->SetEyePos(mCam);
	pSkyGraphTex->SetWorld(skyWorld * pMirror->GetReflectionMatrix());
	pSkyGraphTex->Render();

	// Water
	pWaterGraphTexLight->SetEyePos(mCam);
	pWaterGraphTexLight->UpdateAllLightPositions(pMirror->GetReflectionMatrix());
	pWaterGraphTexLight->SetWorld(waterWorld * pMirror->GetReflectionMatrix());
	pWaterGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0, 0, 0, 150));
	pWaterGraphTexLight->Render();
	pWaterGraphTexLight->UpdateAllLightPositions(pMirror->GetReflectionMatrix().getInv());

	this->DeactivateTransparency();

	//>>>>>>> STEP 4: Render the mirror again, using alpha blending.

	pMirror->FinalRenderMirror(mCam.getViewMatrix(), mCam.getProjMatrix());

	//>>>>>>> STEP 5: Render objects behind the mirror

	// Skybox 2
	pSky2GraphTex->SetEyePos(mCam);
	pSky2GraphTex->SetWorld(sky2World);
	pSky2GraphTex->Render();

	// Terrain
	pTerrainGraphTexLight->SetEyePos(mCam);
	pTerrainGraphTexLight->SetWorld(terrainWorld);
	pTerrainGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.2f, 0.2f, 0.2f, 30));
	pTerrainGraphTexLight->Render();

	// Seagull
	pSeagullGraphTexLight->SetEyePos(mCam);
	pSeagullGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.4, 0.4, 0.4, 100));
	pSeagullGraphTexLight->Render();

	// Tree 3
	pTree3GraphTexLight->SetEyePos(mCam);
	pTree3GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0.6f, 0.6f, 0.6f, 100));
	pTree3GraphTexLight->Render();

	// Water 2
	pWater2GraphTexLight->SetEyePos(mCam);
	pWater2GraphTexLight->SetWorld(water2World);
	pWater2GraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0, 0, 0, 150));
	pWater2GraphTexLight->Render();

	// Mirror Box
	pBoxGraphColLight->SetEyePos(mCam);
	pBoxGraphColLight->SetMaterial(Colors::Brown, Colors::Brown, Vect(0.3f, 0.3f, 0.3f, 150));
	pBoxGraphColLight->Render();

	this->ActivateTransparency();

	// Skybox
	pSkyGraphTex->SetEyePos(mCam);
	pSkyGraphTex->SetWorld(skyWorld);
	pSkyGraphTex->Render();

	// Water
	pWaterGraphTexLight->SetEyePos(mCam);
	pWaterGraphTexLight->SetWorld(waterWorld);
	pWaterGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0, 0, 0, 150));
	pWaterGraphTexLight->Render();

	// Planet
	pPlanetGraphTexLight->SetEyePos(mCam);
	pPlanetGraphTexLight->SetMaterial(Colors::White, Colors::White, Vect(0, 0, 0, 150));
	pPlanetGraphTexLight->Render();

	this->DeactivateTransparency();

	// Switches the display to show the now-finished back-buffer
	mSwapChain->Present(0, 0);
}

DXApp::~DXApp()
{
	// Manager
	CamSplineControl::Destroy();
	LODManager::Destroy();
	TextureManager::Destroy();
	ShaderManager::Destroy();
	ModelManager::Destroy();

	//// Models
	delete pTerrainModel;
	delete pWaterModel;
	delete pWater2Model;
	delete pSkyModel;
	delete pSky2Model;

	// Graphic Objects
	delete pTrollGraphTexLight;
	delete pTroll2GraphTexLight;
	delete pSkyGraphTex;
	delete pSky2GraphTex;
	delete pSmallTrollGraphTexLight;
	delete pSeagullGraphTexLight;
	delete pTerrainGraphTexLight;
	delete pWizardGraphTexLight;
	delete pWaterGraphTexLight;
	delete pWater2GraphTexLight;
	delete pUFOGraphTexLight;
	delete pTreeGraphTexLight;
	delete pTree2GraphTexLight;
	delete pTree3GraphTexLight;
	delete pPlanetGraphTexLight;
	delete pBoatGraphTexLight;
	delete pBoxGraphColLight;
	delete pFlowerGraphTexLight;
	delete pFlower2GraphTexLight;
	delete pFishGraphTexLight;

	// Mirror.
	delete pMirror;

	// DX Stuff
	ReleaseAndDeleteCOMobject(FrontFaceAsCCWRS);
	ReleaseAndDeleteCOMobject(TransparentBS);
	ReleaseAndDeleteCOMobject(mRenderTargetView);
	ReleaseAndDeleteCOMobject(mpDepthStencilView);
	ReleaseAndDeleteCOMobject(mSwapChain);
	ReleaseAndDeleteCOMobject(md3dImmediateContext);

	// Must be done BEFORE the device is released
	ReportLiveDXObjects();		// See http://masterkenth.com/directx-leak-debugging/

	ReleaseAndDeleteCOMobject(md3dDevice);
}

DXApp::DXApp(HWND hwnd)
{
	assert(hwnd);
	mhMainWnd = hwnd;

	BackgroundColor = Colors::MidnightBlue;

	md3dDevice = nullptr;
	md3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	CHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);
	mMainWndCaption = str;

	// Initialize DX11
	this->InitDirect3D();

	// Demo initialization
	this->InitDemo();
}

void DXApp::InitDirect3D()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This is a *greatly* simplified process to create a DX device and context:
	// We force the use of DX11 feature level since that's what CDM labs are limited to.
	// For real-life applications would need to test what's the best feature level and act accordingly
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &md3dDevice, nullptr, &md3dImmediateContext);
	assert(SUCCEEDED(hr));

	// Now we obtain the associated DXGIfactory1 with our device 
	// Many steps...
	IDXGIDevice* dxgiDevice = nullptr;
	hr = md3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = nullptr;
	hr = dxgiDevice->GetAdapter(&adapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory1* dxgiFactory1 = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory1));
	assert(SUCCEEDED(hr));
	// See also note on weird stuff with factories and swap chains (1s and 2s)
	// https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

	// We are done with these now...
	ReleaseAndDeleteCOMobject(adapter);
	ReleaseAndDeleteCOMobject(dxgiDevice);

	// Controls MSAA option:
	// - 4x count level garanteed for all DX11 
	// - MUST be the same for depth buffer!
	// - We _need_ to work with the depth buffer because reasons... (see below)
	DXGI_SAMPLE_DESC sampDesc = { 0 };
	sampDesc.Count = 1;
	sampDesc.Quality = static_cast<UINT>(D3D11_CENTER_MULTISAMPLE_PATTERN);  // MS: what's with the type mismtach?

	DXGI_MODE_DESC buffdesc;				// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173064(v=vs.85).aspx
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Next we create a swap chain. 
	// Useful thread: http://stackoverflow.com/questions/27270504/directx-creating-the-swapchain
	// Note that this is for a DirectX 11.0: in a real app, we should test the feature levels and act accordingly

	DXGI_SWAP_CHAIN_DESC sd;				// See MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173075(v=vs.85).aspx
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;						// Much confusion about this number... see http://www.gamedev.net/topic/633807-swap-chain-buffer-count/
	sd.BufferDesc = buffdesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mhMainWnd;
	sd.SampleDesc = sampDesc;
	sd.Windowed = TRUE;

	hr = dxgiFactory1->CreateSwapChain(md3dDevice, &sd, &mSwapChain);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(dxgiFactory1);

	// Create a render target view		https://msdn.microsoft.com/en-us/library/windows/desktop/ff476582(v=vs.85).aspx
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	assert(SUCCEEDED(hr));;

	hr = md3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	ReleaseAndDeleteCOMobject(pBackBuffer);
	assert(SUCCEEDED(hr));

	/**********************************************************/

	// First we fix what it means for triangles to be front facing.
	// Requires setting a whole new rasterizer state
	//*
	D3D11_RASTERIZER_DESC rd = {  };
	rd.FillMode = D3D11_FILL_SOLID;  // Also:*/ D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_BACK;
	rd.FrontCounterClockwise = true; // true for RH forward facing
	rd.DepthBias = 0;
	rd.SlopeScaledDepthBias = 0.0f;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;  // Does not in fact turn on/off multisample: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476198(v=vs.85).aspx
	rd.AntialiasedLineEnable = false;

	//ID3D11RasterizerState* rs;
	md3dDevice->CreateRasterizerState(&rd, &FrontFaceAsCCWRS);

	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);
	//ReleaseAndDeleteCOMobject(rs); // we can release this resource since we won't be changing it any further
	//*/

	// We must turn on the abilty to process depth during rendering.
	// Done through depth stencils (see https://msdn.microsoft.com/en-us/library/windows/desktop/bb205074(v=vs.85).aspx)
	// Below is a simplified version
	//*
	D3D11_TEXTURE2D_DESC descDepth = { 0 };
	descDepth.Width = mClientWidth;
	descDepth.Height = mClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = sampDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencil;
	hr = md3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	assert(SUCCEEDED(hr));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;;

	hr = md3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mpDepthStencilView);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(pDepthStencil);
	//*/

	/**********************************************************/

	//md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);  // to use without depth stencil
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mpDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp = { 0 };
	vp.Width = (FLOAT)mClientWidth;
	vp.Height = (FLOAT)mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	md3dImmediateContext->RSSetViewports(1, &vp);

	// These settings implement the standard alpha blending: c_src*(a_src) + c_dst*(1-a_src) 
	D3D11_BLEND_DESC bd = { };
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = md3dDevice->CreateBlendState(&bd, &TransparentBS);
	assert(SUCCEEDED(hr));
}

// See http://masterkenth.com/directx-leak-debugging/
void DXApp::ReportLiveDXObjects()
{
#ifdef _DEBUG
	HRESULT hr = S_OK;

	// Now we set up the Debug interface, to be queried on shutdown
	ID3D11Debug* debugDev = nullptr;
	hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ReleaseAndDeleteCOMobject(debugDev);
#endif
}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::ActivateTransparency()
{
	// BLENDING: Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	md3dImmediateContext->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);
}

void DXApp::DeactivateTransparency()
{
	// BLENDING: Back to normal blending (turn off transparent blending)
	md3dImmediateContext->OMSetBlendState(0, nullptr, 0xffffffff);
}

void DXApp::FrameTick()
{
	mTimer.Tick();
	CalculateFrameStats();

	this->UpdateScene();
	this->DrawScene();
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}