#include "Mashiro/Math/Vector2.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Game/LockOn.h"

LockOn::LockOn() : mTrance( 0.f ), mFlag( false ){
	mBitmap = Mashiro::Graphics::Bitmap::create( "res/image/Lock.png" );
}

LockOn::~LockOn(){

}

void LockOn::update( bool flag ){
	mFlag = flag;
}

void LockOn::draw(){
		Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	if( mFlag ){
		mTrance += 0.1f;
		sp.setTrance( mTrance );
	} else {
		mTrance = 0.f;
		sp.setTrance( mTrance );
	}
	int w = Mashiro::WorkSpace::instance().width() / 2;
	int h = Mashiro::WorkSpace::instance().height() / 2;
	sp.setBitmap( mBitmap );
	sp.setBitmapRectangle( Mashiro::Math::Vector2( w-160, h-160 ) );
	sp.draw();
}