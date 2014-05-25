#ifndef INCLUDE_MASHIRO_GAME_SHADER_BASE_H_
#define INCLUDE_MASHIRO_GAME_SHADER_BASE_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include "Mashiro/Graphics/Enum.h"

namespace Mashiro{

namespace Shader {

class ShaderBase {
public:
	ShaderBase();
	virtual ~ShaderBase();

	//シェーダのコンパイル実施
	virtual void compile( ID3D10Device1* ) = 0;

	//コンスタントバッファへのマッピング
	HRESULT lock( void** cb );

	//コンスタントバッファへのマッピング終わり
	void unLock();

	//シェーダセット
	void setShader( ID3D10Device1* device );

protected:
	//vertexShaderコンパイル
	HRESULT compileVSFromFile( ID3D10Device1* device, const char* fileName, const char* funcName, D3D10_INPUT_ELEMENT_DESC layout[], UINT numElements );
	//pixelShaderコンパイル
	HRESULT compilePSFromFile( ID3D10Device1* device, const char* fileName, const char* funcName );
	//GeometryShaderコンパイル
	HRESULT compileGSFromFile( ID3D10Device1* device, const char* fileName, const char* funcName );
	//コンスタントバッファ作成
	HRESULT createConstantBuffer( ID3D10Device1* device, UINT bufferSize );

private:
	ID3D10VertexShader* mVertexShader;
	ID3D10PixelShader* mPixelShader;
	ID3D10GeometryShader* mGeometryShader;
	ID3D10Buffer* mConstantBuffer;
	ID3D10InputLayout* mInputLayout;

};

}
}


#endif