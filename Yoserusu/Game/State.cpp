#include "Mashiro/Mashiro.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"

#include "Game/State.h"
#include "Game/Ball.h"
#include "Game/Wall.h"
#include "Game/LockOn.h"
#include "Game/Explanation.h"

#include "Util/ModelLoader.h"
#include "Util/DepthSingleton.h"
#include "Util/DataBase.h"

#include <sstream>

#include "Sequence/Game/ParentGame.h"
using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Kinect;
using namespace std;

template< typename T > std::unique_ptr< T > factory(){
	return std::unique_ptr< T >( NEW T );
}

State::State(){
	//オブジェクトの生成
	ball = factory< Ball >();
	wall = factory< Wall >();
	lArm= factory< ArmLeft >();
	rArm = factory< ArmRight >();
	timer = factory< Timer >();
	mLockOn = factory< LockOn >();
	mExplanation = factory< Explanation >();
}

void State::init(){
	ball->Initi();
	lArm->Init();
	rArm->Init();
	mExplanation->Init();
}

State::~State(){

}

void State::update(){

	//タイマー更新
	timer->update();
	
	//クラス更新
	lArm->Update(ball->Depth(),Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT);
	rArm->Update(ball->Depth(),Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT);

	//タイマー回してる
	if( timer->isStart() && !timeUp() ){
		//ボールアップデート
		ball->Update( rArm.get(), lArm.get() );
		//ターゲット
		mLockOn->update( targetCheck() );
	}

	if( !timeUp() ){
		wall->Animation(ball.get());
	}

	mExplanation->ChangeTexture(wall->GetWallColor(),targetCheck());

}

void State::draw(){
	//ボール描画
	ball->Draw( timer->draw_10(), timer->draw_01());
	//腕描画
	rArm->Draw();
	lArm->Draw();
	wall->Draw( timer->IsEnd() );
    mLockOn->draw();
	timer->startDraw();
	if( timer->time() <= 10 ){
		if( timer->time() > 0 ){
			timer->warrningDraw();
		}
	}
	if( timer->isStart() && !timeUp() ){
		mExplanation->Draw();
	}

	//タイマーが10ごとに回って写真を取るわけですよ
	if( timer->time() % 10 == 0 && !timer->IsEnd() ){
		Sequence::Game::ParentGame::albumSave( Mashiro::Kinect::Manager().colorTexture() );
	}
	
}
	

bool State::timeUp() const{
	//タイマーが切れればシーン移行へ
	if(timer->IsEnd()){
		return true;
	}
	return false;
}

bool State::countDown() const{
	if(!timer->isStart()){
		return false;
	}
	return true;
}

bool State::targetCheck() const {
	//ターゲット
	if(ball->IsMode() == Keep){
		return true;
	}
	return false;
}