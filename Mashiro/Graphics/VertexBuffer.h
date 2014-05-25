#ifndef INCLUDE_MASHIRO_GRAPHICS_VERTEXBUFFER_H_
#define INCLUDE_MASHIRO_GRAPHICS_VERTEXBUFFER_H_

#include "Mashiro/Graphics/Vertex.h"

namespace Mashiro{
namespace Graphics {

class VertexBuffer {
public:
	static VertexBuffer create( int vertexNumber, unsigned int size );
	bool lock( void** );
	void unlock();
	const char* name() const;
	int vertexNumber() const;

	//à»â∫ÉÜÅ[ÉUÇÕà”éØÇµÇ»Ç¢ä÷êîåQ
	VertexBuffer();
	VertexBuffer( const VertexBuffer& );
	~VertexBuffer();
	operator void*() const;
	VertexBuffer& operator=( const VertexBuffer& );
	void release();
	bool operator==( const VertexBuffer& ) const;
	bool operator!=( const VertexBuffer& ) const;

	class Impl;
	VertexBuffer( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

}
}

#endif