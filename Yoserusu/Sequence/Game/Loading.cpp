#include "Sequence/Game/Loading.h"
#include "Sequence/Game/ParentGame.h"
#include "Sequence/Game/Loading.h"

#include "Mashiro/Mashiro.h"
#include "Lua/LuaManager.h"

namespace Sequence{
namespace Game{

//こちらスレッド
LoadingThread::LoadingThread( ParentGame* p ) : 
mFinished( false ),
mGame( p ){
	start();
}

LoadingThread::~LoadingThread(){
	wait();
	mGame = 0;
}

void LoadingThread::operator()(){
	mGame->startLoading();
	mFinished = true;
}

Loading::Loading() : mLoading( 0 ), mFirst( false ){
	LuaManager::instance()->loadLua( "Lua/Loading.lua", "Loading" );
}

Loading::~Loading(){
	LuaManager::instance()->deleteLua();
	SAFE_DELETE( mLoading );
}

void Loading::update( ParentGame* parent ){
	//ロードする
	if( !mFirst ){
		mLoading = NEW LoadingThread( parent );
		//parent->startLoading();
		mFirst = true;
	}

	int fade = LuaManager::instance()->runLua<int>( "draw" );

	if( fade > 90 ){
		if( mLoading->mFinished ){
			//ロードがおーけーなら次シーンへ
			parent->moveTo( ParentGame::NEXT_PLAY );
		}
	}
}

} //namespace Game
} //namespce Sequence