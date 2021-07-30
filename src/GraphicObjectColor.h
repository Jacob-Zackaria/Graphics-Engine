// GraphicObject_Color
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Color
#define _GraphicObject_Color

#include "GraphicObjectBase.h"
#include "Vect.h"
#include "Camera.h"
#include "ShaderColor.h"

class GraphicObjectColor : public GraphicObjectBase
{
	friend class ShaderColor;

public:
	GraphicObjectColor(const GraphicObjectColor&) = delete;				 // Copy constructor
	GraphicObjectColor(GraphicObjectColor&&) = default;                    // Move constructor
	GraphicObjectColor& operator=(const GraphicObjectColor&) & = default;  // Copy assignment operator
	GraphicObjectColor& operator=(GraphicObjectColor&&) & = default;       // Move assignment operator
	~GraphicObjectColor();		  											 // Destructor

	GraphicObjectColor() = delete;

	void SetEyePos(Camera& mCam);
	void SetColor(const Vect& col);
	void SetWorld(const Matrix& m);
	Matrix& GetWorld();
	virtual void Render() override;

	GraphicObjectColor(ShaderColor* shader, Model* mod);

private:
	ShaderColor* pShader;
	Vect							Color;
	Matrix							World;

};

#endif _GraphicObject_Color
