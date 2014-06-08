#ifndef INCLUDED_SEQUENCE_AUTHE_H
#define INCLUDED_SEQUENCE_AUTHE_H

#include "Sequence/Child.h"
#include "Mashiro/Math/Vector2.h"
#include "Game/Enum.h"

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