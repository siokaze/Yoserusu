#include "Mashiro/Mashiro.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/src/VertexBufferImpl.h"
#include "Mashiro/Graphics/src/GraphicsManagerImpl.h"

namespace Mashiro{
namespace Graphics {

VertexBuffer VertexBuffer::create( int vertexNumber, unsigned int size ){
	VertexBuffer r;
	r.mImpl = NEW Impl( vertexNumber, size, gManagerImpl->dxDevice() );
	return r;
}

bool VertexBuffer::lock( void** buffer ){
	ASSERT( mImpl && "Graphics::VertexBuffer : This is empty Object" );
	return mImpl->lock( buffer );
}

void VertexBuffer::unlock(){
	ASSERT( mImpl && "Graphics::VertexBuffer : This is empty object." );
	mImpl->unlock();
}

int VertexBuffer::vertexNumber() const {
	return mImpl->mVertexNumber;
}

#define TYPE VertexBuffer
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}