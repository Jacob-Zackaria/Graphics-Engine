// ShaderTextureLight

#ifndef _ShaderTextureLight
#define _ShaderTextureLight

#include "ShaderBase.h"
#include "Matrix.h"
#include "Texture.h"

#include "LightType.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct ID3D11Buffer;
struct ID3D11Device;

// Maximum lights in the scene.
#define MAX_NUM_LIGHTS 12

class ShaderTextureLight : public ShaderBase
{
public:
	ShaderTextureLight(const ShaderTextureLight&) = delete;				 // Copy constructor
	ShaderTextureLight(ShaderTextureLight&&) = default;                    // Move constructor
	ShaderTextureLight& operator=(const ShaderTextureLight&) & = default;  // Copy assignment operator
	ShaderTextureLight& operator=(ShaderTextureLight&&) & = default;       // Move assignment operator
	~ShaderTextureLight();		  							         // Destructor

	ShaderTextureLight(ID3D11Device* device, ID3D11DeviceContext* devcon);

	virtual void SetToContext() override;

	void AddFogParameters(const float fogStart, const float fogRange, const Vect fogColor);

	void UpdateAllLightPositions(const Matrix& m);
	void AddDirectionalLight(DirectionalLight::Name newName, DirectionalLight& newLight);
	void AddPointLight(PointLight::Name newName, PointLight& newLight);
	void AddSpotLight(SpotLight::Name newName, SpotLight& newLight);

	SpotLight* FindSpotLight(SpotLight::Name newName);
	PointLight* FindPointLight(PointLight::Name newName);
	DirectionalLight* FindDirectionalLight(DirectionalLight::Name newName);

	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void SendLightParameters(const Vect& eyepos);
	void SendWorldAndMaterial(const Matrix& world, const Vect& amb = Vect(.5f, .5f, .5f), const Vect& dif = Vect(.5f, .5f, .5f), const Vect& sp = Vect(.5f, .5f, .5f), const Vect& col = Vect(1.0f, 1.0f, 1.0f));

	struct Material
	{
		Vect Ambient;
		Vect Diffuse;
		Vect Specular;
	};
private:
	void privAddToFront(LightType* node, LightType*& head);
	void privRemove(LightType* node, LightType*& head);


	// Linked list of lights
	struct DirectionalLightShader
	{
		DirectionalLightShader()
			:
			Light{},
			Direction()
		{}

		const DirectionalLightShader& operator = (const DirectionalLight& newLight)
		{
			this->Light = newLight.GetLight();
			this->Direction = newLight.GetDirection();
			return (*this);
		}

		LightType::PhongADS Light;
		Vect Direction;
	};
	DirectionalLight* DirLightData;

	struct PointLightShader
	{
		PointLightShader()
			:
			Light{},
			Position(),
			Attenuation(),
			Range(0.0f)
		{}

		const PointLightShader& operator = (const PointLight& newLight)
		{
			this->Light = newLight.GetLight();
			this->Position = newLight.GetPosition();
			this->Attenuation = newLight.GetAttenuation();
			this->Range = newLight.GetRange();
			return (*this);
		}

		LightType::PhongADS Light;
		Vect Position;
		Vect Attenuation;
		float Range;
	};
	PointLight* PointLightData;

	struct SpotLightShader
	{
		SpotLightShader()
			:
			Light{},
			Position(),
			Attenuation(),
			Direction(),
			SpotExp(0.0f),
			Range(0.0f)
		{}

		const SpotLightShader& operator = (const SpotLight& newLight)
		{
			this->Light = newLight.GetLight();
			this->Position = newLight.GetPosition();
			this->Attenuation = newLight.GetAttenuation();
			this->Direction = newLight.GetDirection();
			this->SpotExp = newLight.GetExposure();
			this->Range = newLight.GetRange();
			return (*this);
		}

		LightType::PhongADS Light;
		Vect Position;
		Vect Attenuation;
		Vect Direction;
		float SpotExp;
		float Range;
	};
	SpotLight* SpotLightData;

	struct CamMatrices
	{
		Matrix View;
		Matrix Projection;
	};

	ID3D11Buffer* mpBufferCamMatrices;

	struct Data_WorldAndMaterial
	{
		Matrix World;
		Matrix WorlInv;
		Material Mat;
		Vect color;
	};

	ID3D11Buffer* mpBuffWordAndMaterial;

	struct Data_LightParams
	{
		DirectionalLightShader DirLight[MAX_NUM_LIGHTS];
		PointLightShader PntLight[MAX_NUM_LIGHTS];
		SpotLightShader SptLight[MAX_NUM_LIGHTS];
		Vect EyePosWorld;
	};

	ID3D11Buffer* mpBufferLightParams;

	struct LightCount
	{
		unsigned int num_dir_light;
		unsigned int num_point_light;
		unsigned int num_spot_light;

		// empty data to align 16 byte for the hlsl.
		unsigned int num_gg_light;
	};

	ID3D11Buffer* mpBufferLightCount;

	LightCount lightCount;

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
	};

	ID3D11Buffer* mpBufferFog;

	Fog newFog;
};

#endif _ShaderTextureLight

