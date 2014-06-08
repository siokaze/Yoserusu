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
#include "Game/ArmLeft.h"
#include "Game/ArmRight.h"
#include "Util/DepthSingleton.h"
#include "Util/SoundManager.h"
#include "Game/BackGround.h"

#include "Util/DataBase.h"

#include "Shader/include/CocTrans.h"
#include "Lua/LuaManager.h"

using namespace Mashiro::Math;
using namespace Mashiro::Graphics;

namespace Sequence{

Title::Title() :mPosY(0),mPosZ(0),mAngY(0),mCount(0),isTitle(false),oK(0),oKCount(0), mSceneMoveFlag( false ){
	SoundManager::instance()->playBgm( SoundManager::BGM_TITLE );
		
	mBall.create( "res/model/Ball.pmd" );

	mBallPos =Vector3(0,10,10);
	mTitlePos=Vector3(0,4,-50);

	mTitleBitmap = Mashiro::Graphics::Bitmap::create( "res/image/titile.png" );
	mStart = Mashiro::Graphics::Bitmap::create("res/image/start4.png");

	mHr = NEW ArmRight();
	mHl = NEW ArmLeft();
	Color = 0;
	isTitle = false;
	mKeep = false;
	isEnd = false;

	LuaManager::instance()->loadLua( "lua/Title.lua", "Title" );
}

Title::~Title(){
	SoundManager::instance()->stopBgm();
	LuaManager::instance()->deleteLua();
	SAFE_DELETE( mHr );
	SAFE_DELETE( mHl );
}

void Title::update( Parent* parent ){
#if _DEBUG
	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( Parent::NEXT_GAME );
	}
	if( Mashiro::Input::Manager::instance().keyboard().isTriggered( Input::Keyboard::KEY_RETURN ) ){
		LuaManager::instance()->loadLua( "lua/Title.lua", "Title" );
	}
#endif
	//更新
	titleUpdate( parent );
	//描画
	titledraw();
}

void Title::titledraw(){

	LuaManager::instance()->runLua<int>( "draw" );

	//3D表示
	//ボール
	mBall.setPosition(mBallPos);
	mBall.setColor(Vector3(1,1,1));
	mBall.draw( CocTrans::TYPE_BALL );

}

void Title::titleUpdate( Parent* parent ){
	mHl->Update(0,Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT);
	mHr->Update(0,Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT);

	mAngY+=2.0f;
	mBall.setAngle(Vector3(0.0f,mAngY,-23.4f));

	mPosY += 0.2f/100;

	oKCount++;
	if(oKCount>60&&oK<150){
		oK+=5;
		
	}
	//ボールを上から落とす
	if(mBallPos.y>= -8)
	{
		mBallPos=Vector3(0,mBallPos.y-mPosY,mBallPos.z);
		mBall.setPosition(Vector3( mBallPos ));
	}
	//落ちきったのならタイトルを出す準備をする
	if(mBallPos.y<= -3 &&!isTitle)
	{
		isTitle = true;
		mDepth = DepthSingleton::instance()->getDepthMin();	
	}
	//奥に隠れているタイトルを手前に持ってくる
	if(isTitle&&mTitlePos.z<-0)
	{
		mPosZ+=1.0f;
		mTitlePos=Vector3(0,mTitlePos.y,mTitlePos.z+mPosZ);
	}	
	//タイトルがいるならボールを掴んでいるか判定
	if(isTitle&&!mKeep){
		if(mHl->Depth() <= DepthSingleton::instance()->getDepthMin()){
			mKeep = true;
			Color = 1;
		}
		
		if(mHr->Depth() <= DepthSingleton::instance()->getDepthMin()){
			mKeep = true;
			if(Color == 1){
				Color = 3;
			}
			Color = 2;
		}

	}
	//掴んでいるなら タイトルを手前に移動
	if(mKeep)
	{	
		if(mHl->Depth() == 0 || mHl->Depth() == 0 ) return;

		if(!isEnd){
		int depthcheck =  mHr->Depth();

		if(mHl->Depth() < mHr->Depth()){
			depthcheck =  mHl->Depth();
		}

		if( mDepth < depthcheck) mDepth++;
		if( mDepth > depthcheck) mDepth--;
		

		if(mDepth >= DepthSingleton::instance()->getDepthMax()){
			isEnd = true;
			}	
		}

		if(isEnd)
			mDepth++;
		mBallPos = Vector3(0,mBallPos.y,DepthSingleton::instance()->getDepthMax() - (mDepth));

		if(mDepth > DepthSingleton::instance()->getDepthMax()+40){
			parent->moveTo( Parent::NEXT_GAME );
		}
	}
}

} //namespace Sequence