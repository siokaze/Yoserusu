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
	//シーン更新
	void update( Parent* );
	//更新
	void autheUpdate( Parent* );
	//描画
	void autheDraw();

private:
	//シーンポップフラグ
	int mMoveSceneCount;

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
};

} //namespace Sequence

#endif