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

struct VS_INPUT{
	float4 pos : POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 X : TEXCOORD2;
};

VS_OUTPUT vs_coc( VS_INPUT In ){
	VS_OUTPUT Out;

	//座標変換
	Out.pos = mul( In.pos, mWorld );
	Out.pos = mul( Out.pos, mView );
	Out.pos = mul( Out.pos, mProj );

	//拡散光 + 環境光
	float amb = -mLight.w;
	float3 L = -mLight.xyz;
	Out.col = ( In.col * -mLight ) * max( amb, dot( float4( In.nor, 1.f ), -mLight ) );

	Out.tex = In.tex;

	float4 n = mul( float4( In.nor, 1.0 ), mWorld );
	n = mul( n, mView );
//	n = mul( n, mProj );

	Out.N = n.xyz;
	Out.X = In.pos.xyz;

	return Out;
}