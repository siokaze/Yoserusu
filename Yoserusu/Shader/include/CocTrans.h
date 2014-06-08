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
	CocTrans() : mLight( -0.577f, -0.577f, -0.577f, 0.f ){
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
		Vector4 v = mat.translation( Vector4( -0.577f, -0.577f, -0.577f, 0.f ), mat );
		v.normalize();
		v.w = -0.3f;// 環境光の強さ
		mLight = v;
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
		Graphics::Manager::instance().setLight( mLight );
	}

private:
	Mashiro::Math::Vector4 mLight;

	Mashiro::Graphics::Shader mShader;

};


#endif