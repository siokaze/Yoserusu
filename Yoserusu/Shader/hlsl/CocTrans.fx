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
	float4 mDrawType;
	float4x4 mWorldInv;
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

	//法線をワールド空間へ
	float3 nor = mul( In.nor, (float3x3)mWorldInv );
	Out.N = normalize( nor.xyz );

	//拡散光 + 環境光
	float amb = -mLight.w;
	float3 L = -mLight.xyz;
	Out.col = In.col * max( amb, dot( float4( Out.N, 1.f ), -mLight ) );

	//テクスチャUV
	Out.tex = In.tex;

	//頂点別で確保
	Out.X = In.pos.xyz;

	//viewを頂点で確保
	Out.vpos = view_pos.xyz;

	float3 light = world_pos.xyz - mLight.xyz;
	Out.light = normalize(light);

	Out.eye = world_pos.xyz - mEye.xyz;
	Out.eye = normalize( Out.eye );

	return Out;
}

float4 ps_coc( VS_OUTPUT In ) : SV_Target {
	float3 L = -In.light.xyz; //ライトベクトル
	float3 N = normalize( In.N ); //法線ベクトル
	float3 V = normalize( mEye.xyz - In.X ); //視線ベクトル
	float3 H = normalize( L + V ); //ハーフベクトル

	//計算に使うそれぞれの角度
	float NV = dot( N, V );
	float NH = dot( N, H );
	float VH = dot( V, H );
	float NL = dot( N, L );
	float LH = dot( L, H );

	const float m = 0.45f;//粗さ
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

	float4 color = float4( 1, 1, 1, 1 );
	if( (int)mDrawType.x == WALL){ //BumpMapを使います
		color = In.col * tex_0.Sample( texSamp_0, In.tex );
	}
	if( (int)mDrawType.x == ARM ){//通常描画
		float3 reflect_vec = reflect( In.eye, In.N );
		color = In.col * envMap.Sample( texSamp_0, reflect_vec );
		color = saturate( 1.0f * color + 0.2 );
	}
	if( (int)mDrawType.x == BALL ){ //tex2枚で描画
		float4 texCol_0, texCol_1;
		texCol_0 = tex_0.Sample( texSamp_0, In.tex );
		texCol_1 = tex_1.Sample( texSamp_1, In.tex );
		color = texCol_0 * texCol_1;
	}

	//金属の色
	float4 ks = { mDiffuse.x*0.486f, mDiffuse.y*0.433f, mDiffuse.z*0.450f, 1.0f };

	color.w = mDiffuse.w;

	return color * mDiffuse
			+ ks * max( 0, F * D * G / NV );
}