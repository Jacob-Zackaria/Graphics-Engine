// ShaderColor
// Andre Berthiaume, july 2016

#ifndef _ShaderColor
#define _ShaderColor

#include "ShaderBase.h"
#include "Matrix.h"
#include "Camera.h"

struct ID3D11Buffer;
struct ID3D11Device;

class ShaderColor : public ShaderBase
{
public:
	ShaderColor(const ShaderColor&) = delete;				 // Copy constructor
	ShaderColor(ShaderColor&&) = default;                    // Move constructor
	ShaderColor& operator=(const ShaderColor&) & = default;  // Copy assignment operator
	ShaderColor& operator=(ShaderColor&&) & = default;       // Move assignment operator
	~ShaderColor();		  							         // Destructor

	ShaderColor(ID3D11Device* device, ID3D11DeviceContext* devcon);

	virtual void SetToContext() override;

	void AddFogParameters(const float fogStart, const float fogRange, const Vect fogColor);
	void SetEyePos(const Camera& mCam);
	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendWorldColor(const Matrix& world, const Vect& col);

private:
	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer* mpBufferCamMatrices;

	struct Data_WorldColor
	{
		Matrix World;
		Matrix WorlInv;
		Vect Color;
	};

	ID3D11Buffer* mpBuffWordColor;

	struct Fog
	{
		Fog()
			:
			fogActive(0),
			fogStart(0.0f),
			fogRange(0.0f),
			fogColor(Vect())
		{
		}
		unsigned int fogActive;
		float fogStart;
		float fogRange;
		Vect fogColor;
		Vect EyePosWorld;
	};

	ID3D11Buffer* mpBufferFog;

	Fog newFog;
};

#endif _ShaderColor
