#include "Sequence/AutheScene.h"
#include "Sequence/Parent.h"

#include "Mashiro/Mashiro.h"
#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Game\ml\MLCameraHandler.h"

#include "Util/SoundManager.h"
#include "Util/DepthSingleton.h"
#include "Util/DataBase.h"
#include "boost\algorithm\clamp.hpp"

using namespace Mashiro;
using namespace Mashiro::Math;

namespace Sequence{

AutheScene::AutheScene() : 
mMoveSceneCount( 0 ),
mDepth(),
mCount(),
mSum(),
mHandCheckCount(),
mHandCheck( false ),
check( false ),
mMoveSceneFlag( MODE_WAIT ){
	SoundManager::instance()->playBgm(SoundManager::BGM_AUTHE);
	Graphics::Manager m = Graphics::Manager::instance();

	Matrix world,view,proj;
	int width, height;
	m.getViewPort(0,0,&width, &height);
	world.setTranslation(Vector3(0));
	proj.setPerspectiveFovLH(45.f, (float)WorkSpace::instance().width(),(float)WorkSpace::instance().height(),1.0f,10000.f);

	view.setIdentity();
	view.setLookAtLH( Vector3(0,0,-30),Vector3(0.f),Vector3(0,1,0));

	Matrix mat;
	mat.setIdentity();
	mat.setMul( mat, view );
	mat.setMul( mat, proj );
	Matrix inv;
	inv.setInverse( mat );
	Vector4 v( -0.57f, -0.57f, -0.57f, 1.f );
	v = mat.translation( v, inv );

	m.setLight( Vector4( 0.7f,0.7f, 0.6f, 2.f ) );
	m.setEyePos( v );

	m.setViewMatrix(view);
	m.setWorldMatrix(world);
	m.setProjectionMatrix( proj);

	Mashiro::Kinect::Manager::instance().setCamera(0);

	mLeftHand = std::unique_ptr< SpriteUtil >( NEW SpriteUtil() );
	mRightHand = std::unique_ptr< SpriteUtil >( NEW SpriteUtil() );

	mAuth = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/Auth.png" ) );
	mOK = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/Ok.png" ) );
	mRedMaru = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/RedMaru.png" ) );
	mBlueMaru = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/BlueMaru.png" ) ); 
	mTitle_Auth = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/Title_Auth.png" ) );
	mBackGraound = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/bg.png" ) );

	ML::MLCameraHandler::setCameraMatrix(&view);
}

AutheScene::~AutheScene(){
	SoundManager::instance()->stopBgm();
}

void AutheScene::update( Parent* parent ){

#if _DEBUG
	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( Parent::NEXT_TITLE );
	}
#endif

	autheUpdate( parent );

	autheDraw();
}

void AutheScene::autheDraw(){
	Graphics::Manager m = Graphics::Manager::instance();
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();

	Vector2 posR = Mashiro::Kinect::Manager::instance().skeletonPos( Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT );
	Vector2 posL = Mashiro::Kinect::Manager::instance().skeletonPos( Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT );

	//背景の描画
	mBackGraound->draw( 0, 0 );

	//認証範囲画像
	mRedMaru->draw( 600, 100 );
	mBlueMaru->draw( 200, 100 );

	//switchケース 
	switch( mMoveSceneFlag ){
		case MODE_WAIT: mTitle_Auth->draw( 130, 400 ); break;
		case MODE_NOW: 
			mTitle_Auth->draw( 130, 400 );
			if ( check ) {
				mAuth->draw( 200, 530 );
			}
			break;
		case MODE_END: mOK->draw( 130, 530 ); break;
	}

	//手の描画
	mLeftHand->setColor( 0.0, 0.0, 1.0  );
	mLeftHand->drawEllipse( posL.x, posL.y, 30, 30 );

	mRightHand->setColor( 1.0, 0.0, 0.0 );
	mRightHand->drawEllipse( posR.x, posR.y, 30, 30 );
}

void AutheScene::autheUpdate( Parent* parent ){

	float angle =  -11 + Mashiro::Kinect::Manager::instance().depthSkeleton( Mashiro::Kinect::SKELETON_INDEX_HEAD );
	
	Sprite::instance().setTrance(1);

	 if(!Mashiro::Kinect::Manager::instance().tracking()) angle = 0;

	angle = boost::algorithm::clamp(angle,-11,18);

	Mashiro::Kinect::Manager::instance().setCamera(angle);

	auto handRang = [this]( const Vector2& kinectPos, const Vector2& checkPos1 )->bool{
		if(mHandCheckCount > 0) mHandCheckCount--;
		const float lb = checkPos1.x - 200;
		const float rb = checkPos1.x + 200;
		if( ( kinectPos.x < rb ) && ( kinectPos.x > lb ) ){
			const float tb = checkPos1.y - 200;
			const float bb = checkPos1.y + 200;
			if( ( kinectPos.y < bb ) && ( kinectPos.y > tb ) ){
				mHandCheckCount += 2 ;
				return true;
			}
		}
		return false;
	};

	//枠ない収まってるかチェックしてます
	bool rCheck = handRang( Mashiro::Kinect::Manager::instance().skeletonPos( Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT ), Vector2( 600, 100 ) );
	bool lCheck = handRang( Mashiro::Kinect::Manager::instance().skeletonPos( Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT ),  Vector2( 200, 100 ) );

	auto handFlag = [this]( bool rCheck, bool lCheck )mutable->bool{

		if ( rCheck && lCheck ){
			mMoveSceneFlag = MODE_NOW;
			mSum += Mashiro::Kinect::Manager::instance().depthSkeleton( Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT );
			++mCount;
			check = true;
		}else if(!rCheck && !lCheck){
			check = false;
			mCount = 0;
			mDepth = 0;
		}

		if( mCount > 20 ){
			return mHandCheck = true;
		}

		return mHandCheck = false;
	};

	auto depthCheck = [this]()mutable->void{
		//深度値をうまく取ってこれるまでループさせて
		mDepth = mSum / mCount;

		int depthCheck = 14;

		if(Mashiro::Kinect::Manager::instance().depthSkeleton(Mashiro::Kinect::SKELETON_INDEX_HEAD)  <  38 ) depthCheck = 1; //子供の身長によって変える

		if(mDepth < depthCheck){
			mHandCheck = false;
			mSum = 0;
			mCount = 0;
			mMoveSceneFlag = MODE_NOW;
			return;
		}

		DepthSingleton::instance()->setDepthMin( static_cast< float >( mDepth - 3));
		DepthSingleton::instance()->setDepthMax(static_cast< float >(
			Mashiro::Kinect::Manager::instance().depthSkeleton(Mashiro::Kinect::SKELETON_INDEX_HEAD)) );

		if(DepthSingleton::instance()->getDepthMin() > DepthSingleton::instance()->getDepthMax()){
			mHandCheck = false;
			mSum = 0;
			mCount = 0;
			mMoveSceneFlag = MODE_NOW;
			return;
		}
		if(DepthSingleton::instance()->getDepthMax()-DepthSingleton::instance()->getDepthMin() < depthCheck ){
			mHandCheck = false;
			mSum = 0;
			mCount = 0;
			mMoveSceneFlag = MODE_NOW;
			return;
		}

		//モードをNow→END
		if(mHandCheck){
			mMoveSceneFlag = MODE_END;
		}
	};

	auto moveScene = [this]()mutable->bool{
		if( mMoveSceneCount == 90 ){
			//認証が完了した 放置してからシーンをポップする
			SoundManager::instance()->playSe(SoundManager::SE_OK);
		}
		if( mMoveSceneCount == 100 ){
			return true;
		}
		++mMoveSceneCount;
		return false;
	};


	if( mMoveSceneFlag == MODE_END ){
		if( moveScene() ){
			parent->moveTo( Parent::NEXT_TITLE );
		}
	}

	if( !mHandCheck ){
		if( handFlag( rCheck, lCheck ) ){
			//認証が完了したら深度値取得
			depthCheck();
		}
	}
}

} //namespace Sequence