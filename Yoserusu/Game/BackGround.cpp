#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Game/BackGround.h"

BackGround::BackGround(){
	mBitmap = Mashiro::Graphics::Bitmap::create( "res/image/bg.png" );
}

BackGround::~BackGround(){

}

void BackGround::draw(){
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance( 1.f );
	sp.setBitmap( mBitmap );
	sp.setBitmapRectangle( Mashiro::Math::Vector2( 0 ) );
	sp.draw();
}