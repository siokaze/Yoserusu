#ifndef INCLUDE_MASHIRO_GRAPHICS_SHADER_H_
#define INCLUDE_MASHIRO_GRAPHICS_SHADER_H_

#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/Shader.h"
#include "Mashiro/Shader/src/ShaderBase.h"

#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Shader/hlsl/CocTrans.h"
#include "Mashiro/Shader/hlsl/Effect.h"
#include "Mashiro/Shader/hlsl/Fog.h"
#include "Mashiro/Shader/hlsl/Outline.h"
#include "Mashiro/Shader/hlsl/SkinMesh.h"
#include "Mashiro/Shader/hlsl/Toon.h"
#include "Mashiro/Shader/hlsl/DefferdPost.h"
#include "Mashiro/Shader/hlsl/DefferdShading.h"
#include "Mashiro/Shader/hlsl/ShadowMap.h"
#include "Mashiro/Shader/hlsl/ShadowScene.h"
#include "Mashiro/Shader/hlsl/Lambert.h"
#include "Mashiro/Shader/hlsl/Canvas.h"
#include "Mashiro/Shader/hlsl/CircleShadow.h"
using namespace Mashiro::Shader;


namespace Mashiro{
namespace Graphics{

class Shader::Impl : public Mashiro::ReferenceType {
public:
	Impl( ShaderType type, ID3D10Device1* device ) : 
	mShaderType( type ){
		createShader( device );
	}
	~Impl(){
		SAFE_DELETE( mShader );
	}
	void createShader( ID3D10Device1* device ){
		//HRESULT hr;
		//シェーダの生成
		switch( mShaderType ){
			case SHADER_COCTRANS: mShader = NEW CocTrans( device ); break;
			case SHADER_EFFECT: mShader = NEW Effect( device ); break;
			case SHADER_FOG: mShader = NEW Fog( device ); break;
			case SHADER_OUTLINE: mShader = NEW Outline( device ); break;
			case SHADER_TOON: mShader = NEW Toon( device ); break;
			case SHADER_SKINMESH: mShader = NEW SkinMesh( device ); break;
			case SHADER_POST: mShader = NEW DefferdPost( device ); break;
			case SHADER_SHADING: mShader = NEW DeffedShading( device ); break;
			case SHADER_SHADOW_MAP: mShader = NEW ShadowMap( device ); break;
			case SHADER_SHADOW_SCENE: mShader = NEW ShadowScene( device ); break;
			case SHADER_LAMBERT: mShader = NEW Lambert( device ); break;
			case SHADER_CANVAS: mShader = NEW Canvas( device ); break;
			case SHADER_CIRCLE_SHADOW: mShader = NEW CircleShadow( device ); break;
		}
	}
	bool lock( void** cb ){
		if( mShader->lock( cb ) == S_OK ){
			return true;
		}
		return false;
	}
	void unlock(){
		mShader->unLock();
	}

	ShaderBase* mShader;
	ShaderType mShaderType;
};

}
}

#endif