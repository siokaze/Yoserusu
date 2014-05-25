#ifndef INCLUDE_MASHIRO_GRAPHICS_INDEXBUFFER_H_
#define INCLUDE_MASHIRO_GRAPHICS_INDEXBUFFER_H_

namespace Mashiro{
namespace Graphics{

class IndexBuffer {
public:
	static IndexBuffer create( int indexNumber );
	unsigned long* lock();
	void unlock( unsigned long** );
	int indexNumber();

	//à»â∫ÉÜÅ[ÉUÇÕãCÇ…Ç∑ÇÈÇ»
	IndexBuffer();
	~IndexBuffer();
	IndexBuffer( const IndexBuffer& );
	operator void*() const;
	IndexBuffer& operator=( const IndexBuffer& );
	void release();
	bool operator==( const IndexBuffer& ) const;
	bool operator!=( const IndexBuffer& ) const;
	class Impl;
	IndexBuffer( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

}
}

#endif