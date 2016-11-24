//+-----------------------------------------------------------------------------
//| Input structure
//+-----------------------------------------------------------------------------
struct VS_IN
{
  float4 Position : POSITION;
  float3 Normal : NORMAL;
  float4 Color : COLOR0;
  float2 TexturePosition : TEXCOORD;
};


//+-----------------------------------------------------------------------------
//| Output structure
//+-----------------------------------------------------------------------------
struct VS_OUT
{
  float4 Position : POSITION;
  float2 TexturePosition : TEXCOORD0;
  float3 Light : TEXCOORD1;
  float3 Normal : TEXCOORD2;
  float3 View : TEXCOORD3;
  float4 Color : COLOR0;
};


//+-----------------------------------------------------------------------------
//| Global variables
//+-----------------------------------------------------------------------------
float4x4 WorldMatrix;
float4x4 ViewMatrix;
float4x4 ProjectionMatrix;
float4x4 WorldViewProjectionMatrix;
float4 CameraPosition;
float4 CameraDirection;
float4 AmbientColor;
float4 DiffuseColor;
float4 SpecularColor;
float SpecularPower;


//+-----------------------------------------------------------------------------
//| Main shader function
//+-----------------------------------------------------------------------------
VS_OUT main(VS_IN In)
{
  VS_OUT Out;

  float3 WorldPosition = normalize(mul(In.Position, WorldMatrix));

  Out.Position = mul(In.Position, WorldViewProjectionMatrix);
  Out.TexturePosition = In.TexturePosition;
  Out.Light = -CameraDirection;
  Out.Normal = normalize(In.Normal);
  Out.View = normalize(CameraPosition - WorldPosition);
  Out.Color = In.Color;

  return Out;
}
