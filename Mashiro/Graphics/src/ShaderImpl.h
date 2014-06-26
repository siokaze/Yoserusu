#ifndef INCLUDE_MASHIRO_GRAPHICS_SHADER_H_
#define INCLUDE_MASHIRO_GRAPHICS_SHADER_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include <d3dcompiler.h>

#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/Shader.h"
#include "Mashiro/Graphics/ShaderDesc.h"
#include "Mashiro/Graphics/src/Release.h"

#include <fstream>
using namespace std;

namespace Mashiro{
namespace Graphics{

class Shader::Impl : public Mashiro::ReferenceType {
public:
	Impl( ShaderFile shaderFile, ElementDesc desc[], unsigned int size, size_t constantBufferSize, ID3D10Device1* device ) :
	mVertexShader(),
	mPixelShader(),
	mGeometryShader(),
	mInputLayout(),
	mConstantBuffer(),
	mShaderFile( shaderFile ),
	mBufferSize( constantBufferSize ){
		createShader( device, desc, size );
	}
	~Impl(){
		
	}
	void createShader( ID3D10Device1* device, ElementDesc myDesc[], unsigned int size ){
		HRESULT hr = S_OK;
		//ElementDesc�̍\�z
		D3D10_INPUT_ELEMENT_DESC* desc = NEW D3D10_INPUT_ELEMENT_DESC[ size ];
		for( int i = 0; i < size; ++i ){
			ElementDesc d = myDesc[ i ];
			D3D10_INPUT_ELEMENT_DESC value = { d.mElementName, 0, (DXGI_FORMAT)d.mFromat, 0, d.mOffset, D3D10_INPUT_PER_VERTEX_DATA, 0 };
			desc[ i ] = value;
		}

		//�e�V�F�[�_�̃R���p�C��
		if( mShaderFile.mVSFunc ){
			hr = compileVSFromFile( device, desc, size );
			if( FAILED ( hr ) ){
				STRONG_ASSERT( hr != E_FAIL && "VertexShader not create" );
			}
		}
		if( mShaderFile.mPSFunc ){
			hr = compilePSFromFile( device );
			if( FAILED ( hr ) ){
				STRONG_ASSERT( hr != E_FAIL && "PixelShader not create" );
			}
		}
		if( mShaderFile.mGSFunc ){
			hr = compileGSFromFile( device );
			if( FAILED ( hr ) ){
				STRONG_ASSERT( hr != E_FAIL && "GeometryShader not create" );
			}
		}
		if( mBufferSize ){
			hr = createConstantBuffer( device, mBufferSize );
			if( FAILED ( hr ) ){
				STRONG_ASSERT( hr != E_FAIL && "ConstantBuffer not create" );
			}
		}
		SAFE_DELETE_ARRAY( desc );
	}

	HRESULT compileVSFromFile( ID3D10Device1* device, D3D10_INPUT_ELEMENT_DESC layout[], UINT numElements ){
		ID3D10Blob* shader=NULL;
		ID3D10Blob* errors=NULL;
		HRESULT hr = S_OK;

		//�V�F�[�_�[�t�@�C���̃R���p�C��
		if( FAILED( hr = D3DX10CompileFromFile( mShaderFile.mFileName, NULL, NULL, mShaderFile.mVSFunc, "vs_4_0", 0, 0, NULL, &shader, &errors, NULL ) ) ){
			char* c = (char*)errors->GetBufferPointer();
			//�G���[���O�o���܂�
			ofstream file;
			file.open( "errorVertexLog.txt" );
			file << c << endl;
			file.close();
			return E_FAIL;
		}
		SafeRelease(&errors);

		//VertexShader�𐶐�
		hr = device->CreateVertexShader( shader->GetBufferPointer(), shader->GetBufferSize(), &mVertexShader );
		if( FAILED( hr ) ){
			SafeRelease(&shader);
			return E_FAIL;
		}

		//���_�C���v�b�g���C�A�E�g���쐬
		hr = device->CreateInputLayout( layout, numElements, shader->GetBufferPointer(), shader->GetBufferSize(), &mInputLayout );
		if( FAILED( hr ) ){
			return E_FAIL;
		}
		SafeRelease( &shader );
		return S_OK;
	}
	HRESULT compilePSFromFile( ID3D10Device1* device ){
		ID3D10Blob* shader=NULL;
		ID3D10Blob* errors=NULL;

		//�V�F�[�_�[�t�@�C���̃R���p�C��
		if( FAILED( D3DX10CompileFromFile( mShaderFile.mFileName, NULL, NULL, mShaderFile.mPSFunc, "ps_4_0", 0, 0, NULL, &shader, &errors, NULL ) ) ){
			char* c = (char*)errors->GetBufferPointer();
			//�G���[���O�o���܂�
			ofstream file;
			file.open( "errorPixelLog.txt" );
			file << c << endl;
			file.close();
			return E_FAIL;
		}
		SafeRelease(&errors);

		//PixelShader�𐶐�
		if( FAILED( device->CreatePixelShader( shader->GetBufferPointer(), shader->GetBufferSize(), &mPixelShader ) ) ){
			SafeRelease(&shader);
			return E_FAIL;
		}

		SafeRelease( &shader );
		return S_OK;
	}
	HRESULT compileGSFromFile( ID3D10Device1* device ){
		ID3D10Blob* shader=NULL;
		ID3D10Blob* errors=NULL;

		//�V�F�[�_�[�t�@�C���̃R���p�C��
		if( FAILED( D3DX10CompileFromFile( mShaderFile.mFileName, NULL, NULL, mShaderFile.mGSFunc, "gs_4_0", 0, 0, NULL, &shader, &errors, NULL ) ) ){
			char* c = (char*)errors->GetBufferPointer();
			//�G���[���O�o���܂�
			ofstream file;
			file.open( "errorGeometryLog.txt" );
			file << c << endl;
			file.close();
			return E_FAIL;
		}
		SafeRelease(&errors);

		//GeometryShader�𐶐�
		if( FAILED( device->CreateGeometryShader( shader->GetBufferPointer(), shader->GetBufferSize(), &mGeometryShader ) ) ){
			SafeRelease(&shader);
			return E_FAIL;
		}

		SafeRelease( &shader );
		return S_OK;
	}
	HRESULT createConstantBuffer( ID3D10Device1* device, UINT bufferSize ){
		D3D10_BUFFER_DESC cb;
		cb.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = bufferSize;
		cb.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.Usage = D3D10_USAGE_DYNAMIC;

		HRESULT hr = E_FAIL;

		//�R���X�^���g�o�b�t�@���쐬
		if( FAILED( hr = device->CreateBuffer( &cb, NULL, &mConstantBuffer ) ) ){
			return E_FAIL;
		}
		return S_OK;
	}
	void setShader( ID3D10Device1* device ){
		//�e�V�F�[�_���Z�b�g
		if( NULL != mVertexShader ){
			device->VSSetShader( mVertexShader );
		}
		if( NULL != mGeometryShader ){
			device->GSSetShader( mGeometryShader );
		}
		if( NULL != mPixelShader ){
			device->PSSetShader( mPixelShader );
		}

		//�R���X�^���g�o�b�t�@���Z�b�g
		if( NULL != mConstantBuffer ){
			device->VSSetConstantBuffers( 1, 1, &mConstantBuffer );
		}
		if( NULL != mConstantBuffer ){
			device->GSSetConstantBuffers( 1, 1, &mConstantBuffer );
		}
		if( NULL != mConstantBuffer ){
			device->PSSetConstantBuffers( 1, 1, &mConstantBuffer );
		}

		//���_���C�A�E�g�Z�b�g
		if( NULL != mInputLayout ){
			device->IASetInputLayout( mInputLayout );
		}
	}
	bool lock( void** cb ){
		if( mConstantBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, cb ) == S_OK ){
			return true;
		}
		return false;
	}

	void unLock(){
		mConstantBuffer->Unmap();
	}

	ShaderFile mShaderFile;
	size_t mBufferSize;

	//�V�F�[�_�t�@�C���S
	ID3D10VertexShader* mVertexShader;
	ID3D10PixelShader* mPixelShader;
	ID3D10GeometryShader* mGeometryShader;
	ID3D10Buffer* mConstantBuffer;
	ID3D10InputLayout* mInputLayout;
};

}
}

#endif