#ifndef INCLUDE_MASHIRO_GRAPHICS_SPRITE_H_
#define INCLUDE_MASHIRO_GRAPHICS_SPRITE_H_

#include "Mashiro/Graphics/Enum.h"

namespace Mashiro {
	namespace Math {
		class Matrix;
		class Vector4;
		class Vector3;
		class Vector2;
	}
}
using namespace Mashiro::Math;

namespace Mashiro{
namespace Graphics{

class Bitmap;

class Sprite{
public:
	Sprite();
	static Sprite instance();

	void standby();//描画前初期化
	void end(); //描画終了
	//透過設定
	void setTrance( float );
	//ビットマップ
	void setBitmap( Bitmap );
	//カラー設定
	void setColor( const Vector3& color );
	//描画
	void draw() const;
	//線をセット
	void setLine( const Vector2& p0, const Vector2& p1 );
	//四角形をセット
	void setRectangle( const Vector2& p0, const Vector2& p1 );
	void setFillRectangle( const Vector2& p0, const Vector2& p1 );
	//円をセット
	void setEllipse( const Vector2& center, const Vector2& radius );
	void setFillEllipse( const Vector2& center, const Vector2& radius );
	//ビットマップ描画
	void setBitmapRectangle( const Vector2& p );
	void setBitmapRectangle( const Vector2& p, const Vector2& size, const Vector4& imageRect );

	static void create();
	static void destroy();
};

}
}

#endif
