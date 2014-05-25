#ifndef INCLUDE_SPLITE_H_
#define INCLUDE_SPLITE_H_

#include "Mashiro/Graphics/Bitmap.h"

/*
* @file Splite.h
* @brif 2D描画クラス 一枚一クラス
*/
class SpriteUtil{
public:
	/*
	* @brif コンストラクタ bitmapロード
	*/
	SpriteUtil( const char* fileName );
	/*
	* @brif デストラクタ
	*/
	~SpriteUtil();
	/*
	* @brif 2D描画
	* @param int x - x座標
	* @param int y - y座標
	*/
	void draw( int x, int y );
private:
	Mashiro::Graphics::Bitmap mBitmap;
};

#endif