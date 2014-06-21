#ifndef INCLUDED_SEQUENCE_AUTHE_H
#define INCLUDED_SEQUENCE_AUTHE_H

#include "Sequence/Child.h"
#include "Mashiro/Math/Vector2.h"
#include "Game/Enum.h"
#include "Util/Sprite.h"

#include <memory>

class BackGround;
class Authe;

namespace Sequence{
class Parent;

class AutheScene : public Child{
public:
	AutheScene();
	~AutheScene();
	//�V�[���X�V
	void update( Parent* );
	//�X�V
	void autheUpdate( Parent* );
	//�`��
	void autheDraw();

private:
	std::unique_ptr< SpriteUtil > mLeftHand;
	std::unique_ptr< SpriteUtil > mRightHand;

	std::unique_ptr< SpriteUtil > mAuth;
	std::unique_ptr< SpriteUtil > mOK;
	std::unique_ptr< SpriteUtil > mRedMaru;
	std::unique_ptr< SpriteUtil > mBlueMaru;
	std::unique_ptr< SpriteUtil > mTitle_Auth;
	std::unique_ptr< SpriteUtil > mBackGraound;

	//�V�[���|�b�v�t���O
	int mMoveSceneCount;

	//�F�؊���
	bool mHandCheck;

	//�[�x�l
	int mDepth;
	int mCount;
	int mSum;
	int mHandCheckCount;

	bool check;

	//�V�[���|�b�v�t���O
	AutheMode mMoveSceneFlag;
};

} //namespace Sequence

#endif