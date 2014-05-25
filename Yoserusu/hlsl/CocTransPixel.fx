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

Texture2D tex : register( t0 );
SamplerState texSamp : register( s0 )
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct PixelIn{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 X : TEXCOORD2;
};

float4 ps_coc( PixelIn In ) : SV_Target {
	float3 L = -mLight.xyz; //ライトベクトル
	float3 N = normalize( In.N ); //法線ベクトル
	float3 V = normalize( mEye.xyz - In.X ); //視線ベクトル
	float3 H = normalize( L + V ); //ハーフベクトル

	//計算に使うそれぞれの角度
	float NV = dot( N, V );
	float NH = dot( N, H );
	float VH = dot( V, H );
	float NL = dot( N, L );
	float LH = dot( L, H );

	const float m = 0.35f;//粗さ
	float NH2 = NH*NH;
	float D = exp( -( 1 - NH2 ) / ( NH2 * m * m ) ) / ( 4 * m * m * NH2 * NH2 );

	float G = min( 1, min( 2 * NH * NV / VH, 2 * NH * NL / VH ) );

	//フレネル
	float n = 20.0f;//屈折率
	float g = sqrt( n * n + LH * LH - 1 );
	float gpc = g + LH;
	float gnc = g - LH;
	float cgpc = LH * gpc - 1;
	float cgnc = LH * gnc + 1;
	float F = 0.5f * gnc * gnc * ( 1 + cgpc * cgpc / ( cgnc * cgnc ) ) / ( gpc * gpc );

	//金属の色
	float4 ks = { 2.0f * 0.468f, 2.0f * 0.433f, 2.0f * 0.185f, 1.0f };
	In.col = In.col + ks * max( 0, F * D * G / NV ); 

	float4 color = In.col * tex.Sample( texSamp, In.tex );

	color.xyz = color.xyz * mDiffuse.xyz;
	color.w = mDiffuse.w;

	return color;
}