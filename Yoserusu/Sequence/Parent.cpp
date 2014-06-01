#include "Mashiro/Mashiro.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/ParentGame.h"
#include "Sequence/AutheScene.h"
#include "Sequence/Title.h"

namespace Sequence{

Parent* Parent::mInstance = 0;

void Parent::create(){
	ASSERT( !mInstance );
	mInstance = NEW Parent();
}

void Parent::destroy(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

Parent* Parent::instance(){
	return mInstance;
}

Parent::Parent() : 
mNextSequence( NEXT_NONE ),
mChild( 0 ),
mStageID( 0 ){
	//�ŏ��ɍ��̂̓^�C�g��
	mChild = NEW AutheScene();
}

Parent::~Parent(){
	//�c���Ă���Ζ��E
	SAFE_DELETE( mChild );
}

void Parent::update(){
	mChild->update( this );
	//�J�ڔ���
	switch ( mNextSequence ){
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			mChild = NEW Title();
			break;
		case NEXT_GAME:
			SAFE_DELETE( mChild );
			mChild = NEW Game::ParentGame();
			break;
		case NEXT_AUTHE:
			SAFE_DELETE( mChild );
			mChild = NEW AutheScene();
			break;
	}
	mNextSequence = NEXT_NONE; //�߂�
}
void Parent::moveTo( NextSequence next ){
	if( mNextSequence != NEXT_NONE ){
		ASSERT( mNextSequence == NEXT_NONE ); //����ȊO���肦�Ȃ�
	}
	mNextSequence = next;
}


} //namespace Sequence
