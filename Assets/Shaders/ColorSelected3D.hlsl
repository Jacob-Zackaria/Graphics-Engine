#pragma pack_matrix( row_major )

struct Fog
{
    uint fogActive;
    float fogStart;
    float fogRange;
    float4 fogColor;
};

cbuffer CamData : register(b0)
{
    float4x4 View;
    float4x4 Projection;
}

cbuffer InstanceData : register(b1)
{
    float4x4 World;
    float4x4 WorldInv;
    float4 SelectedColor;
};

cbuffer FogData : register(b2)
{
    Fog newFog;
    float4 EyePosWorld;
}


//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
    float4 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION)
{
    VS_OUTPUT output;
    output.PosMS = Pos;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = SelectedColor;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 msEyePos = mul(EyePosWorld, WorldInv);
    if (newFog.fogActive == 1)
    {
        float distToEye = length(msEyePos - input.PosMS);
        float FogPercent = saturate((distToEye - newFog.fogStart) / newFog.fogRange);  // See https://msdn.microsoft.com/en-us/library/windows/desktop/bb509645(v=vs.85).aspx

        input.Color = input.Color * (1 - FogPercent) + newFog.fogColor * FogPercent;
    }

    return input.Color;
}
