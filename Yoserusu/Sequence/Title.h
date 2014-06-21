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
	//�V�[���X�V
	void update( Parent* );
	//�X�V
	void titleUpdate( Parent* );
	//�`��
	void titledraw();
private:	
	//�^�C�g���ŏo���I�u�W�F�N�g�Q
	std::unique_ptr< ModelObj > mBall;
	std::unique_ptr< SpriteUtil > mBackGraound;
	std::unique_ptr< SpriteUtil > mTitleBitmap;
	std::unique_ptr< SpriteUtil > mStart;

	Vector3 mBallPos;
	Difference mDiff;
	SceneMode mMode;
	int mDepth;
};

} //namespace Sequence

#endif