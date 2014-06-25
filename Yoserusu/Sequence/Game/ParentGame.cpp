#include "Sequence/Parent.h"
#include "Sequence/Game/ParentGame.h"
#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Game/Result.h"

#include "Mashiro/Mashiro.h"

namespace Sequence{
namespace Game{

int ParentGame::mAlbumCount = 0;
Mashiro::Graphics::Bitmap ParentGame::mAlbum[ 3 ] = {};

ParentGame::ParentGame() :
mNextSequence( NEXT_NONE ),
mChild( 0 ),
mState( 0 ){
	//�ŏ���Ready
	mChild = NEW Loading();
}

ParentGame::~ParentGame(){
	SAFE_DELETE( mChild );
	SAFE_DELETE( mState );
}

void ParentGame::update( GrandParent* parent ){
	mChild->update( this );
	//�J�ڔ���
	switch ( mNextSequence ){
		case NEXT_LOAD:
			SAFE_DELETE( mChild );
			mChild = NEW Loading();
			break;
		case NEXT_AUTHE:
			SAFE_DELETE( mChild );
			parent->moveTo( GrandParent::NEXT_AUTHE );
			break;
		case NEXT_PLAY:
			SAFE_DELETE( mChild );
			mChild = NEW Play();
			break;
		case NEXT_RESULT:
			SAFE_DELETE( mChild );
			mChild = NEW Result();
			break;
	}
	mNextSequence = NEXT_NONE;
}

void ParentGame::moveTo( NextSequence next ){
	if( mNextSequence != NEXT_NONE ){
		ASSERT( mNextSequence == NEXT_NONE ); //����ȊO���肦�Ȃ�
	}
	mNextSequence = next;
}

void ParentGame::startLoading(){
	if( mState ){
		SAFE_DELETE( mState );
	}
	mState = NEW State();
}

State* ParentGame::getState(){
	return mState;
}

void ParentGame::albumSave( const Mashiro::Graphics::Bitmap& bitmap ) {
	mAlbum[ mAlbumCount ] = bitmap;
	++mAlbumCount;
	if( mAlbumCount > 3 ){
		mAlbumCount = 0;
	}
}

Mashiro::Graphics::Bitmap ParentGame::getAlbum(int i)
{
	return mAlbum[ i ];
}

} //namespace Game
} //namespace Sequence