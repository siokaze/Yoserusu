#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/Texture.h"
#include "Mashiro/Graphics/src/TextureImpl.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"

namespace Mashiro{

namespace Graphics {

Texture Texture::create( const char* fileName ){
	Texture r;
	r.mImpl = NEW Impl( fileName, gManagerImpl->dxDevice() );
	return r;
}

Texture Texture::create( int w, int h ){
	Texture r;
	r.mImpl = NEW Impl( w, h, gManagerImpl->dxDevice() );
	return r;
}

const char* Texture::name() const {
	ASSERT( mImpl && "Graphics::Texture : This is empty object." );
	return mImpl->mFileName;
}

Texture::Texture( const Impl* ) : mImpl( 0 ){
}

void Texture::unlock() {
	ASSERT( mImpl && "Graphics::Texture : This is empty object." );
	return mImpl->unLock();
}

unsigned char* Texture::lockChar() {
	ASSERT( mImpl && "Graphics::Texture : This is empty object." );
	return mImpl->lockChar();
}

void Texture::lock( unsigned int** ad, int* pitch, int mip ) {
	ASSERT( mImpl && "Graphics::Texture : This is empty object." );
	return mImpl->lock( ad, pitch, mip );
}

void Texture::lock( unsigned int** ad, int* pitch, int x, int y, int w, int h, int mip ){
	ASSERT( mImpl && "Graphics::Texture : This is empty object." );
	return mImpl->lock( ad, pitch, x, y, w, h, mip );
}

bool Texture::operator<( const Texture& o ) const {
	return ( mImpl < o.mImpl );
}

#define TYPE Texture
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}