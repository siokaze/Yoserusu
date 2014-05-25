#include "Mashiro/Mashiro.h"

#include "Mashiro/Graphics/src/SpriteManagerImpl.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Graphics/StringRender.h"
#include "Mashiro/Graphics/src/StringRenderImpl.h"

namespace Mashiro{

namespace Graphics {

StringRender StringRender::create( Font font, int size ) {
	StringRender r;
	r.mImpl = NEW Impl( font, size, gSpriteImpl->writeFac(), gSpriteImpl->backBuffer()  );
	return r;
}

StringRender& StringRender::operator<<( char a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( unsigned char a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( int a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( unsigned a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( short a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( unsigned short a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( float a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( double a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( const std::string& a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( const char* a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( char* a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << a;
	return *this;
}

StringRender& StringRender::operator<<( StringRender& ( *f )( StringRender& ) ){
	return ( *f )( *this );
}

StringRender& StringRender::endl(){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->mBuffer << '\n';
	return *this;
}

void StringRender::setColor( float r, float g, float b, float a ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->setColor( r, g, b, a );
}

void StringRender::draw( int x, int y ){
	ASSERT( mImpl && "Graphics::StringRender : This is empty object." );
	mImpl->draw( gSpriteImpl->backBuffer(), x, y );
}

StringRender& endl( StringRender& a ){
	return a.endl();
}

#define TYPE StringRender
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}