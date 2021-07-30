// GraphicObject
// Andre Berthiaume, July 2016

#ifndef _GraphicObject_Base
#define _GraphicObject_Base

#include "Matrix.h"

class Model;
class ShaderMeshData;
struct ID3D11DeviceContext;
class ShaderBase;


class GraphicObjectBase : public Align16
{
public:
	GraphicObjectBase(const GraphicObjectBase&) = delete;				   // Copy constructor
	GraphicObjectBase(GraphicObjectBase&&) = default;                    // Move constructor
	GraphicObjectBase& operator=(const GraphicObjectBase&) & = default;  // Copy assignment operator
	GraphicObjectBase& operator=(GraphicObjectBase&&) & = default;       // Move assignment operator
	~GraphicObjectBase() = default;		  							   // Destructor
	GraphicObjectBase();

	void SetModel(Model* mod);
	virtual void Render() = 0;

protected:
	Model* pModel;

};

#endif _GraphicObject_Base
