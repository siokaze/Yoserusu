#include "Mashiro/Mashiro.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Window/Window.h"
#include "Mashiro/Sound/src/SoundBase/SoundBase.h"
#include "Mashiro/Threading/ThreadingManager.h"

#include <string>
#include <Windows.h>
#include <fstream>
using namespace std;

namespace Mashiro{

namespace {

class Impl {
public:
	Impl() :
	mWidth( 800 ),
	mHeight( 600 ),
	mFullScreen( false ),
	mTitle( "ましろんアプリケーション" ),
	mFrameRate( 0 ),
	mIdealFrameInterval( 0 ),
	mEndRequested( false ),
	mStarted( false ),
	mExtraThreadNumber( 0 ),
	mKinectStart( false )
	{
		//フレーム履歴リセット
		unsigned t = time();
		for ( int i = 0; i < TIME_HISTORY_SIZE; ++i ){
			mTimeHistory[ i ] = t;
		}
	}
	~Impl(){
		Sound::SoundBase::destroy();
		Graphics::Sprite::destroy();
		Input::Manager::destroy();
		if( mKinectStart ){
			Kinect::Manager::destroy();
		}
		Graphics::Manager::destroy();
		Threading::Manager::destroy();
	}
	void start( void* handle ){
		//スレッド初期化
		Threading::Manager::create( mExtraThreadNumber );
		//描画初期化
		Graphics::Manager::create( handle, mWidth, mHeight, mFullScreen );
		Graphics::Sprite::create();
		//Kinect初期化
		if( mKinectStart ){
			Kinect::Manager::create( mWidth, mHeight, mFullScreen );
		}
		//入力初期化
		Input::Manager::create( handle );
		//音楽初期化
		Sound::SoundBase::create();

		mStarted = true;
	}
	unsigned time() const {
		return Window().time();
	}
	void preUpdate(){
		unsigned currentTime = time();
		while( ( currentTime - mPreviousFrameTime ) < mIdealFrameInterval ){
			sleep( 1 );
			currentTime = time();
		}
		mPreviousFrameTime = currentTime;

		//フレーム更新
		mPreviousFrameInterval = currentTime - mTimeHistory[ TIME_HISTORY_SIZE - 1 ];
		unsigned frameIntervalSum = currentTime - mTimeHistory[ 0 ];
		mFrameRate = TIME_HISTORY_SIZE * 1000 / frameIntervalSum;
		//履歴更新
		for( int i = 0; i < TIME_HISTORY_SIZE - 1; ++i ){
			mTimeHistory[ i ] = mTimeHistory[ i + 1 ];	
		}
		mTimeHistory[ TIME_HISTORY_SIZE - 1 ] = currentTime;

		float pointerScale;
		Mashiro::Math::Vector2 pointerOffset;
		Graphics::Manager().getPointerModifier( &pointerScale, &pointerOffset );
		Input::Manager().update( pointerScale, pointerOffset );
		if( mKinectStart ){
			Kinect::Manager::instance().update();
		}
	}
	void postUpdate(){
		Graphics::Manager().end();
	}
	void sleep( int s ){
		Sleep( s );
	}
	void kinectCreate(){
		//Kinect初期化
		mKinectStart = true;
		Kinect::Manager::create( mWidth, mHeight, mFullScreen );
	}
	int mWidth;
	int mHeight;
	bool mFullScreen;
	string mTitle;	
	int mExtraThreadNumber;
	static const int TIME_HISTORY_SIZE = 60;
	unsigned mTimeHistory[ TIME_HISTORY_SIZE ];
	int mPreviousFrameInterval;
	unsigned mPreviousFrameTime;
	int mFrameRate;
	unsigned mIdealFrameInterval;
	bool mEndRequested;
	bool mStarted;

	bool mKinectStart;
};

Impl* gImpl = 0;

}

void WorkSpace::Configuration::setWidth( int width ){
	gImpl->mWidth = width;
}

void WorkSpace::Configuration::setHeight( int height ){
	gImpl->mHeight = height;
}

void WorkSpace::Configuration::setTitle( const char* title ){
	gImpl->mTitle = title;
}

void WorkSpace::Configuration::enableWindowScreen( bool f ){
	gImpl->mFullScreen = f;
}

const char* WorkSpace::Configuration::commandLineString() const {
	return Window().commandLineString();
}

void WorkSpace::Configuration::enableVSync( bool f ){
	
}

void WorkSpace::Configuration::setExtraThreadNumber( int n ){
	//STRONG_ASSERT( n >= 0 && "extraThreadNumber must be >= 0" );
	gImpl->mExtraThreadNumber = n;
}

WorkSpace::WorkSpace(){

}

void WorkSpace::create(){
	gImpl = NEW Impl();
}

void WorkSpace::destroy(){
	SAFE_DELETE( gImpl );
}

WorkSpace WorkSpace::instance(){
	return WorkSpace();
}

void WorkSpace::start( void* handle ){
	gImpl->start( handle );
}

int WorkSpace::width() const {
	return gImpl->mWidth;
}

int WorkSpace::height() const {
	return gImpl->mHeight;
}

const char* WorkSpace::title() const {
	return gImpl->mTitle.c_str();
}

bool WorkSpace::isWindowScreen() const {
	return gImpl->mFullScreen;
}

void WorkSpace::requestEnd() {
	gImpl->mEndRequested = true;
}

bool WorkSpace::isEndRequested() const {
	return gImpl->mEndRequested;
}

unsigned WorkSpace::time() const {
	return gImpl->time();
}

int WorkSpace::frameRate() const {
	return gImpl->mFrameRate;
}

void WorkSpace::setFrameRate( int f ){
	gImpl->mIdealFrameInterval = 1000 / f;
}

void WorkSpace::preUpdate(){
	gImpl->preUpdate();
}

void WorkSpace::postUpdate(){
	gImpl->postUpdate();
}

void WorkSpace::sleep( int i ){
	gImpl->sleep( i );
}

void WorkSpace::kinectCreate() {
	gImpl->kinectCreate();
}

void Window::configure( Configuration* c ){
	WorkSpace::create();
	//ユーザー設定
	WorkSpace w;
	WorkSpace::Configuration wsCon;
	w.configure( &wsCon );
	c->setWidth( w.width() );
	c->setHeight( w.height() );
	c->setTitle( w.title() );
}

void Window::update(){
	Window w;
	Graphics::Manager m;
	WorkSpace ws;
	bool sleep = false;
	if( w.isMinimized() ){
		sleep = true;
	}
	//終了チェック
	if( isEndRequested() ){
		ws.requestEnd();
	}
	if( !sleep ){
		ws.preUpdate();
	}
	if( sleep ){
		ws.sleep( 50 );
	} else {
		try{
			ws.update();
		}
		catch( ... ){
			WorkSpace::destroy();
			end();
			throw Mashiro::EXCEPTION_EXIT;
		}
		ws.postUpdate();
	}
	if( ws.isEndRequested() ){
		WorkSpace::destroy();
		end();
	}
}

void Window::start( void* handle ){
	WorkSpace().start( handle );
}

}