cbuffer ConstantBasic : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 mLight;
	float4 mDiffuse;
	float4 mAmbient;
	float4 mEye;
	float4 mSpecular;
};

Texture2D tex_0 : register( t0 );
SamplerState texSamp_0 : register( s0 )
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT{
	float4 pos : POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
	float3 bin : BINORMAL;
	float3 tan : TANGENT;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUTPUT vs_feadback( VS_INPUT In ){
	VS_OUTPUT Out;

	return Out;
}

float4 ps_feadback( VS_OUTPUT In ) : SV_Target {
	float4 color = float4( 1, 1, 1, 1 );

	return color;
}