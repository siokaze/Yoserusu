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
using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Kinect;
using namespace std;

template< typename T > std::unique_ptr< T > factory(){
	return std::unique_ptr< T >( NEW T );
}

State::State(){
	//�I�u�W�F�N�g�̐���
<<<<<<< HEAD
	ball = factory< Ball >();
	wall = factory< Wall >();
	lArm= factory< ArmLeft >();
	rArm = factory< ArmRight >();
	timer = factory< Timer >();
	mLockOn = factory< LockOn >();
	mExplanation = factory< Explanation >();
=======
	ball = NEW Ball();
	wall = NEW Wall();
	lArm= NEW ArmLeft();
	rArm = NEW ArmRight();
	timer = NEW Timer();
	mLockOn = NEW LockOn();
	mExplanation = NEW Explanation();
>>>>>>> origin/develop
}

void State::init(){
	ball->Initi();
	lArm->Init();
	rArm->Init();
	mExplanation->Init();
}

State::~State(){
<<<<<<< HEAD

=======
	//delete�͐����������̋t���ł��悤
	SAFE_DELETE( mLockOn );
	SAFE_DELETE(timer);
	SAFE_DELETE(rArm);
	SAFE_DELETE(lArm);
	SAFE_DELETE(wall);
	SAFE_DELETE(ball);
	SAFE_DELETE(mExplanation);
>>>>>>> origin/develop
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
		ball->Update( rArm.get(), lArm.get() );
		//�^�[�Q�b�g
		mLockOn->update( targetCheck() );
	}

	if( !timeUp() ){
		wall->Animation(ball.get());
	}

	mExplanation->ChangeTexture(wall->GetWallColor(),targetCheck());

}

void State::draw(){
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
	if( timer->isStart() && !timeUp() ){
		mExplanation->Draw();
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