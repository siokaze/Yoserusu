#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

#include "Sequence/Child.h"

#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Graphics/Bitmap.h"
#include "Game/ArmRight.h"
#include "Game/ArmLeft.h"

using namespace Mashiro;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

class BackGround;

namespace Sequence{
class Parent;

class Title : public Child{
public:
	Title();
	~Title();
	//シーン更新
	void update( Parent* );
	//更新
	void titleUpdate( Parent* );
	//描画
	void titledraw();
private:	
	Mashiro::Graphics::Bitmap mTitleBitmap;

	Model mBall;
	Model mTitle;
	Texture mTitleTex;

	Vector3 mBallPos;
	Vector3 mTitlePos;

	ArmRight* mHr;
	ArmLeft* mHl;
	BackGround* mBack;

	float mPosY;
	float mPosZ;
	float mAngY;
	float mCount;

	float oKCount;
	float oK;

	bool mKeep;
	int Color;
	bool isTitle;
	int mDepth;
	bool isEnd;

	bool mSceneMoveFlag;
};

} //namespace Sequence

#endif