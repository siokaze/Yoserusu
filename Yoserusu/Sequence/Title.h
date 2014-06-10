#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

#include "Sequence/Child.h"

#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Graphics/Bitmap.h"

using namespace Mashiro;
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

#include "Util/ModelObj.h"

class BackGround;

namespace Sequence{
class Parent;

class Title : public Child{
public:
	Title();
	~Title();
	//�V�[���X�V
	void update( Parent* );
	//�X�V
	void titleUpdate( Parent* );
	//�`��
	void titledraw();
private:	
	Mashiro::Graphics::Bitmap mTitleBitmap;

	ModelObj mBall;

	Texture mTitleTex;
	Mashiro::Graphics::Bitmap mStart;

	Vector3 mBallPos;
	Vector3 mTitlePos;

	float mPosY;
	float mPosZ;
	float mAngY;
	float mCount;

	float oKCount;
	float oK;

	bool mKeep;
	bool isTitle;
	int mDepth;
	bool isEnd;

	bool mSceneMoveFlag;
};

} //namespace Sequence

#endif