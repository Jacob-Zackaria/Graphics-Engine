

#ifndef _GraphicObject_Texture_Light
#define _GraphicObject_Texture_Light

#include "GraphicObjectBase.h"
#include "Vect.h"
#include "Camera.h"
#include "ShaderTextureLight.h"

class GraphicObjectTextureLight : public GraphicObjectBase
{
friend class ShaderTextureLight;

public:
	GraphicObjectTextureLight(const GraphicObjectTextureLight&) = delete;				 // Copy constructor
	GraphicObjectTextureLight(GraphicObjectTextureLight&&) = default;                    // Move constructor
	GraphicObjectTextureLight& operator=(const GraphicObjectTextureLight&) & = default;  // Copy assignment operator
	GraphicObjectTextureLight& operator=(GraphicObjectTextureLight&&) & = default;       // Move assignment operator
	~GraphicObjectTextureLight();		  											 // Destructor

	GraphicObjectTextureLight() = delete;

	void UpdateAllLightPositions(const Matrix& m);
	void SetTexture(Texture* pNewTex);
	void SetWorld(const Matrix& m);
	void SetColor(const Vect& col);
	Matrix& GetWorld();
	void SetEyePos(Camera& mCam);
	void SetMaterial(const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));
	virtual void Render() override;

	GraphicObjectTextureLight(ShaderTextureLight* shader, Model* mod, Texture* pNewTex);

private:
	ShaderTextureLight*				pShader;
	Matrix							World;
	Vect							mEyePos;
	Vect							Color;
	ShaderTextureLight::Material	mMaterial;
	Texture*						mTex;
};

#endif _GraphicObject_Texture_Light
