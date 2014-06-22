#include "Util/Sprite.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Graphics/SpriteManager.h"
using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Math;

SpriteUtil::SpriteUtil( const char* fileName ) : 
mColor( 1.f, 1.f, 1.f ),
mAlpha( 1.f ),
mScale( 1.f ),
mRadian(){
	mBitmap = Bitmap::create( fileName );
}

SpriteUtil::~SpriteUtil(){

}

void SpriteUtil::draw(int x, int y) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setBitmap( mBitmap );
	sp.setBitmapRectangle( Vector2( x, y ) );
	sp.setRotate( mRadian );
	sp.setScale( mScale.x, mScale.y );
	sp.draw();
}

void SpriteUtil::draw(const Bitmap& bitmap) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setBitmap( bitmap );
	sp.setBitmapRectangle( Vector2( 0, 0 ) );
	sp.setRotate( mRadian );
	sp.setScale( mScale.x, mScale.y );
	sp.draw();
}

void SpriteUtil::drawEllipse(int x, int y, int radiusX, int radiusY){
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setFillEllipse( Vector2( x, y ), Vector2( radiusX, radiusY ) );
	sp.draw();
}

void SpriteUtil::setColor(float r, float g, float b) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setColor( Vector3( r, g, b ) );
}

void SpriteUtil::setColor(const Vector3& color) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setColor( color );
}

void SpriteUtil::setTransparency(float t) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance( t );
}

void SpriteUtil::drawRectangle(int x, int y, int w, int h) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setFillRectangle( Vector2( x, y ), Vector2( w, h ) );
	sp.draw();
}

void SpriteUtil::create(const char* fileName) {
	mBitmap = Bitmap::create( fileName );
}

void SpriteUtil::setTexture( const Mashiro::Graphics::Bitmap& bitmap )
{
	if( mBitmap == bitmap ){
		return;
	}
	mBitmap = bitmap;
}

void SpriteUtil::setScale(float x, float b)
{
	mScale.set( x, b );
}

void SpriteUtil::setRotate(float radian)
{
	mRadian = radian;
}

