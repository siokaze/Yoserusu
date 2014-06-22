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
#include "Game/LockOn.h"

#include "Util/DataBase.h"

#include "Shader/include/CocTrans.h"
#include "Lua/LuaManager.h"
#include <utility>
using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

template< typename T > std::unique_ptr< T > factory(){
	return std::unique_ptr< T >( NEW T );
}

namespace Sequence{

Title::Title() :
mDiff(),
mDepth(),
mMode( MODE_NONE ),
mBallPos( 0, 10, 0 ){
	SoundManager::instance()->playBgm( SoundManager::BGM_TITLE );
		
<<<<<<< HEAD
	mBall = std::unique_ptr< ModelObj >( NEW ModelObj( "res/model/Ball.pmd" ) );
	mTitleBitmap = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/titile.png" ) );
	mBackGraound = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/bg.png" ) );
	mStart = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/start4.png" ) );
	mLockOn = factory< LockOn >();
	mBallPos =Vector3(0,10,15);	
=======
	mBall.create( "res/model/Ball.pmd" );

	mBallPos =Vector3(0,10,10);
	mTitlePos=Vector3(0,4,-50);

	mTitleBitmap = Mashiro::Graphics::Bitmap::create( "res/image/titile.png" );
	mStart = Mashiro::Graphics::Bitmap::create("res/image/start4.png");
	mLockOn = NEW LockOn();

	isTitle = false;
	mKeep = false;
	isEnd = false;

	LuaManager::instance()->loadLua( "lua/Title.lua", "Title" );
>>>>>>> origin/develop
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
	//�X�V
	titleUpdate( parent );
	//�`��
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

	mLockOn->draw(Vector2(350, 430));

<<<<<<< HEAD
=======
	//3D�\��
	//�{�[��
	mBall.setPosition(mBallPos);
	mBall.setColor(Vector3(1, 1, 1));
	mBall.draw( CocTrans::TYPE_BALL );
	startAlpha+=0.1;
	mLockOn->draw(Vector2(350, 470));
	LuaManager::instance()->runLua<int>( "startDraw",boost::make_tuple(sin(startAlpha),0,0,0));
>>>>>>> origin/develop
}

void Title::titleUpdate( Parent* parent ){
	Mashiro::Kinect::Manager kinect =  Mashiro::Kinect::Manager::instance();

	int rHandDepth = kinect.depthSkeleton(Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT);
	int lHandDepth = kinect.depthSkeleton(Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT);

	mDiff.mAngY+=2.0f;
	mBall->setAngle(Vector3(0.0f,mDiff.mAngY,-23.4f));

	mDiff.mPosY += 0.2f/100;

	//�{�[�����ォ�痎�Ƃ�
	if(mBallPos.y>= -8)
	{
		mBallPos=Vector3(0,mBallPos.y-mDiff.mPosY,mBallPos.z);
		mBall->setPosition(Vector3( mBallPos ));
	}
	//�����������̂Ȃ�^�C�g�����o������������
	if( mBallPos.y <= -3 && mMode == MODE_NONE )
	{
		mMode = MODE_TITLE;
		mDepth = DepthSingleton::instance()->getDepthMin();	
	}	

	catchCheck(rHandDepth,lHandDepth);

	move(rHandDepth,lHandDepth,parent);
}

void Title::catchCheck(int rHandDepth, int lHandDepth){
	//�^�C�g��������Ȃ�{�[����͂�ł��邩����
	if( mMode == MODE_TITLE ){
		if( rHandDepth<= DepthSingleton::instance()->getDepthMin()){
			mMode = MODE_KEEP;
		}
		
		if( lHandDepth <= DepthSingleton::instance()->getDepthMin()){
			mMode = MODE_KEEP;
		}
<<<<<<< HEAD
		mLockOn->update((bool)mMode);
=======
		mLockOn->update(mKeep);
>>>>>>> origin/develop
	}
}

void Title::move(int rHandDepth, int lHandDepth,Parent* parent){
	//�͂�ł���Ȃ� �^�C�g������O�Ɉړ�
	if(mMode == MODE_KEEP){	
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
	}
	if( mMode == MODE_END ){
		mDepth++;
		mBallPos = Vector3(0,mBallPos.y,DepthSingleton::instance()->getDepthMax() - (mDepth));
	}		

	if(mDepth > DepthSingleton::instance()->getDepthMax()+40){
		parent->moveTo( Parent::NEXT_GAME );
	}
}

} //namespace Sequence