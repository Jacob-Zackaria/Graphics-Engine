#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "Matrix.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class SkyBox : public Align16
{
private:
	Model* pPlane;

public:
	SkyBox(const SkyBox&) = delete;				// Copy constructor
	SkyBox(SkyBox&&) = delete;						// Move constructor
	SkyBox& operator=(const SkyBox&) & = delete;  // Copy assignment operator
	SkyBox& operator=(SkyBox&&) & = delete;       // Move assignment operator
	~SkyBox();

	SkyBox(ID3D11Device* dev, float len);

	Model* GetModel();
	void Render(ID3D11DeviceContext* context);

};


#endif SKY_BOX_H
