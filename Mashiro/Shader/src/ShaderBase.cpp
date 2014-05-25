#include "Mashiro/Shader/src/ShaderBase.h"
#include "Mashiro/Graphics/src/Release.h"
#include <fstream>
using namespace std;
using namespace Mashiro::Graphics;
namespace Mashiro{

namespace Shader {

ShaderBase::ShaderBase() :
mConstantBuffer( 0 ),
mVertexShader( 0 ),
mGeometryShader( 0 ),
mPixelShader( 0 ),
mInputLayout( 0 ){
}

ShaderBase::~ShaderBase(){
	SafeRelease( &mConstantBuffer );
	SafeRelease( &mVertexShader );
	SafeRelease( &mPixelShader );
	SafeRelease( &mGeometryShader );
	SafeRelease( &mInputLayout );
}

HRESULT ShaderBase::compileVSFromFile( ID3D10Device1* device, const char* fileName, const char* funcName, D3D10_INPUT_ELEMENT_DESC layout[],UINT numElements ){
	ID3D10Blob* shader=NULL;
	ID3D10Blob* errors=NULL;
	HRESULT hr = S_OK;

	//シェーダーファイルのコンパイル
	if( FAILED( hr = D3DX10CompileFromFile( fileName, NULL, NULL, funcName, "vs_4_0", 0, 0, NULL, &shader, &errors, NULL ) ) ){
		char* c = (char*)errors->GetBufferPointer();
		//エラーログ出します
		ofstream file;
		file.open( "errorVertexLog.txt" );
		file << c << endl;
		file.close();
		return E_FAIL;
	}
	SafeRelease(&errors);

	//VertexShaderを生成
	if( FAILED( device->CreateVertexShader( shader->GetBufferPointer(), shader->GetBufferSize(), &mVertexShader ) ) ){
		SafeRelease(&shader);
		return E_FAIL;
	}

	//頂点インプットレイアウトを作成
	if( FAILED( hr = device->CreateInputLayout( layout, numElements, shader->GetBufferPointer(), shader->GetBufferSize(), &mInputLayout ) ) ){
		return E_FAIL;
	}
	SafeRelease( &shader );
	return S_OK;
}

HRESULT ShaderBase::compilePSFromFile( ID3D10Device1* device, const char* fileName, const char* funcName ){
	ID3D10Blob* shader=NULL;
	ID3D10Blob* errors=NULL;

	//シェーダーファイルのコンパイル
	if( FAILED( D3DX10CompileFromFile( fileName, NULL, NULL, funcName, "ps_4_0", 0, 0, NULL, &shader, &errors, NULL ) ) ){
		char* c = (char*)errors->GetBufferPointer();
		//エラーログ出します
		ofstream file;
		file.open( "errorPixelLog.txt" );
		file << c << endl;
		file.close();
		return E_FAIL;
	}
	SafeRelease(&errors);

	//PixelShaderを生成
	if( FAILED( device->CreatePixelShader( shader->GetBufferPointer(), shader->GetBufferSize(), &mPixelShader ) ) ){
		SafeRelease(&shader);
		return E_FAIL;
	}

	SafeRelease( &shader );
	return S_OK;
}

HRESULT ShaderBase::compileGSFromFile( ID3D10Device1* device, const char* fileName, const char* funcName ){
	ID3D10Blob* shader=NULL;
	ID3D10Blob* errors=NULL;

	//シェーダーファイルのコンパイル
	if( FAILED( D3DX10CompileFromFile( fileName, NULL, NULL, funcName, "gs_4_0", 0, 0, NULL, &shader, &errors, NULL ) ) ){
		char* c = (char*)errors->GetBufferPointer();
		//エラーログ出します
		ofstream file;
		file.open( "errorGeometryLog.txt" );
		file << c << endl;
		file.close();
		return E_FAIL;
	}
	SafeRelease(&errors);

	//GeometryShaderを生成
	if( FAILED( device->CreateGeometryShader( shader->GetBufferPointer(), shader->GetBufferSize(), &mGeometryShader ) ) ){
		SafeRelease(&shader);
		return E_FAIL;
	}

	SafeRelease( &shader );
	return S_OK;
}

HRESULT ShaderBase::createConstantBuffer( ID3D10Device1* device, UINT bufferSize ){
	D3D10_BUFFER_DESC cb;
	cb.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = bufferSize;
	cb.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D10_USAGE_DYNAMIC;

	HRESULT hr = E_FAIL;

	//コンスタントバッファを作成
	if( FAILED( hr = device->CreateBuffer( &cb, NULL, &mConstantBuffer ) ) ){
		return E_FAIL;
	}
	return S_OK;
}

void ShaderBase::setShader( ID3D10Device1* device ){
	//各シェーダをセット
	if( NULL != mVertexShader ){
		device->VSSetShader( mVertexShader );
	}
	if( NULL != mGeometryShader ){
		device->GSSetShader( mGeometryShader );
	}
	if( NULL != mPixelShader ){
		device->PSSetShader( mPixelShader );
	}

	//コンスタントバッファをセット
	if( NULL != mConstantBuffer ){
		device->VSSetConstantBuffers( 1, 1, &mConstantBuffer );
	}
	if( NULL != mConstantBuffer ){
		device->GSSetConstantBuffers( 1, 1, &mConstantBuffer );
	}
	if( NULL != mConstantBuffer ){
		device->PSSetConstantBuffers( 1, 1, &mConstantBuffer );
	}

	//頂点レイアウトセット
	if( NULL != mInputLayout ){
		device->IASetInputLayout( mInputLayout );
	}
}

HRESULT ShaderBase::lock( void** cb ){
	HRESULT hr = S_OK;
	if( FAILED( hr = mConstantBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, cb ) ) ){
		return E_FAIL;
	}
	return S_OK;
}

void ShaderBase::unLock(){
	mConstantBuffer->Unmap();
}

}
}
