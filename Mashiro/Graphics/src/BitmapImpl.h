#ifndef INCLUDE_MASHIRO_GRAPHICS_BITMAP_IMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_BITMAP_IMPL_H_

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <vector>

#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Graphics/Bitmap.h"
#include "Mashiro/Graphics/src/Release.h"
using namespace Mashiro::Math;

namespace Mashiro{

namespace Graphics {
class Bitmap::Impl : public Mashiro::ReferenceType{
public:
	Impl( const char* fileName, ID2D1RenderTarget* backBuffer, IWICImagingFactory* image ) 
		: mBitmap( 0 ), mDecoder( 0 ), mConverter( 0 ){
		HRESULT hr = S_OK;
		//�ꉞ���
		SafeRelease(&mBitmap);
		//�C���[�W�f�R�[�h
		mBitmap = Decoder( fileName, backBuffer, image );

		//�C���[�W�̃T�C�Y
		D2D1_SIZE_U size = mBitmap->GetPixelSize();
		mImageSize.x = static_cast< float >( size.width );
		mImageSize.y = static_cast< float >( size.height );

		if( NULL == mBitmap ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "�r�b�g�}�b�v�̐����Ɏ��s" );
		}
	}
	Impl( int width, int height, long stride, ID2D1RenderTarget* backBuffer )
		: mWidth(width), mHeight(height), mBitmap( 0 ), mDecoder( 0 ), mConverter( 0 ){
		HRESULT hr = S_OK;
		//�ꉞ���
		SafeRelease(&mBitmap);
		//�T�C�Y�擾
		mImageSize.x = static_cast< float >( width );
		mImageSize.y = static_cast< float >( height );

		D2D1_SIZE_U size = { width, height };

		hr = backBuffer->CreateBitmap( size, 
			D2D1::BitmapProperties( D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE) ),
            &mBitmap );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "�r�b�g�}�b�v�̐����Ɏ��s" );
		}
	}
	//unsigned char*����
	void copyFromMemory( unsigned char* byte ){
		HRESULT hr = S_OK;
		hr = mBitmap->CopyFromMemory( NULL, byte, mWidth * sizeof(long) );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "�r�b�g�}�b�v�̐����Ɏ��s" );
		}
	}
	~Impl(){
		SafeRelease(&mBitmap);
		SafeRelease(&mDecoder);
		SafeRelease(&mConverter);
	}
	ID2D1Bitmap* Decoder(const char* fileName, ID2D1RenderTarget* backBuffer, IWICImagingFactory* image){		
		//SafeRelease(&mDecoder);
		//SafeRelease(&mConverter);
		HRESULT hr = S_OK;
		//const char* �� LPWSTR�ɕϊ�
		int size = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, fileName, strlen(fileName), NULL, 0 );
		
		std::vector< wchar_t > file( size );
		
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED, fileName, strlen(fileName), &file[ 0 ], size );
		
		std::wstring wstrValue( file.begin(), file.begin() + file.size() );
		
		//Decoder��LPWSTR file������
		if( FAILED( hr = image->CreateDecoderFromFilename( 
			wstrValue.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &mDecoder ) ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Bitmap Create FileName Error" );
		}

		//�t���[���擾
		IWICBitmapFrameDecode* frame;
		mDecoder->GetFrame( 0, &frame );
		//�R���o�[�^��Direct2D�p�t�H�[�}�b�g�ɕϊ�
		IWICFormatConverter* convert;
		image->CreateFormatConverter( &convert );
		if( hr = convert->Initialize( frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut)){
			mDecoder->Release();
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "Bitmap Create FileName Error" );
		}
		ID2D1Bitmap* bitmap;
		backBuffer->CreateBitmapFromWicBitmap( convert, NULL, &bitmap );

		return bitmap;
	}
	ID2D1Bitmap* bitmap(){
		return mBitmap;
	}
	D2D1_SIZE_U imageSize(){
		D2D1_SIZE_U a = { 
			static_cast< UINT32 >( mImageSize.x ),
			static_cast< UINT32 >( mImageSize.y ) };
		return a;
	}
	ID2D1Bitmap* mBitmap; //�摜�C���^�[�t�F�C�X
	Vector2 mImageSize;
	LONG mStride; //	�X�g���C�h
	IWICBitmapDecoder* mDecoder;
	IWICFormatConverter* mConverter;

	int mWidth;
	int mHeight;
};

}
}

#endif

//windowscodecs.lib
