#ifndef INCLUDE_SHADER_CIRCLE_SHADOW_H_
#define INCLUDE_SHADER_CIRCLE_SHADOW_H_

#include "Mashiro/Shader/src/ShaderBase.h"

#include "Mashiro/Graphics/Vertex.h"

using namespace Mashiro::Shader;

namespace Mashiro {

namespace Graphics {

//CocTrans�V�F�[�_
class CircleShadow : public ShaderBase {
public:
	CircleShadow( ID3D10Device1* dev ){
		compile( dev );
	}
	~CircleShadow(){
	}
	//�R���p�C��
	void compile( ID3D10Device1* device ) {
		//���_Desc
		D3D10_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 44, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		};

		//�錾
		HRESULT hr = S_OK;
		unsigned int size = sizeof( desc ) / sizeof( desc[ 0 ] );
		const char* file = "hlsl/CircleShadow.fx";
		
		//���_�V�F�[�_�R���p�C��
		if( FAILED( hr = compileVSFromFile( device, file, "vs_shadow", desc, size ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Vertex�V�F�[�_�̃R���p�C�����s" );
		}

		//�s�N�Z���V�F�[�_�R���p�C��
		if( FAILED( hr = compilePSFromFile( device, file, "ps_shadow" ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Pixel�V�F�[�_�̃R���p�C�����s" );
		}

		if( FAILED( createConstantBuffer( device, sizeof( circleShadowConstantBuffer ) ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "ConstantBuffer�̃R���p�C�����s" );
		}

	}
};

}
}

#endif