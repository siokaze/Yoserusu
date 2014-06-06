#define BALL 0
#define ARM 1
#define WALL 2

#define DEBUG 0

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
cbuffer ConstantBasic : register(b1)
{
	int mDrawType;
	float4x4 mWorldInv;
	float3 mDummy;
};

Texture2D tex_0 : register( t0 );
SamplerState texSamp_0 : register( s0 )
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D tex_1 : register( t1 );
SamplerState texSamp_1 : register( s1 )
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

TextureCube envMap : register( t2 ); //キューブマップ
SamplerState texSamp_2 : register( s2 )
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
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
	float3 N : TEXCOORD1;
	float3 X : TEXCOORD2;
	float3 eye : TEXCOORD3;
	float3 vpos : TEXCOORD4;
	float3 light : TEXCOORD5;
};

VS_OUTPUT vs_coc( VS_INPUT In ){
	VS_OUTPUT Out;

	float4 world_pos, view_pos;
	world_pos = mul( In.pos, mWorld );
	view_pos = mul( In.pos, mView );

	//座標変換
	Out.pos = mul( In.pos, mWorld );
	Out.pos = mul( Out.pos, mView );
	Out.pos = mul( Out.pos, mProj );

	//拡散光 + 環境光
	float amb = -mLight.w;
	float3 L = -mLight.xyz;
	Out.col = ( In.col * -mLight ) * max( amb, dot( float4( In.nor, 1.f ), -mLight ) );

	Out.tex = In.tex;

	float3 nor = mul( In.nor, (float3x3)mWorld  );


	Out.N = nor.xyz;
	Out.X = In.pos.xyz;

	Out.vpos = view_pos.xyz;

	float4 t = normalize( mul( float4( In.tan.xyz,1.0f ), ( mWorldInv ) ) ); 
	float4 b = normalize( mul( float4( In.bin.xyz,1.0f ), ( mWorldInv ) ) ); 
	float4 n = normalize( mul( float4( In.nor.xyz,1.0f ), ( mWorldInv ) ) );

	float3 light = world_pos.xyz - mLight.xyz;
	Out.light.x = dot(light, t.xyz); 
	Out.light.y = dot(light, b.xyz); 
	Out.light.z = dot(light, n.xyz); 
	Out.light = normalize(Out.light);

	Out.eye = world_pos.xyz - mEye.xyz;
	Out.eye = normalize( Out.eye );




	return Out;
}

float4 ps_coc( VS_OUTPUT In ) : SV_Target {
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
	float4 ks = { 1.0f, 1.0f, 1.0f, 1.0f };
	In.col = In.col * 1.5f; 

	float4 color = float4( 0, 0, 0, 1 );
	if( mDrawType == WALL){ //BumpMapを使います
		color = In.col * tex_0.Sample( texSamp_0, In.tex );
	}
	if( mDrawType == ARM ){//通常描画
		float3 reflect_vec = reflect( In.eye, In.N );
		color = In.col * envMap.Sample( texSamp_0, reflect_vec );
		color = saturate( 1.0f * color + 0.2 );
	}
	if( mDrawType == BALL ){ //tex2枚で描画
		float4 texCol_0, texCol_1;
		texCol_0 = tex_0.Sample( texSamp_0, In.tex );
		texCol_1 = tex_1.Sample( texSamp_1, In.tex );
		color = In.col * texCol_0 * texCol_1;
	}

	color = color + ks*max( 0, F * D * G / NV ) / 2;

	color.xyz = color.xyz * mDiffuse.xyz*1.5;
	color.w = mDiffuse.w;

	return color+pow(max(0,dot(N,H)),50);
}