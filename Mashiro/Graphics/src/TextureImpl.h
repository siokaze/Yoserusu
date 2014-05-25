#ifndef INCLUDE_MASHIRO_GRAPHICS_TEXTURE_IMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_TEXTURE_IMPL_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include "Mashiro/Graphics/src/Release.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/Texture.h"

namespace Mashiro{
namespace Graphics {

class Texture::Impl : public Mashiro::ReferenceType {
public:
	//テクスチャーファイルから生成
	Impl( const char* fileName, ID3D10Device* device ) : 
	mTexture( 0 ),
	mFileName( fileName ),
	mContainerTexture( 0 ){
		create( mFileName, device );
	}
	//空のテクスチャを作成
	Impl( int w, int h, ID3D10Device* device ) :
	mDevice( device ),
	mTexture( 0 ),
	mFileName( 0 ),
	mContainerTexture( 0 ){
		createDxObject( w, h );
	}
	~Impl(){
		if( mDevice ){
			mDevice = 0;
		}
		SafeRelease( &mTexture );
		SafeRelease( &mContainerTexture );
	}
	void create( const char* fileName, ID3D10Device* device ){
		HRESULT hr;

		mLoadInfo.Width = 0;
		mLoadInfo.Height = 0;
		mLoadInfo.Depth = 0;
		mLoadInfo.FirstMipLevel = 0;
		mLoadInfo.MipLevels = D3DX10_DEFAULT;
		mLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		mLoadInfo.Usage = D3D10_USAGE_DEFAULT;
		mLoadInfo.CpuAccessFlags = 0;
		mLoadInfo.MiscFlags = 0;
		mLoadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		mLoadInfo.Filter = D3DX10_FILTER_TRIANGLE;
		mLoadInfo.MipFilter = D3DX10_FILTER_TRIANGLE;
		mLoadInfo.pSrcInfo = NULL;

		hr = D3DX10CreateShaderResourceViewFromFile(
			device,
			fileName,
			&mLoadInfo,
			NULL,
			&mTexture,
			NULL );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "TextureLoad Err" );
		}
	}
	//ミップレベル固定にしときますわ
	void createDxObject( int w, int h ){
		HRESULT hr;
		D3D10_TEXTURE2D_DESC desc;
		memset( &desc, 0, sizeof( desc ) );
		desc.Width = w;
		desc.Height = h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc .Count = 1;
		desc.Usage = D3D10_USAGE_DYNAMIC;
		desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		hr = mDevice->CreateTexture2D( &desc, 0, &mContainerTexture );
	}
	//Char*でLock
	unsigned char* lockChar(){
		D3D10_MAPPED_TEXTURE2D mapped;
		mContainerTexture->Map(
			D3D10CalcSubresource( 0, 0, 1 ),
			D3D10_MAP_WRITE_DISCARD,
			0,
			&mapped );
		return static_cast< unsigned char* >( mapped.pData );
	}
	//lock
	void lock( unsigned** adress, int* pitch, int mplLevel = 0 ){
		D3D10_MAPPED_TEXTURE2D mapped;
		mContainerTexture->Map(
			D3D10CalcSubresource( 0, 0, 1 ),
			D3D10_MAP_WRITE_DISCARD,
			0,
			&mapped );
		*adress = static_cast< unsigned* >( mapped.pData );
		*pitch = mapped.RowPitch;
	}
	void lock( unsigned** adress, int* pitch, int x, int y, int w, int h, int mipLevel = 0 ){
		D3D10_MAPPED_TEXTURE2D mapped;
		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = x + w;
		rect.bottom = y + h;
		mContainerTexture->Map(
			D3D10CalcSubresource( 0, 0, 1 ),
			D3D10_MAP_WRITE_DISCARD,
			0,
			&mapped );
		*adress = static_cast< unsigned* >( mapped.pData );
		*pitch = mapped.RowPitch;
	}
	//Unlock
	void unLock(){
		mContainerTexture->Unmap( D3D10CalcSubresource( 0, 0, 1 ) );
		//シェーダリソースビューに送る
		mDevice->CreateShaderResourceView( mContainerTexture, 0, &mTexture );
	}

	D3DX10_IMAGE_LOAD_INFO mLoadInfo;
	ID3D10ShaderResourceView* mTexture;
	const char* mFileName;
	ID3D10Device* mDevice;
private:
	//テクスチャー容器
	ID3D10Texture2D* mContainerTexture;
};

}
}

#endif
