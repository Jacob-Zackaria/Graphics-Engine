// ShaderColor
// Andre Berthiaume, july 2016

#ifndef _ShaderTexture
#define _ShaderTexture

#include "ShaderBase.h"
#include "Matrix.h"
#include "Texture.h"

struct ID3D11Buffer;
struct ID3D11Device;
class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(const ShaderTexture&) = delete;				 // Copy constructor
	ShaderTexture(ShaderTexture&&) = default;                    // Move constructor
	ShaderTexture& operator=(const ShaderTexture&) = default;  // Copy assignment operator
	ShaderTexture& operator=(ShaderTexture&&) = default;       // Move assignment operator
	~ShaderTexture();		  							         // Destructor

	ShaderTexture(ID3D11Device* device, ID3D11DeviceContext* devcon);

	virtual void SetToContext() override;

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendWorldColor(const Matrix& world, const Vect& col);

private:
	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer* mpBufferCamMatrices;

	struct WorldData
	{
		Matrix World;
		Vect color;
	};

	ID3D11Buffer* mpBuffWorld;
};

#endif _ShaderTexture