#ifndef INCLUDED_SEQUENCE_AUTHE_H
#define INCLUDED_SEQUENCE_AUTHE_H

#include "Sequence/Child.h"

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
	//移動のフラグ
	bool moveScene();
private:
	//シーンポップフラグ
	int mMoveSceneCount;
	//認証クラス
	Authe* mAuthe;
	//背景クラス
	BackGround* mBack;
};

} //namespace Sequence

#endif