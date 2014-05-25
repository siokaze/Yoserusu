#ifndef AUTHE_H
#define AUTHE_H

#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Graphics/Bitmap.h"
#include "Enum.h"
using namespace Mashiro::Math;

class Authe {
public:
	Authe();
	~Authe();

	//腕チェック
	bool HandRang( const Vector2& kinectPos, const Vector2& checkPos1 );

	bool handCheck();
	
	void depthCheck();

	void draw() ;

	AutheMode getMode() const;
	bool handFlag() const;
private: 
	//認証完了
	 bool mHandCheck;
	//深度値
	int mDepth;
	int mCount;
	int mSum;
	int mHandCheckCount;

	bool check;

	//シーンポップフラグ
	AutheMode mMoveSceneFlag;

	Mashiro::Graphics::Bitmap mBitmap[ 5 ];
};

#endif