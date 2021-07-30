#pragma pack_matrix( row_major )

// Maximum lights in the scene.
#define MAX_NUM_LIGHTS 12

struct LightCount
{
	uint num_dir_light;
	uint num_point_light;
	uint num_spot_light;

	// empty data to align 16 byte for the hlsl.
	uint num_gg_light;
};

struct Fog
{
	uint fogActive;
	float fogStart;
	float fogRange;
	float4 fogColor;
};

// Material properties
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;  // Hack: w holds the specular power
};

// Light properties
struct PhongADS
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

// Basic Phong Illumination computation
void PhongModel(
	Material mat,
	PhongADS lightint,
	float3 L,
	float3 normal,
	float3 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// every light source adds to the ambient light total
	ambient = mat.Ambient * lightint.Ambient;

	float diffuseFactor = dot(L, normal);

	// Diffuse and specular contribution only if not facing away from light source
	if (diffuseFactor > 0.0f)
	{
		diffuse = diffuseFactor * mat.Diffuse * lightint.Diffuse;

		float3 r = reflect( -L, normal);		// Compute the reflected ray of light
		float spec_val = max(dot(r, DirToEye.xyz), 0);
		float specFactor = pow(spec_val, mat.Specular.w);
		spec = specFactor * mat.Specular * lightint.Specular;
	}
	else
	{
		diffuse = float4(0, 0, 0, 0);
		spec = float4(0, 0, 0, 0);
	}
}

// Directional Light Data
struct DirectionalLight
{
	PhongADS LightInt;
	float4 Direction;
};

// Point Light data
struct PointLight
{
	PhongADS LightInt;
	float4 Position;
	float4 Attenuation;
	float Range;
};

// Spot light data
struct SpotLight
{
	PhongADS LightInt;
	float4 Position;
	float4 Attenuation;
	float4 Direction;
	float SpotExp;
	float Range;
};

//  Constant Buffers
cbuffer CamData : register(b0)
{
	float4x4 View;
	float4x4 Projection;
};

cbuffer LightParameters : register (b1)
{
	DirectionalLight DirLight[MAX_NUM_LIGHTS];
	PointLight PntLight[MAX_NUM_LIGHTS];
	SpotLight SpLight[MAX_NUM_LIGHTS];
	float4 EyePosWorld;
};

cbuffer InstanceData : register(b2)
{
	float4x4 World;
	float4x4 WorldInv;
	Material Mater;
};

cbuffer LightCountData : register(b3)
{
	LightCount lightCount;
};

cbuffer FogData : register(b4)
{
	Fog newFog;
}

// Directional Light Computation
void ComputeDirectionalLight(
	Material mat,
	DirectionalLight DirLit,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the initial ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// we compute ligth in the model-space 
	// For directional light, the source is infinitely far, so translation have no effect
	float3 L = normalize(mul(-DirLit.Direction.xyz, (float3x3)WorldInv));

	PhongModel(mat, DirLit.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);
}

// Point Light Computation
void ComputePointLight(
	Material mat,
	PointLight PLight,
	float4 posms,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// we compute the model-space position
	float3 litPosMS = mul(PLight.Position, WorldInv).xyz;
	float3 L = litPosMS - posms.xyz;

	// Early out if out of range
	float d = length(L);
	if (d > PLight.Range)
		return;

	L /= d;	// normalize 

	PhongModel(mat, PLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
	float att = 1 / dot(PLight.Attenuation.xyz, float3(1,d, d*d) );
	// Ambient not attenuated
	diffuse *= att;
	spec *= att;
}

// Spot Light Computation
void ComputeSpotLight(
	Material mat,
	SpotLight SptLight,
	float4 posms,
	float4 normal,
	float4 DirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
	// Zeroing out the ADS contributions
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	// ****** Same calculations as Point Light  ****

	// we compute the model-space position
	float3 litPosMS = mul(SptLight.Position, WorldInv).xyz;
	float3 L = litPosMS - posms.xyz;

	float d = length(L);
	if (d > SptLight.Range)
		return;

	L /= d;  // normalize 

	PhongModel(mat, SptLight.LightInt, L, normal.xyz, DirToEye.xyz, ambient, diffuse, spec);

	// Now we attenuate based on range
	float att = 1 / dot(SptLight.Attenuation.xyz, float3(1, d, d*d));

	// Ambient not attenuated
	diffuse *= att;
	spec *= att;

	// *****************************************

	// now we compute the spotlight factor
	float4 msLitDir = SptLight.Direction;
	msLitDir.w = 0; // making sure we have a vector and not a point
	msLitDir = normalize(mul(msLitDir, WorldInv));
	float spot = pow(max(dot(-L, msLitDir.xyz), 0), SptLight.SpotExp);

	ambient *= spot;
	diffuse *= spot;
	spec *= spot;
}



// ******* Shader operations

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float4 PosMS : POSITION;
	float4 Norm : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( float4 Pos : POSITION, float4 nor : NORMAL )
{
    VS_OUTPUT output;
	output.PosMS = Pos;  // We pass along the raw model space position 
	output.Norm = nor;	 // and the face normal

    output.Pos = mul( Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );


    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	// Compute light values in model-space
	float4 msEyePos = mul(EyePosWorld, WorldInv);
	float4 msDirToEye = normalize(msEyePos - input.PosMS);

	float4 ambient = float4(0, 0, 0, 0); 
	float4 diffuse = float4(0, 0, 0, 0);
	float4 spec = float4(0, 0, 0, 0);

	float4 A, D, S;

	for (uint i = 0; i < lightCount.num_dir_light; i++)
	{
		ComputeDirectionalLight(Mater, DirLight[i], normalize(input.Norm), msDirToEye, A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	for (uint i = 0; i < lightCount.num_point_light; i++)
	{
		ComputePointLight(Mater, PntLight[i], input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	for (uint i = 0; i < lightCount.num_spot_light; i++)
	{
		ComputeSpotLight(Mater, SpLight[i], input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 litColor = ambient + diffuse + spec;

	if (newFog.fogActive == 1)
	{
		float distToEye = length(msEyePos - input.PosMS);
		float FogPercent = saturate((distToEye - newFog.fogStart) / newFog.fogRange);  // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509645(v=vs.85).aspx

		litColor = litColor * (1 - FogPercent) + newFog.fogColor * FogPercent;
	}

	return litColor;
}
