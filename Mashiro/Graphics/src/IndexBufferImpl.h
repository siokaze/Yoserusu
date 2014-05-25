#ifndef INCLUDE_MASHIRO_GRAPHICS_INDEXBUFFERIMPL_H_
#define INCLUDE_MASHIRO_GRAPHICS_INDEXBUFFERIMPL_H_

#include <d3d10_1.h>
#include <d3dx10.h>
#include "Mashiro/Base/Impl/ReferenceType.h"
#include "Mashiro/Graphics/IndexBuffer.h"

namespace Mashiro{
namespace Graphics{

class IndexBuffer::Impl : public Mashiro::ReferenceType {
public:
	Impl( int indexNumber, ID3D10Device* device ) : 
	mIndexBuffer( 0 ),
	mIndexNumber( indexNumber ){
		createIndex( device );
	}
	~Impl(){
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}
	void createIndex( ID3D10Device* device ){
		HRESULT hr;
		//BufferDesc‚Ìì¬
		D3D10_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof( unsigned long ) * mIndexNumber;
		bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bufferDesc.MiscFlags = 0;
		//IndexBufferì¬
		hr = device->CreateBuffer(
			&bufferDesc,
			NULL,
			&mIndexBuffer );
		if( FAILED( hr ) ){
			hr = E_FAIL;
			STRONG_ASSERT( hr != E_FAIL && "CreateIndexBuffer Filed" );
		}
	}
	unsigned long* lock(){
		void* r = 0;
		mIndexBuffer->Map( D3D10_MAP_WRITE_DISCARD, NULL, &r );
		return static_cast< unsigned long* >( r );
	}
	void unlock(){
		mIndexBuffer->Unmap();
	}

	ID3D10Buffer* mIndexBuffer;
	int mIndexNumber;
};

}
}

#endif