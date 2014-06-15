#ifndef INCLUDE_SHADER_FEADBACK_H_
#define INCLUDE_SHADER_FEADBACK_H_

#include "Mashiro/Graphics/GraphicsManager.h"

#include "Mashiro/Graphics/Enum.h"
#include "Mashiro/Graphics/Shader.h"

#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Math/Matrix.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Math;

//CocTransシェーダ
class FeadBack {
public:
	FeadBack() {
		create();
	}
	~FeadBack(){
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
		file.mFileName = "Shader/hlsl/FeadBack.fx";
		file.mVSFunc = "vs_feadback";
		file.mPSFunc = "ps_feadback";
		//シェーダ生成
		mShader = Shader::create(file, desc, 6, 0 );	
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
	}

private:
	Mashiro::Graphics::Shader mShader;
};


#endif