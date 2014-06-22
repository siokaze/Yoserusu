#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"
#include "Mashiro/Graphics/src/SpriteManagerImpl.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro/Window/Window.h"
#include "Mashiro/Graphics/Bitmap.h"

namespace Mashiro{

namespace Graphics{

SpriteImpl* gSpriteImpl = 0;

Sprite::Sprite(){
	//•Ê‚ÌƒXƒŒƒbƒhNG
	ASSERT( Window::isMainThread() && "you must call from MAIN thread" );
}

void Sprite::create(){
	STRONG_ASSERT( !gSpriteImpl );
	gSpriteImpl = NEW SpriteImpl( gManagerImpl->chain() );
}

void Sprite::destroy(){
	SAFE_DELETE( gSpriteImpl );
}

Sprite Sprite::instance(){
	return Sprite();
}

void Sprite::standby(){
	gSpriteImpl->standby();
}

void Sprite::setColor( const Vector3& color ){
	gSpriteImpl->setColor( color );
}

void Sprite::setTrance( float trans ){
	gSpriteImpl->setTrance( trans );
}

void Sprite::draw() const {
	gSpriteImpl->draw();
}

void Sprite::setBitmapRectangle( const Vector2& p ){
	gSpriteImpl->setBitmapRectangle( p );
}

void Sprite::setBitmapRectangle( const Vector2& p, const Vector2& size, const Vector4& rectA ){
	gSpriteImpl->setBitmapRectangle( p, size, rectA );
}

void Sprite::setFillEllipse( const Vector2& center, const Vector2& r ){
	gSpriteImpl->setFillEllipse( center, r );
}

void Sprite::setEllipse( const Vector2& center, const Vector2& r ){
	gSpriteImpl->setEllipse( center, r );
}

void Sprite::setLine( const Vector2& p0, const Vector2& p1 ){
	gSpriteImpl->setLine( p0, p1 );
}

void Sprite::setFillRectangle( const Vector2& p0, const Vector2& p1 ){
	gSpriteImpl->setFillRectangle( p0, p1 );
}

void Sprite::setRectangle( const Vector2& p0, const Vector2& p1 ) {
	gSpriteImpl->setRectangle( p0, p1 );
}

void Sprite::setBitmap( Bitmap bit ){
	gSpriteImpl->setBitmap( bit.mImpl );
}

void Sprite::setRotate(float radian)
{
	gSpriteImpl->setRotate( radian );
}

void Sprite::setScale(float x, float y)
{
	gSpriteImpl->setScale( x, y );
}



}
}