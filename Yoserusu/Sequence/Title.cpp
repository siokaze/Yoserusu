#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Mashiro/Mashiro.h"
#include "Mashiro\WorkSpace\WorkSpace.h"
#include "Mashiro\Graphics\GraphicsManager.h"
#include "Mashiro\Kinect\KinectManager.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Util/ModelLoader.h"
#include "Util/DepthSingleton.h"
#include "Util/SoundManager.h"

#include "Util/DataBase.h"

#include "Shader/include/CocTrans.h"
#include "Lua/LuaManager.h"
#include <utility>
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Sequence{

Title::Title() :
mDiff(),
mDepth(),
mMode( MODE_NONE ),
mBallPos( 0, 10, 10 ){
	SoundManager::instance()->playBgm( SoundManager::BGM_TITLE );
		
	mBall = std::unique_ptr< ModelObj >( NEW ModelObj( "res/model/Ball.pmd" ) );
	mTitleBitmap = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/titile.png" ) );
	mBackGraound = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/bg.png" ) );
	mStart = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/start4.png" ) );
}

Title::~Title(){
	SoundManager::instance()->stopBgm();
}

void Title::update( Parent* parent ){
#if _DEBUG
	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( Parent::NEXT_GAME );
	}
#endif
	//更新
	titleUpdate( parent );
	//描画
	titledraw();
}

void Title::titledraw(){

	mBackGraound->setTransparency(1);
	mBackGraound->draw( 0, 0 );
	mTitleBitmap->draw( 90, -10 );

	mBall->setPosition(mBallPos);
	mBall->setColor(Vector3(1, 1, 1));
	mBall->draw( CocTrans::TYPE_BALL );
	
	static float startAlpha = 0.0;
	startAlpha += 0.1;
	float alpha = sin( startAlpha );

	mStart->setTransparency(sin(alpha));
	mStart->draw( 280, 550 );

}

void Title::titleUpdate( Parent* parent ){
	Mashiro::Kinect::Manager kinect =  Mashiro::Kinect::Manager::instance();

	int rHandDepth = kinect.depthSkeleton(Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT);
	int lHandDepth = kinect.depthSkeleton(Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT);

	mDiff.mAngY+=2.0f;
	mBall->setAngle(Vector3(0.0f,mDiff.mAngY,-23.4f));

	mDiff.mPosY += 0.2f/100;

	//ボールを上から落とす
	if(mBallPos.y>= -8)
	{
		mBallPos=Vector3(0,mBallPos.y-mDiff.mPosY,mBallPos.z);
		mBall->setPosition(Vector3( mBallPos ));
	}
	//落ちきったのならタイトルを出す準備をする
	if( mBallPos.y <= -3 && mMode == MODE_NONE )
	{
		mMode = MODE_TITLE;
		mDepth = DepthSingleton::instance()->getDepthMin();	
	}	
	//タイトルがいるならボールを掴んでいるか判定
	if( mMode == MODE_TITLE ){
		if( rHandDepth<= DepthSingleton::instance()->getDepthMin()){
			mMode = MODE_KEEP;
		}
		
		if( lHandDepth <= DepthSingleton::instance()->getDepthMin()){
			mMode = MODE_KEEP;
		}

	}
	//掴んでいるなら タイトルを手前に移動
	if(mMode == MODE_KEEP)
	{	
		if( rHandDepth == 0 || lHandDepth == 0 ) return;

		if( mMode != MODE_END ){
			int depthcheck =  rHandDepth;

			if( lHandDepth < rHandDepth){
				depthcheck =  lHandDepth;
			}

			if( mDepth < depthcheck) mDepth++;
			if( mDepth > depthcheck) mDepth--;
		

			if(mDepth >= DepthSingleton::instance()->getDepthMax()){
				mMode = MODE_END;
			}	
		}

		if( mMode == MODE_END ){
			mDepth++;
		}
		mBallPos = Vector3(0,mBallPos.y,DepthSingleton::instance()->getDepthMax() - (mDepth));

		if(mDepth > DepthSingleton::instance()->getDepthMax()+40){
			parent->moveTo( Parent::NEXT_GAME );
		}
	}
}

} //namespace Sequence