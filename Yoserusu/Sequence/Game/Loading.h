#ifndef INCLUDED_SEQUENCE_GAME_LOADY_H
#define INCLUDED_SEQUENCE_GAME_LOADY_H

#include "Sequence/Game/Child.h"
#include "Mashiro/Threading/Thread.h"

class BackGround;

namespace Sequence{

namespace Game{
class ParentGame;

class LoadingThread : public Mashiro::Threading::Thread{
public:
	LoadingThread( ParentGame* );
	~LoadingThread();

	void operator()();
public:
	bool mFinished;
	ParentGame* mGame;
};

class Loading : public Child{
public:
	Loading();
	~Loading();
	void update( ParentGame* );
private:
	LoadingThread* mLoading;
	bool mFirst;
};

} //namespace Game
} //namespace Sequence

#endif