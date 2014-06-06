#ifndef INCLUDE_SHADER_COCTRANS_H_
#define INCLUDE_SHADER_COCTRANS_H_

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
		int mDrawType;
		Matrix mWorldInv;
		Vector3 mDummy;
	};
public:
	static CocTrans* instance(){
		static CocTrans mInstance;
		return &mInstance;
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

	Mashiro::Math::Vector4 worldLight( const Matrix& world ){
		Matrix mat = world;
		mat.setInverse( mat );
		Vector4 v = mat.translation( mLight, mat );
		v.normalize();
		v.w = -0.3f;// 環境光の強さ
		return v;
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

	Mashiro::Math::Vector4 mLight;

	Mashiro::Graphics::Shader mShader;

private:
	CocTrans() : mLight( -0.577f, -0.577f, -0.577f, 0.f ){
		create();
	}

	~CocTrans(){
	}
};


#endif