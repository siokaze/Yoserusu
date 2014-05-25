#include "Mashiro/Mashiro.h"
#include "Mashiro/Threading/Thread.h"
#include "Mashiro/Threading/Event.h"
#include "Mashiro/Threading/src/ThreadingManagerImpl.h"

namespace Mashiro {

namespace Threading {

class Thread::Impl{
public:
	Impl(){
		mFinished = Event::create();
	}
	~Impl(){
	}
	Event mFinished;
};

Thread::Thread() : mImpl( 0 ){
}

void Thread::start(){
	wait(); //前のがあれば待つ
	mImpl = NEW Impl;
	gManagerImpl->add( this, &mImpl->mFinished ); //実行開始
}

void Thread::wait(){
	if( mImpl ){
		mImpl->mFinished.wait();
		SAFE_DELETE( mImpl );
	}
}

bool Thread::isFinished() {
	bool r = false;
	if( mImpl ){
		if( mImpl->mFinished.isSet() ){
			SAFE_DELETE( mImpl );
			r = true;
		}
	} else {
		r = true;
	}
	return r;
}

Thread::~Thread(){
	STRONG_ASSERT( !mImpl && "You must call wait() or isFinished() before destruction." );
}

}
}