

#ifndef _GraphicObject_Texture
#define _GraphicObject_Texture

#include "GraphicObjectBase.h"
#include "Vect.h"
#include "Camera.h"
#include "ShaderTexture.h"

class GraphicObjectTexture : public GraphicObjectBase
{
friend class ShaderTexture;

public:
	GraphicObjectTexture(const GraphicObjectTexture&) = delete;				 // Copy constructor
	GraphicObjectTexture(GraphicObjectTexture&&) = default;                    // Move constructor
	GraphicObjectTexture& operator=(const GraphicObjectTexture&) & = default;  // Copy assignment operator
	GraphicObjectTexture& operator=(GraphicObjectTexture&&) & = default;       // Move assignment operator
	~GraphicObjectTexture();		  											 // Destructor

	GraphicObjectTexture() = delete;

	void SetEyePos(Camera& mCam);
	void SetTexture(Texture* newTex);
	void SetWorld(const Matrix& m);
	void SetColor(const Vect& col);
	Matrix& GetWorld();
	virtual void Render() override;

	GraphicObjectTexture(ShaderTexture* shader, Model* mod, Texture* pNewTex);

private:
	ShaderTexture*					pShader;
	Matrix							World;
	Vect							Color;
	Texture*						mTex;
};

#endif _GraphicObject_Texture
