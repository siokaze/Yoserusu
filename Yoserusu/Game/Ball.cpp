#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Util/DepthSingleton.h"
#include "Game/Ball.h"
#include "Util/ModelLoader.h"
#include "Shader/include/CocTrans.h"

#include "Mashiro/Math/Matrix.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Math/Vector4.h"
using namespace Mashiro;
using namespace Mashiro::Math;

Ball::Ball() : mode( Normal ), pos( 20 ), count( 3 ) {	
	//�����[�x�l���Q�T�ɐݒ�
	depth = static_cast< float >( DepthSingleton::instance()->getDepthMin() );
	//�{�[���̐���
	mBall.create( "res/model/Ball.pmd" );
	mBall.setPosition(Vector3(0.f,0.f,float(30+depth)));
	mBall.setAngle(Vector3(0.f,0.f,0.f));
	mBall.setColor(Vector3(1.f,1.f,1.f));
	mBall.setScale(Vector3(2.f));
	count = 0;
	color = 3;
	mode = Normal;
}

void Ball::Initi(){
	mode = Normal;
	color = 4;
	pos = 30+depth;
	mBall.setColor(Vector3(1.f,1.f,1.f));
}

void Ball::Update(Arm* rhand, Arm* lhand){ 
	if(mode == Release || mode == Catch)
	{
		Move();
		return;
	}

	if(rhand->Depth() ==0 ||
		 lhand->Depth() == 0) return;
	//���ꂼ��̘r���Œ�[�x�l�ɒB���Ă��邩�`�F�b�N
	KeepCheck(rhand);
	KeepCheck(lhand);

	if(rhand->Keep()&&lhand->Keep()){
		DepthChange(rhand,lhand);
		mBall.setColor(Vector3(0.0f, 1.0f, 0.0f));
		return;
	}

	//�r�̈ʒu�𓮂���
	if(rhand->Keep()){
		DepthChange(rhand,0);
		mBall.setColor(Vector3(1.0f, 0.0f, 0.0f));
		return;
	}
	if(lhand->Keep())
	{
		DepthChange(lhand,1);
		mBall.setColor(Vector3(0.0f, 0.0f, 1.0f));
		return;
	}
	mBall.setColor(Vector3(1.0f, 1.0f, 1.0f));
	mode = Normal;
}

void Ball::KeepCheck(Arm* hand){
	
	if(hand->Depth() <= DepthSingleton::instance()->getDepthMin()){
		//�ʂ��L���b�`
		hand->Keep(true);
		mode = Keep;
	}

	if(hand->Pos().x >=800 || hand->Pos().x <= 200){
		hand->Keep(false);
		mode = Normal;
	}
}

void Ball::DepthChange(Arm* hand,int n){
	if(!hand->Keep()) return;
	//�L���[�u�̐[�x�l��r�̐[�x�l�ɐݒ�
	if(hand->Depth() > depth)depth+=0.6f;
	if(hand->Depth() < depth)depth-=0.6f;
	mBall.setPosition(Vector3(0.f,0.f,DepthSingleton::instance()->getDepthMax()-depth + 30));
	hand->Keep(Check(n));
}

void Ball::DepthChange(Arm* rhand,Arm* lhand){
	if(rhand->Depth() < lhand->Depth())
	{
		if(rhand->Depth() > depth)depth+=0.6f;
		if(rhand->Depth() < depth)depth-=0.6f;
	}
	else{
		if(lhand->Depth() > depth)depth+=0.6f;
		if(lhand->Depth() < depth)depth-=0.6f;
	}
	mBall.setPosition(Vector3(0.f,0.f,DepthSingleton::instance()->getDepthMax()-depth + 30));
	Check(rhand,lhand);
}

bool Ball::Check(int n){
	//�E�肪���̋����𗣂ꂽ��
	if(depth<DepthSingleton::instance()->getDepthMax()) return true;
	//�{�[���𗣂�
	mode = Catch;
	color = n;
	return false; 
	
}
void Ball::Check(Arm* rhand,Arm* lhand){
	if(depth<DepthSingleton::instance()->getDepthMax()) return;

	rhand->Keep(false);
	lhand->Keep(false);
	color = 2;
	mode = Catch;
}

void Ball::Move(){
	if(mode == Catch){
		pos -= 2;
		if(pos <= 30){
			mode = Release;
			depth = static_cast< float >( DepthSingleton::instance()->getDepthMin() );
		}
		mBall.setPosition(Vector3(0.f,0.f,DepthSingleton::instance()->getDepthMax()-depth + 30));
		return;
	}
	pos += 2;
	if(pos>=+ DepthSingleton::instance()->getDepthMax() + 30) Initi();
	mBall.setPosition(Vector3(0.f,0.f,static_cast< float >( pos )));
	
}

Ball::~Ball(){
}

void Ball::Draw( TextureObj tex10, TextureObj tex01){	
	mBall.setTexture( tex01 );
	mBall.setTexture( tex10, 1 );
	mBall.draw( CocTrans::TYPE_BALL );	
}

int Ball::Color(){ 
	return color;
}

Mode Ball::IsMode(){ 
	return mode;
}

float Ball::Depth(){ 
	return DepthSingleton::instance()->getDepthMax()-depth;
}
