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
	//シーン更新
	void update( Parent* );
	//更新
	void autheUpdate( Parent* );
	//描画
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