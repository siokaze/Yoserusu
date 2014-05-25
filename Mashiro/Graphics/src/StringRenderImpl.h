#ifndef INCLUDE_MASHIRO_GRAPHICS_STRING_IMPL_RENDER_H_
#define INCLUDE_MASHIRO_GRAPHICS_STRING_IMPL_RENDER_H_

#include "Mashiro/Mashiro.h"
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/StringRender.h"

#include <D2D1.h>
#include <DWrite.h>
#include <Windows.h> 
#include <tchar.h> 
#include <vector>
namespace Mashiro{

namespace Graphics {

class StringRender::Impl : public Mashiro::ReferenceType {
public:
	Impl( Font font, int fontSize, IDWriteFactory* write, ID2D1RenderTarget* backBuffer  ) :
	  mTextFormat( 0 ) {
		HRESULT hr;
		switch( font ){
		case FONT_GOSIC:
			hr = create( L"ＭＳゴシック", fontSize, write );
			break;
		case FONT_MEIRIO:
			hr = create( L"メイリオ", fontSize, write );
			break;

		case FONT_MYOUTYOU:
			hr = create( L"ＭＳ Ｐ明朝", fontSize, write );
			break;
		}

		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "フォントの生成に失敗しました" );
		}

		//ブラシ生成
		backBuffer->CreateSolidColorBrush( D2D1::ColorF( 0, 0, 0 ), &mBursh );
	}
	~Impl(){
		mTextFormat->Release();
		mTextFormat = 0;
		mBursh->Release();
		mBursh = 0;
	}

	HRESULT create( LPCWSTR fontName, int fontSize, IDWriteFactory* write ){
		// フォントサイズがあり得ない場合強制的に.0f指定
		if( fontSize <= 0 )
			fontSize = 16;
		// テキストフォーマットを生成する。
		if( FAILED( write->CreateTextFormat( 
			TEXT( fontName ), NULL,
			DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, static_cast< FLOAT >( fontSize ), L"en-us", &mTextFormat )))
		{
			return E_FAIL;
		}
		return S_OK;
	}

	void draw( wchar_t* text, int x, int y, ID2D1RenderTarget* backBuffer ){
		backBuffer->BeginDraw();

		FLOAT size = mTextFormat->GetFontSize();

		D2D1_RECT_F rect = D2D1::RectF( 
			static_cast< FLOAT >( x ), 
			static_cast< FLOAT >( y ), 
			static_cast< FLOAT >( x ) + size*wcslen(text), 
			static_cast< FLOAT >( y ) + size );

		backBuffer->DrawText(
			text, wcslen( text ), mTextFormat, rect, mBursh );

		backBuffer->EndDraw();
	}

	void draw(  ID2D1RenderTarget* backBuffer, int x = 0, int y = 0 ){

		backBuffer->BeginDraw();

		//文字列をワイド文字列に変換
		int iSize = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED,
					mBuffer.str().c_str(), (int)mBuffer.str().size(), NULL, 0 );

		//収得したサイズ分要素を確保
		std::vector< wchar_t > vecWide( iSize );

		//改めてサイズ分コピー
		iSize = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,
					mBuffer.str().c_str(), (int)mBuffer.str().size(),&vecWide[0],iSize);
		
		//wstring型に変換
		std::wstring wstrValue( vecWide.begin(), vecWide.begin() + vecWide.size() );

		//フォントサイズを取得
		FLOAT size = mTextFormat->GetFontSize();

		//文字列を描画する領域を指定
		D2D1_RECT_F lauyoutRect = D2D1::RectF(
			static_cast< FLOAT >( x ), 
			static_cast< FLOAT >( y ), 
			static_cast< FLOAT >( x ) + size * wstrValue.length(),
			static_cast< FLOAT >( y ) + size );

		//文字列を描画
		backBuffer->DrawText( wstrValue.c_str(),
			wstrValue.length(), mTextFormat, lauyoutRect, mBursh );

		mBuffer.str( "" );

		backBuffer->EndDraw();


	}
	
	void setColor( float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f ){
		D2D1_COLOR_F c = { r, g, b, a };
		mBursh->SetColor( c );
	}


	//描画
	//文字列
	std::ostringstream mBuffer;
	IDWriteTextFormat* mTextFormat;
	ID2D1SolidColorBrush* mBursh;
};

}
}

#endif