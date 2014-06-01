#ifndef INCLUDE_SPLITE_H_
#define INCLUDE_SPLITE_H_

#include "Mashiro/Graphics/Bitmap.h"

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
	* @brif 2D�`��
	* @param int x - x���W
	* @param int y - y���W
	*/
	void draw( int x, int y );
	/*
	* @brif 2D�`��
	*/
	void draw( const Mashiro::Graphics::Bitmap& bitmap );
private:
	Mashiro::Graphics::Bitmap mBitmap;
};

#endif