#ifndef INCLUDE_SHADER_COCTRANS_H_
#define INCLUDE_SHADER_COCTRANS_H_

#include "Mashiro/Graphics/GraphicsManager.h"

#include "Mashiro/Graphics/Enum.h"
#include "Mashiro/Graphics/Shader.h"

#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Matrix.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Math;

//CocTransシェーダ
class CocTrans {
public:
	enum Type {
		TYPE_BALL,
		TYPE_ARM,
		TYPE_WALL,
	};
	struct ConstantBuffer{
		Vector4 mDrawType;
		Matrix mWorldInv;
	};
public:
	CocTrans() : mLight( -0.577f, -0.577f, -0.577f, 0.f ), mEye( -0.57f, -0.57f, -0.57f, 1.f ){
		create();
	}

	~CocTrans(){

	}
	//シェーダ生成
	void create(){
		ElementDesc desc[] = {
			{ "POSITION", FORMAT_R32G32B32_FLOAT, 0 },
			{ "NORMAL", FORMAT_R32G32B32_FLOAT, 16 },
			{ "COLOR", FORMAT_R32G32B32A32_FLOAT, 28 },
			{ "TEXCOORD", FORMAT_R32G32_FLOAT, 44 },
			{ "BINORMAL", FORMAT_R32G32B32_FLOAT, 52 },
			{ "TANGENT", FORMAT_R32G32B32_FLOAT, 64 },
		};
		ShaderFile file;
		file.mFileName = "Shader/hlsl/CocTrans.fx";
		file.mVSFunc = "vs_coc";
		file.mPSFunc = "ps_coc";
		//シェーダ生成
		mShader = Shader::create(file, desc, 6, sizeof( ConstantBuffer ) );	
	}

	void worldLight( const Matrix& world ){
		Matrix mat = world;
		mat.setInverse( mat );
		Vector4 v = mat.translation( Vector4( 0.f, 100.f, 50.0f, 1.f ), mat );
		v.normalize();
		v.w = -0.7f;// 環境光の強さ
		mLight = v;
	}

	void worldViewEye( const Matrix& wp ){
		Matrix mat = wp;
		mat.setInverse( mat );
		mEye = mat.translation( Vector4( 0.0f, 0.f, -50.f, 1.f ), mat );
		mEye.normalize();
	}

	bool lock( void** cb ){
		if( mShader.lock( cb ) ){
			return true;
		}
		return false;
	}

	void unLock(){
		mShader.unlock();
	}
	 
	Mashiro::Graphics::Shader shader(){
		return mShader;
	}

	void setShader(){
		Graphics::Manager::instance().setShader( mShader );
		Graphics::Manager::instance().setEyePos( mEye );
		Graphics::Manager::instance().setLight( mLight );
		Graphics::Manager::instance().setDiffuse( Vector4( 1.0 ) );
	}

public:
	Mashiro::Math::Vector4 mLight;
	Mashiro::Math::Vector4 mEye;
	Mashiro::Graphics::Shader mShader;

};


#endif