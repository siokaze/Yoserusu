#ifndef INCLUDED_SEQUENCE_PARENT_H
#define INCLUDED_SEQUENCE_PARENT_H

#include "Game/State.h"

namespace Sequence{
class Child;

class Parent{
public:
	enum NextSequence{
		NEXT_TITLE,
		NEXT_GAME,
		NEXT_AUTHE,

		NEXT_NONE,
	};

	//更新
	void update();
	//シーンの移動
	void moveTo( NextSequence );

	//ゲームロード
	bool startLoad();

	//メイン関数郡
	static void create();
	static void destroy();
	static Parent* instance();
private:
	Parent();
	~Parent();

	NextSequence mNextSequence;
	int mStageID;

	Child* mChild;

	static Parent* mInstance;

};

} //namespace Sequence

#endif
