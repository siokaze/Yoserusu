#include "Util/Sprite.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Graphics/SpriteManager.h"
using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Math;

SpriteUtil::SpriteUtil( const char* fileName ) : 
mColor( 1.f, 1.f, 1.f ),
mAlpha( 1.f ){
	mBitmap = Bitmap::create( fileName );
}

SpriteUtil::~SpriteUtil(){

}

void SpriteUtil::draw(int x, int y) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance( 1.f );
	sp.setBitmap( mBitmap );
	sp.setBitmapRectangle( Vector2( x, y ) );
	sp.draw();
}

void SpriteUtil::draw(const Bitmap& bitmap) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance( 1.f );
	sp.setBitmap( bitmap );
	sp.setBitmapRectangle( Vector2( 0, 0 ) );
	sp.draw();
}

void SpriteUtil::drawEllipse(int x, int y, int radiusX, int radiusY){
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance( 1.f );
	sp.setFillEllipse( Vector2( x, y ), Vector2( radiusX, radiusY ) );
	sp.draw();
}

void SpriteUtil::setColor(float r, float g, float b) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setColor( Vector3( r, g, b ) );
}

void SpriteUtil::setTransparency(float t) {
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance( t );
}

