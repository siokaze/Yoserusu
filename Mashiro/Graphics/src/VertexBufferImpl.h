#ifndef INCLUDE_MASHIRO_GRAPHICS_VERTEXBUFFERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_VERTEXBUFFERIMPL_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/Vertex.h"
#include "Mashiro/Graphics/VertexBuffer.h"
#include "Mashiro/Graphics/src/Release.h"

using namespace Mashiro::Math;

namespace Mashiro{
namespace Graphics{

class VertexBuffer::Impl : public Mashiro::ReferenceType {
public:
	Impl( int vertexNumber, unsigned int size, ID3D10Device* device ) : 
	mVertexBuffer( 0 ),
	mSize( size ),
	mVertexNumber( vertexNumber ){
		createDxBuffer( device );
	}
	~Impl(){
		SafeRelease( &mVertexBuffer );
	}
	void createDxBuffer( ID3D10Device* device ){
		HRESULT hr;
		//BufferDesc�̍쐬
		D3D10_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = mSize * mVertexNumber;
		bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		//VertexBuffer�̍쐬
		hr = device->CreateBuffer(
			&bufferDesc,
			NULL,
			&mVertexBuffer );
		if( FAILED( hr ) ){
			ASSERT( hr && "CreateIndexBuffer Filed" );
		}
	}
	bool lock( void** buffer ){
		if( mVertexBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, buffer ) == S_OK ){
			return true;
		}
		return false;
	}
	void unlock(){
		mVertexBuffer->Unmap();
	}
	
	ID3D10Buffer* mVertexBuffer;
	unsigned int mSize;
	int mVertexNumber;
};

}
}

#endif