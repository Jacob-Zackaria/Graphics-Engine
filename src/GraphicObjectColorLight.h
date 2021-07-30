

#ifndef _GraphicObject_Color_Light
#define _GraphicObject_Color_Light

#include "GraphicObjectBase.h"
#include "Vect.h"
#include "Camera.h"
#include "ShaderColorLight.h"

class GraphicObjectColorLight : public GraphicObjectBase
{
friend class ShaderColorLight;

public:
	GraphicObjectColorLight(const GraphicObjectColorLight&) = delete;				 // Copy constructor
	GraphicObjectColorLight(GraphicObjectColorLight&&) = default;                    // Move constructor
	GraphicObjectColorLight& operator=(const GraphicObjectColorLight&) & = default;  // Copy assignment operator
	GraphicObjectColorLight& operator=(GraphicObjectColorLight&&) & = default;       // Move assignment operator
	~GraphicObjectColorLight();		  											 // Destructor

	GraphicObjectColorLight() = delete;

	void UpdateAllLightPositions(const Matrix& m);
	void SetWorld(const Matrix& m);
	Matrix& GetWorld();
	void SetEyePos(Camera& mCam);
	void SetMaterial(const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f));
	virtual void Render() override;

	GraphicObjectColorLight(ShaderColorLight* shader, Model* mod);

private:
	ShaderColorLight*				pShader;
	Matrix							World;
	Vect							mEyePos;
	ShaderColorLight::Material		mMaterial;
};

#endif _GraphicObject_Color_Light
