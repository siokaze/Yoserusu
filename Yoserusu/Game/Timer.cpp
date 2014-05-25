//#include "Mashiro/Mashiro.h"
#include "Game/Timer.h"
#include <sstream>
#include <fstream>
#include "Util/SoundManager.h"
#include "Util/DataBase.h"
#include "Game/Score.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector2.h"
using namespace std;
using namespace Mashiro;
using namespace Mashiro::Graphics;

int gCount = 0;

Timer::Timer() :  
timer( 0 ),
mFlag( false ), 
mCount( 0 ), 
mWarrningFlag( false ),  
mWarningCount( 30 ),
mPitch( 1.f ){
	mTime = DataBase::instance()->gameTime();
	mStartTimer = 3;
	mWarningpos =0;

	mRWarning = Mashiro::Graphics::Bitmap::create( "res/image/Right_Warning.png");
	mLWarning = Mashiro::Graphics::Bitmap::create( "res/image/Left_Warning.png");

	ostringstream oss;

	//BItmap
	for( int i = 0; i < 4; ++i ){
		oss.str( "" );
		oss << "res/image/count_" << i << ".png";
		Mashiro::Graphics::Bitmap bit = Mashiro::Graphics::Bitmap::create( oss.str().c_str() );
		mCountNumber.insert( pair< int, Mashiro::Graphics::Bitmap >( i, bit ) );
	}
	//10の位をセット
	for( int x = 0; x < 7; ++x ){
		oss.str( "" );
		oss << "res/image/ball10_" << x << ".png";
		Mashiro::Graphics::Texture texture = Mashiro::Graphics::Texture::create( oss.str().c_str() );
		m10Tex.insert( pair< int, Mashiro::Graphics::Texture >( x, texture ) );
	}
	//1の位をセット5
	for( int x = 0; x < 10; ++x ){
		oss.str( "" );
		oss << "res/image/ball1_" << x << ".png";
		Mashiro::Graphics::Texture texture = Mashiro::Graphics::Texture::create( oss.str().c_str() );
		m01Tex.insert( pair< int, Mashiro::Graphics::Texture >( x, texture ) );
	}
}

Timer::~Timer(){
	mCountNumber.clear();
	m10Tex.clear();
	m01Tex.clear();
}

void Timer::update(){
	mFlag = false;

	if( mCount == 60 ){
		
		if(mStartTimer >= 0){
			mStartTimer -= 1;
			mCount = 0;
			return;
		}

		mFlag = true;

	//チュートリアル中はタイマー動かない
	if(Score::instance()->getScore() > 0)
	{
		mTime -= 1;
	}
		mCount = 0;
	}
	
	++mCount;

	if(mTime <= 10){
		//ワーニング切り出し位置を調整
		mWarningpos++;
		if(mWarningpos >= 1024)
		{
			mWarningpos = 0;
		}
		//ワーニングで音がなる
		if( mTime > 0 ){
			mPitch += 0.1f;
			if( mPitch > 1.4f ){
				mPitch = 1.4f;
			}
			SoundManager::instance()->setPitch( mPitch );
			//警告チェック
			//warrningCheck();
		} else {
			SoundManager::instance()->setPitch( 1.0f );
			mWarrningFlag = false;
		}
		//ワーニング音
		if( mWarrningFlag ){
			SoundManager::instance()->playSe( SoundManager::SE_WRNNING );
		}
	}
}

void Timer::startDraw(){
	Mashiro::Graphics::Sprite sp = Graphics::Sprite::instance();
	sp.setColor(Vector3(0.f,0.f,1.f));

	if( mStartTimer == 3 ){
		SoundManager::instance()->playSe( SoundManager::SE_COUNTDOWN );
	}

	switch(mStartTimer){
	case 3: 
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNumber[ 3 ] );
		sp.setBitmapRectangle( Vector2( 100,0 ) );
		sp.draw();
		break;
	case 2: 
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNumber[ 2 ] );
		sp.setBitmapRectangle( Vector2( 100,0 ) );
		sp.draw();
		break;
	case 1: 
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNumber[ 1 ] );
		sp.setBitmapRectangle( Vector2( 100,0 ) );
		sp.draw();
		break;
	case 0: 
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNumber[ 0 ] );
		sp.setBitmapRectangle( Vector2( 100,0 ) );
		SoundManager::instance()->playSe( SoundManager::SE_START );
		sp.draw();
		break;
	default : sp.setTrance(0.f);
	}
}

bool Timer::isStart(){
	if(mStartTimer>=0) return false;

	return true;
}

Mashiro::Graphics::Texture Timer::draw_10(){
	int i_10 = mTime / 10; //10の位
	if( i_10 < 0 ){
		i_10 = 0;
	}
	return m10Tex[ i_10 ];
}

Mashiro::Graphics::Texture Timer::draw_01(){
	int i_01 = mTime % 10; //1の位
	if( i_01 < 0 ){
		i_01 = 0;
	}
	return m01Tex[ i_01 ];
}

void Timer::warrningCheck(){
	if( mWarningCount == 30 ){
		mWarningCount = 0;
		mWarrningFlag = true;
	} else {
		++mWarningCount;
		mWarrningFlag = false;
	}
}

void Timer::warrningDraw(){
	Mashiro::Graphics::Manager gm = Mashiro::Graphics::Manager::instance();
	Mashiro::Graphics::Sprite sp = Graphics::Sprite::instance();
	timer++;
	sp.setTrance(sin((double)timer/30)/2+0.5f);
	sp.setBitmap( mRWarning );
	sp.setBitmapRectangle( Vector2(0+mWarningpos,0) );
	sp.draw();

	sp.setTrance(sin((double)timer/30)/2+0.5f);
	sp.setBitmap( mRWarning );
	sp.setBitmapRectangle( Vector2(-1024+mWarningpos,0 ) );
	sp.draw();

	sp.setTrance(sin(((double)timer/30)+3.141592)/2+0.5f);
	sp.setBitmap( mLWarning );
	sp.setBitmapRectangle( Vector2(0-mWarningpos,758) );
	sp.draw();

	sp.setTrance(sin(((double)timer/30)+3.141592)/2+0.5f);
	sp.setBitmap( mLWarning );
	sp.setBitmapRectangle( Vector2(1024-mWarningpos,758) );
	sp.draw();
}

int Timer::time() const {
	return mTime;
}