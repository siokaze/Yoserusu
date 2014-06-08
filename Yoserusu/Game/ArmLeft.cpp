#include "Mashiro/Mashiro.h"
#include "Game/ArmLeft.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Math/Vector4.h"
#include "Mashiro/Scene/PMDFileLoader.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Util/ModelLoader.h"

#include "Util/KinectUtil.h"

#include "Shader/include/CocTrans.h"
using namespace Mashiro::Scene;

ArmLeft::ArmLeft(){
	wrist.model.create( "res/model/LeftHand.pmd" );
	shoulder.model.create("res/model/LeftShoulder.pmd");
	elbow.model.create("res/model/LeftElbow.pmd");
}

ArmLeft::~ArmLeft(){
}

void ArmLeft::Update(float depth,int number){

	Kinect::Manager kinect = Kinect::Manager::instance();
	Vector2 pos1(kinect.skeletonPos(Kinect::SKELETON_INDEX_HAND_LEFT));
	Vector2 pos2(kinect.skeletonPos(Kinect::SKELETON_INDEX_ELBOW_LEFT));

	if(kinect.depthSkeleton(number) < 250){
		this->depth = kinect.depthSkeleton(number);
	}

	if(pos1.x <= 200 ) {
		keep = false;
	}
	
	//	QDÀWðRDÀWÉÏ·
	Vector3 pos1super = Vector3(pos1.x/10-50,-pos1.y/10+15,depth*2);
	Vector3 pos2super = Vector3(-15,-15,0);
	
	//	pos1,po2Ì2_ÔÌ£ðßé
	Vector3 dir=Vector3(pos1super.x - pos2super.x,pos1super.y - pos2super.y,pos1super.z - pos2super.z );

	//	atan2ðg¢x²Ay²Ìpxðßé
	//X²
	float zxLength = sqrt(dir.x * dir.x + dir.z * dir.z);
	float tx = atan2(dir.y, zxLength);
	//y²
	float yxLength = sqrt(dir.y * dir.y + dir.x * dir.x);
	float ty = atan2(dir.x,dir.z);
	//pxðx@ÉÏ·
	tx=tx/3.14159265f*180.f;
	ty=ty/3.14159265f*180.f;

	shoulder.model.setScale( Vector3( 1.f ) );
	shoulder.model.setAngle( Vector3( 270 - tx, ty, 180 ) );
	shoulder.model.setPosition( Vector3( pos2super ) );

	elbow.model.setScale( Vector3( 1.f, 2.0f, 1.0f ) );
	elbow.model.setAngle( Vector3( 270 - tx, ty, 180 ) );
	elbow.model.setPosition( Vector3((pos1super.x+pos2super.x) /2,(pos1super.y+pos2super.y)/2+tx/5,(pos1super.z + pos2super.z)/2 ) );

	wrist.model.setScale( Vector3( 1.f ) );
	wrist.model.setAngle( Vector3( 270, 225, 180 ) );
	wrist.model.setPosition( Vector3(pos1super) );
}

Vector2 ArmLeft::Pos(){
	return Kinect::Manager::instance().skeletonPos(Kinect::SKELETON_INDEX_HAND_LEFT);
}

void ArmLeft::Draw()
{
	Graphics::Manager m = Graphics::Manager::instance();
	
	shoulder.model.setColor( Vector3(  0.f, 0.f, 1.f ) );
	wrist.model.setColor( Vector3( 0.f, 0.f, 1.f ) );
	elbow.model.setColor( Vector3( 0.f, 0.f, 1.f ) );

	shoulder.model.setTexture( mCubeTex, 2 );
	shoulder.model.draw( CocTrans::TYPE_ARM );
	

	wrist.model.setTexture( mCubeTex, 2 );
	wrist.model.draw( CocTrans::TYPE_ARM );


	elbow.model.setTexture( mCubeTex, 2 );
	elbow.model.draw( CocTrans::TYPE_ARM );
}