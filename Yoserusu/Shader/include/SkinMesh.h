#ifndef INCLUDE_MASHIRO_GRAPHICS_SHADER_SKIN_MESH_H_
#define INCLUDE_MASHIRO_GRAPHICS_SHADER_SKIN_MESH_H_

#include "Mashiro/Shader/src/ShaderBase.h"

#include "Mashiro/Graphics/Vertex.h"

using namespace Mashiro::Shader;

namespace Mashiro {

namespace Graphics {

class SkinMesh : public ShaderBase{
public:
	SkinMesh(ID3D10Device1* dev ){
		compile( dev );
	}
	~SkinMesh(){
	}
	void compile( ID3D10Device1* device ){
		//���_Desc
		D3D10_INPUT_ELEMENT_DESC desc[] = {
			{ "POSITION",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 }, //0
			{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }, //12
			{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }, //12
			{ "BONE_INDEX",	 0, DXGI_FORMAT_R32G32B32A32_UINT,  0, 32, D3D10_INPUT_PER_VERTEX_DATA, 0 }, //8
			{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D10_INPUT_PER_VERTEX_DATA, 0 }, //16
		};

		//�錾
		HRESULT hr = S_OK;
		unsigned int size = sizeof( desc ) / sizeof( desc[ 0 ] );
		const char* file = "hlsl/SkinMesh.fx";
		
		//���_�V�F�[�_�R���p�C��
		if( FAILED( hr = compileVSFromFile( device, "hlsl/SkinMesh.fx", "vs_SkinMesh", desc, size ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Vertex�V�F�[�_�̃R���p�C�����s" );
		}

		//�s�N�Z���V�F�[�_�R���p�C��
		if( FAILED( hr = compilePSFromFile( device, file, "ps_SkinMesh" ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Pixel�V�F�[�_�̃R���p�C�����s" );
		}

		if( FAILED( createConstantBuffer( device, sizeof( FbxSkinMeshConstant ) ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "ConstantBuffer�̃R���p�C�����s" );
		}
	}

	//HRESULT setConstantBuffer( const ShaderParameter& parameter ){
	//	FbxSkinMeshConstant* buffer = NULL;

	//	//�R���X�^���g�o�b�t�@�ւ̃|�C���^���擾
	//	if( SUCCEEDED( lock(( void** )&buffer ) ) ){
	//		buffer->mProj = parameter.mProjection;
	//		buffer->mView = parameter.mView;
	//		buffer->mWorld = parameter.mWorld;
	//		buffer->mDiffuse = parameter.mDiffuse;
	//		buffer->mLight = parameter.mLight;
	//		memcpy( buffer->mBoneData, parameter.mBoneData, sizeof( parameter.mBoneData ) ); 
	//		//�}�b�s���O�I��
	//		unLock();
	//	}

	//	return S_OK;
	//}
};

}
}

#endif