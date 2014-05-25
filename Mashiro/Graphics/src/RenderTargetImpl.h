#ifndef INCLUDE_MASHIRO_GRAPHICS_RENDERTARGET_IMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_RENDERTARGET_IMPL_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/RenderTarget.h"
#include "Mashiro/Graphics/src/Release.h"

namespace Mashiro{
namespace Graphics{

class RenderTarget::Impl : public Mashiro::ReferenceType {
public:
	Impl( int w, int h, RenderFormat textureFormat, ID3D10Device1* device ) : 
	mWidth( w ),
	mHeight( h ),
	mTextureFormat( textureFormat ),
	mTexture( 0 ),
	mSrv( 0 ),
	mRtv( 0 ){
		createRenderTarget( device );
	}
	~Impl(){
		SafeRelease( &mTexture );
		SafeRelease( &mRtv );
		SafeRelease( &mSrv );
	}
	//レンダリングターゲットの作成
	void createRenderTarget( ID3D10Device1* device ) {
		HRESULT hr = S_OK;
		D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		D3D10_RENDER_TARGET_VIEW_DESC RTVDesc;
		D3D10_TEXTURE2D_DESC desc;
		ZeroMemory( &desc, sizeof( desc ) );
		ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
		ZeroMemory( &RTVDesc, sizeof( RTVDesc ) );

		//	レンダリングターゲット用のテクスチャを生成	
		desc.Width = mWidth;						//	テクスチャ横幅サイズ
		desc.Height = mHeight;						//	テクスチャ縦幅サイズ
		desc.MipLevels = 1;						//	ミップマップレベル
		desc.ArraySize = 1;						//	テクスチャ配列数
		desc.Format = (DXGI_FORMAT)mTextureFormat;	//	テクスチャフォーマット(フローティングバッファ)
		desc.SampleDesc.Count = 1;				//	マルチサンプリング数
		desc.SampleDesc.Quality = 0;			//	サンプリング品質
		desc.Usage = D3D10_USAGE_DEFAULT;		//	アクセス方法
		desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;		//	バインド
		desc.CPUAccessFlags = 0;				//	CPUアクセス許可
		desc.MiscFlags = 0;						//	オプション

		//	レンダリング対象となるテクスチャの生成
		if( FAILED( hr = device->CreateTexture2D( &desc, NULL, &mTexture )))
		{
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "テクスチャーターゲットの作成失敗" );
		}

		//	レンダリングターゲットビューの作成
		RTVDesc.Format = desc.Format;
		RTVDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
		RTVDesc.Texture2D.MipSlice = 0;
		if( FAILED( hr = device->CreateRenderTargetView( mTexture, &RTVDesc, &mRtv )))
		{
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "レンダリングターゲットの作成失敗" );
		}

		//	シェーダーリソースビューの作成
		ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
		SRVDesc.Format =desc.Format;
		SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		if( FAILED(device->CreateShaderResourceView(mTexture,&SRVDesc,&mSrv)))
		{
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "シェーダリソースビューの作成失敗" );
		}
	}
	void targetTexture( const char* name ){
		D3DX10SaveTextureToFile( mTexture, D3DX10_IFF_PNG, name );
	}

	int mWidth;
	int mHeight;
	RenderFormat mTextureFormat;
	RenderFormat mDepthForamt;

	ID3D10Texture2D* mTexture;	//テクスチャ
	ID3D10RenderTargetView*	mRtv; //レンダリングターゲットビュー
	ID3D10ShaderResourceView* mSrv;	//シェーダーリソースビュー
};

}
}

#endif