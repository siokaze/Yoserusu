#ifndef INCLUDE_MASHIRO_GRAPHICS_SPRITEIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_SPRITEIMPL_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Graphics/src/BitmapImpl.h"
#include <vector>

#pragma comment(lib, "d2d1.lib") 
#pragma comment(lib, "dwrite.lib") 

namespace Mashiro{

namespace Graphics{

class SpriteImpl{
public:
	SpriteImpl( IDXGISwapChain* chain ) :  
	mCurrentBitmap( 0 ),
	mColor( 0 ),
	mFactory( 0 ),
	mDWriteFactory( 0 ),
	mBackBuffer( 0 ),
	mRotate(),
	mScale(),
	mRotateFlag(),
	mScaleFlag(){
		  HRESULT hr = S_OK;
		  //Direct2D初期化
		  if( FAILED( hr = createD2DFactory( chain ) ) ){
			  STRONG_ASSERT( hr != E_FAIL && "Direct2Dの初期化に失敗" );
		  }
		  IDXGISurface *pBackBuffer = NULL;
		  if(FAILED(  chain->GetBuffer( 0, IID_PPV_ARGS(&pBackBuffer))))
		  {
			  hr = E_FAIL;
			  STRONG_ASSERT( hr != E_FAIL && "Direct2Dの初期化に失敗" );
		  }

		  CreateDXGIRendertarget(pBackBuffer);

		  //バックバッファからソリッドを作る
		  if( FAILED( hr = mBackBuffer->CreateSolidColorBrush( D2D1::ColorF( 0, 0, 0 ), &mBrush ) ) ){
			  hr = E_FAIL;
			  STRONG_ASSERT( hr != E_FAIL && "Direct2Dの初期化に失敗" );
		  }
	}
	~SpriteImpl(){
		setBitmap( 0 );
		SafeRelease( &mFactory );
		SafeRelease( &mImageFactory );
		SafeRelease( &mDWriteFactory );
		SafeRelease( &mBackBuffer );
		SafeRelease( &mBrush );
	}
	void standby(){
		mBackBuffer->BeginDraw();
	}
	void end(){
		mBackBuffer->EndDraw();
	}
	void setBitmap( Bitmap::Impl* o ){
		if( mCurrentBitmap == o ){
			return;
		}
		if( mCurrentBitmap ){
			mCurrentBitmap->release();
			if( mCurrentBitmap->referenceCount() == 0 ){
				SAFE_DELETE( mCurrentBitmap );
			}
		}
		mCurrentBitmap = o;
		if( o ){
			o->refer();
		}
	}
	void setTrance( float f ){
		mColor.w = f;
	}
	void setColor( const Vector3& color ){
		mColor.set( color.x, color.y, color.z, 1.f );
	}
	void setLine( const Vector2& p0, const Vector2 p1 ){
		D2D1_POINT_2F point0 = { p0.x, p0.y };
		D2D1_POINT_2F point1 = { p1.x, p1.y };
		mPoint0 = point0;
		mPoint1 = point1;
		mSpriteType = SPRITE_LINE;
	}
	void setRectangle( const Vector2& p0, const Vector2& p1 ){
		mRect = D2D1::RectF( p0.x, p0.y, p1.x, p1.y );
		mSpriteType = SPRITE_RECT;
	}
	void setFillRectangle( const Vector2& p0, const Vector2& p1 ){
		mRect = D2D1::RectF( p0.x, p0.y, p1.x, p1.y );
		mSpriteType =SPRITE_FILL_RECT;

	}
	void setEllipse( const Vector2& center, const Vector2& radius ){
		D2D1_POINT_2F c = { center.x, center.y };
		mEllipse = D2D1::Ellipse( c, radius.x, radius.y );
		mSpriteType = SPRITE_ELLIPSE;
	}
	void setFillEllipse( const Vector2& center, const Vector2& radius ){
		D2D1_POINT_2F c = { center.x, center.y };
		mEllipse = D2D1::Ellipse( c, radius.x, radius.y );
		mSpriteType = SPRITE_FILL_ELLIPSE;
	}
	void setBitmapRectangle( const Vector2& p0 ){
		//座標をセット
		D2D_SIZE_U size = mCurrentBitmap->imageSize();
		D2D1_RECT_F rect = { p0.x, p0.y, p0.x + size.width, p0.y + size.height };
		mRect = rect;
		mSpriteType = SPRITE_BITMAP;
	}
	void setBitmapRectangle( const Vector2& pos, const Vector2& size, const Vector4& rectAngle)
	{	
		D2D_RECT_F recta = { rectAngle.x, rectAngle.y, rectAngle.z, rectAngle.w};
		D2D1_RECT_F rect = { pos.x, pos.y, pos.x + size.x, pos.y + size.y };

		mRectAngle = recta;
		mRect = rect;

		mSpriteType = SPRITE_BITMAP_RECT;
	}
	void draw(){
		//描画はじめの処理
		standby();
		//カラ
		mBrush->SetColor( D2D1::ColorF( mColor.x, mColor.y, mColor.z, mColor.w ) );

		D2D1::Matrix3x2F matrix;
		matrix = D2D1::Matrix3x2F::Identity();

		if( mScaleFlag ){
			matrix = matrix * mScale;
			mScaleFlag = false;
		}
		if( mRotateFlag ){
			matrix = matrix * mRotate;
			mRotateFlag = false;
		}

		mBackBuffer->SetTransform( matrix );

		//それぞれの描画方法へ
		switch( mSpriteType ){
		case SPRITE_BITMAP://ビットマップ
			//BitMap設定されてなければアウト
			mBackBuffer->DrawBitmap( mCurrentBitmap->bitmap(), mRect, mColor.w );
			break;
		case SPRITE_BITMAP_RECT://ビットマップ
			//BitMap設定されてなければアウト
			mBackBuffer->DrawBitmap( mCurrentBitmap->bitmap(), mRect, mColor.w, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &mRectAngle);
			break;
		case SPRITE_FILL_RECT://塗りつぶし四角形
			mBackBuffer->FillRectangle( mRect, mBrush );
			break;
		case SPRITE_RECT://枠のみ四角形
			mBackBuffer->DrawRectangle( mRect, mBrush );
			break;
		case SPRITE_ELLIPSE://枠のみ円
			mBackBuffer->DrawEllipse( mEllipse, mBrush );
			break;
		case SPRITE_FILL_ELLIPSE://塗りつぶし円
			mBackBuffer->FillEllipse( mEllipse, mBrush );
			break;
		case SPRITE_LINE://線
			mBackBuffer->DrawLine( mPoint0, mPoint1, mBrush );
			break;
		}
		//描画終わりの処理
		end();
	}

	HRESULT createD2DFactory( IDXGISwapChain* chain ){
		//D2Dファクトリを生成
		if(  FAILED( D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &mFactory ) ) ){
			return E_FAIL;
		}

		if( FAILED( CoCreateInstance( CLSID_WICImagingFactory,NULL, CLSCTX_INPROC_SERVER,
						IID_IWICImagingFactory, reinterpret_cast<void **>( &mImageFactory )) ) )
		{
			return E_FAIL;
		}

		if( FAILED( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED,
			__uuidof( mDWriteFactory ),
			reinterpret_cast< IUnknown ** >( &mDWriteFactory ) ) ) ){

				return E_FAIL;
		}
		return S_OK;
	}

	HRESULT CreateDXGIRendertarget(IDXGISurface* pBackBuffer)
	{
		FLOAT dpiX;
		FLOAT dpiY;
	
		mFactory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(  D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

		//	Direct2D用のレンダリングターゲット
		if( FAILED( mFactory->CreateDxgiSurfaceRenderTarget( pBackBuffer, &props, &mBackBuffer ))){
			return E_FAIL;
		}
		
		SafeRelease(&pBackBuffer);
		return S_OK;
	}
	void setRotate( float radian ){
		mRotateFlag = true;
		mRotate = D2D1::Matrix3x2F::Rotation( radian, D2D1::Point2F( mRect.right / 2, mRect.bottom / 2 ) );
	}
	void setScale( float x, float y ){
		mScaleFlag = true;
		mScale = D2D1::Matrix3x2F::Scale( x, y, D2D1::Point2F( mRect.right / 2, mRect.bottom / 2 ) );
	}

	IDWriteFactory* writeFac(){
		return mDWriteFactory;
	}
	ID2D1RenderTarget* backBuffer(){
		return mBackBuffer;
	}
	IWICImagingFactory* image(){
		return mImageFactory;
	}

	//Direct2D
	ID2D1Factory* mFactory;
	IWICImagingFactory* mImageFactory;
	IDWriteFactory* mDWriteFactory;
	ID2D1RenderTarget* mBackBuffer;
	ID2D1SolidColorBrush* mBrush;

	D2D1_ELLIPSE mEllipse;
	D2D1_RECT_F mRect;
	D2D1_RECT_F mRectAngle;
	D2D1_POINT_2F mPoint0;
	D2D1_POINT_2F mPoint1;

	D2D1::Matrix3x2F mScale;
	D2D1::Matrix3x2F mRotate;

	bool mRotateFlag;
	bool mScaleFlag;

	Vector4 mColor;
	Bitmap::Impl* mCurrentBitmap;
	SpriteType mSpriteType;	
};
extern SpriteImpl* gSpriteImpl; //インスタンス

}
}

#endif