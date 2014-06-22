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

	void standby();//�`��O������
	void end(); //�`��I��
	//���ߐݒ�
	void setTrance( float );
	//�r�b�g�}�b�v
	void setBitmap( Bitmap );
	//�J���[�ݒ�
	void setColor( const Vector3& color );
	//�`��
	void draw() const;
	//�����Z�b�g
	void setLine( const Vector2& p0, const Vector2& p1 );
	//�l�p�`���Z�b�g
	void setRectangle( const Vector2& p0, const Vector2& p1 );
	void setFillRectangle( const Vector2& p0, const Vector2& p1 );
	//�~���Z�b�g
	void setEllipse( const Vector2& center, const Vector2& radius );
	void setFillEllipse( const Vector2& center, const Vector2& radius );
	//�r�b�g�}�b�v�`��
	void setBitmapRectangle( const Vector2& p );
	void setBitmapRectangle( const Vector2& p, const Vector2& size, const Vector4& imageRect );
	
	//rotate
	void setRotate( float radian );
	//scale
	void setScale( float x, float y );

	static void create();
	static void destroy();
};

}
}

#endif
