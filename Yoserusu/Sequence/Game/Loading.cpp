#include "Sequence/Game/Loading.h"
#include "Sequence/Game/ParentGame.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Sequence/Game/Loading.h"
#include "Game/BackGround.h"

#include "Mashiro/Mashiro.h"

namespace Sequence{
namespace Game{

//������X���b�h
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

Loading::Loading() : mLoading( 0 ), mFade( 0 ), mFirst( false ){
	mBack = NEW BackGround();
}

Loading::~Loading(){
	SAFE_DELETE( mLoading );
	SAFE_DELETE( mBack );
}

void Loading::update( ParentGame* parent ){
	//���[�h����
	if( !mFirst ){
		mLoading = NEW LoadingThread( parent );
		//parent->startLoading();
		mFirst = true;
	}
	if( mFade > 90 ){
		if( mLoading->mFinished ){
			//���[�h�����[���[�Ȃ玟�V�[����
			parent->moveTo( ParentGame::NEXT_PLAY );
		}
	}
	++mFade;

	//�w�i
	mBack->draw();
	////�Ó]
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setColor( Vector3( 0.f ) );
	sp.setTrance(sin((double)mFade/30)/2+0.5f);
	sp.setFillRectangle( Vector2( 0.f ), Vector2( 1024, 800 ) );
	sp.draw();
}

} //namespace Game
} //namespce Sequence