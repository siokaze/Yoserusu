#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

#include "Sequence/Child.h"

#include "Mashiro/Math/Vector3.h"
using namespace Mashiro;
using namespace Mashiro::Math;

#include "Util/ModelObj.h"
#include "Util/Sprite.h"
#include <memory>
class BackGround;
class LockOn;

namespace Sequence{
class Parent;

class Title : public Child{
	enum SceneMode{
		MODE_TITLE,
		MODE_KEEP,
		MODE_END,

		MODE_NONE,
	};
	struct Difference{
		float mPosY;
		float mPosZ;
		float mAngY;
	};
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

	//タイトルで出すオブジェクト群
	std::unique_ptr< ModelObj > mBall;
	std::unique_ptr< SpriteUtil > mBackGraound;
	std::unique_ptr< SpriteUtil > mTitleBitmap;
	std::unique_ptr< SpriteUtil > mStart;
	std::unique_ptr< LockOn > mLockOn;

	void catchCheck(int rHandDepth,int lHandDepth);
	void move(int rHandDepth,int lHandDepth,Parent* parent);

	Vector3 mBallPos;
	Difference mDiff;
	SceneMode mMode;
	int mDepth;
};

} //namespace Sequence

#endif