#include "Mashiro/Mashiro.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"

#include "Game/State.h"
#include "Game/Ball.h"
#include "Game/BackGround.h"
#include "Game/Wall.h"
#include "Game/LockOn.h"

#include "Util/ModelLoader.h"
#include "Util/DepthSingleton.h"
#include "Util/DataBase.h"

#include <sstream>
using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Kinect;
using namespace std;

State::State() : 
ball( 0 ), 
wall( 0 ), 
lArm( 0 ), 
rArm( 0 ), 
timer( 0 ), 
mBack( 0 ), 
mLockOn( 0 ){
	//�I�u�W�F�N�g�̐���
	ball = NEW Ball();
	wall = NEW Wall();
	lArm= NEW ArmLeft();
	rArm = NEW ArmRight();
	timer = NEW Timer();
	mBack = NEW BackGround();
	mLockOn = NEW LockOn();
}

void State::init(){
	ball->Initi();
	lArm->Init();
	rArm->Init();
}

State::~State(){
	//delete�͐����������̋t���ł��悤
	SAFE_DELETE( mBack );
	SAFE_DELETE( mLockOn );
	SAFE_DELETE(timer);
	SAFE_DELETE(rArm);
	SAFE_DELETE(lArm);
	SAFE_DELETE(wall);
	SAFE_DELETE(ball);
}

void State::update(){

	//�^�C�}�[�X�V
	timer->update();
	
	//�N���X�X�V
	lArm->Update(ball->Depth(),Mashiro::Kinect::SKELETON_INDEX_HAND_LEFT);
	rArm->Update(ball->Depth(),Mashiro::Kinect::SKELETON_INDEX_HAND_RIGHT);

	//�^�C�}�[�񂵂Ă�
	if( timer->isStart() && !timeUp() ){
		//�{�[���A�b�v�f�[�g
		ball->Update( rArm, lArm );
		//�^�[�Q�b�g
		mLockOn->update( targetCheck() );
	}

	if( !timeUp() ){
		wall->Animation(ball);
	}
}

void State::draw(){
	//�w�i
	mBack->draw();
	//�{�[���`��
	ball->Draw( timer->draw_10(), timer->draw_01());
	//�r�`��
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
}

bool State::timeUp() const{
	//�^�C�}�[���؂��΃V�[���ڍs��
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
	//�^�[�Q�b�g
	if(ball->IsMode() == Keep){
		return true;
	}
	return false;
}