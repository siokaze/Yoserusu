#include "Sequence/Game/Loading.h"
#include "Sequence/Game/ParentGame.h"
#include "Sequence/Game/Loading.h"

#include "Mashiro/Mashiro.h"
#include "Mashiro/Math/Functions.h"

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

Loading::Loading() : mLoading( 0 ), mFirst( false ), mFade( 0.f ){
	mBlack = std::unique_ptr< SpriteUtil >( NEW SpriteUtil() );
	mBackGraound = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/bg.png" ) );
}

Loading::~Loading(){
	SAFE_DELETE( mLoading );
}

void Loading::update( ParentGame* parent ){
	//ロードする
	if( !mFirst ){
		mLoading = NEW LoadingThread( parent );
		//parent->startLoading();
		mFirst = true;
	}

	float fade = Math::sin( mFade / 30 ) / 2 + 0.5;

	//背景描画
	mBlack->setColor( 0.0, 0.0, 0.0 );
	mBlack->setTransparency( fade );
	mBlack->drawRectangle( 0, 0, 1024, 800 );

	mBackGraound->draw( 0, 0 );

	mFade = mFade + 1;


	if( mFade > 90 ){
		if( mLoading->mFinished ){
			//ロードがおーけーなら次シーンへ
			parent->moveTo( ParentGame::NEXT_PLAY );
		}
	}
}

} //namespace Game
} //namespce Sequence