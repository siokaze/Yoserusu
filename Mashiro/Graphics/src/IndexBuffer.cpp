#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/IndexBuffer.h"
#include "Mashiro/Graphics/src/IndexBufferImpl.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"

namespace Mashiro{
namespace Graphics{

IndexBuffer IndexBuffer::create( int indexNumber ){
	IndexBuffer r;
	r.mImpl = NEW Impl( indexNumber, gManagerImpl->dxDevice() );
	return r;
}

unsigned long* IndexBuffer::lock(){
	ASSERT( mImpl && "Graphics::IndexBuffer : This is empty object." );
	return mImpl->lock();
}

void IndexBuffer::unlock( unsigned long** p ){
	//TODO:デバグ時はロックで返したポインタを覚えておいてここで照合すべき。
	ASSERT( *p && "Graphics::IndexBuffer : give LOCKED pointer." );
	ASSERT( mImpl && "Graphics::IndexBuffer : This is empty object." );
	mImpl->unlock();
	*p = 0;
}

int IndexBuffer::indexNumber(){
	ASSERT( mImpl && "Graphics::IndexBuffer : This is empty object." );
	return mImpl->mIndexNumber;
}

#define TYPE IndexBuffer
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}