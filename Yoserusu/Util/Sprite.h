#ifndef INCLUDE_SPLITE_H_
#define INCLUDE_SPLITE_H_

#include "Mashiro/Graphics/Bitmap.h"
#include "Mashiro/Math/Vector3.h"

/*
* @file Splite.h
* @brif 2D�`��N���X �ꖇ��N���X
*/
class SpriteUtil{
public:
	SpriteUtil(){}
	/*
	* @brif �R���X�g���N�^ bitmap���[�h
	*/
	SpriteUtil( const char* fileName );
	/*
	* @brif �f�X�g���N�^
	*/
	~SpriteUtil();
	/*
	* @brief ����
	*/
	void create( const char* fileName );
	/*
	* @brif 2D�`��(�l�p�`)
	* @param int x - x���W
	* @param int y - y���W
	* @param int w - ��
	* @param int h - ����
	*/
	void drawRectangle( int x, int y, int w, int h );
	/*
	* @brif 2D�`��
	* @param int x - x���W
	* @param int y - y���W
	*/
	void draw( int x, int y );
	/*
	* @brif 2D�`��(�~)
	* @param int x - x���W
	* @param int y - y���W
	* @param int radiusX - ���a�傫��X
	* @param int radiusY - ���a�傫��Y
	*/
	void drawEllipse( int x, int y, int radiusX, int radiusY );
	/*
	* @brif 2D�`��
	*/
	void draw( const Mashiro::Graphics::Bitmap& bitmap );
	/*
	* @brif �J���[�ݒ�(default=white)
	*/
	void setColor( float r, float g, float b );
		/*
	* @brif �J���[�ݒ�(default=white)
	*/
	void setColor( const Mashiro::Math::Vector3& color );
	/*
	* @brif ���ߗ�
	*/
	void setTransparency( float t );
private:
	Mashiro::Graphics::Bitmap mBitmap;
	Mashiro::Math::Vector3 mColor;
	float mAlpha;
};

#endif