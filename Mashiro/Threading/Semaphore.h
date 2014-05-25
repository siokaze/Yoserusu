#ifndef INCLUDE_MASHIRO_THREADING_SEMAPHORE_H_
#define INCLUDE_MASHIRO_THREADING_SEMAPHORE_H_

namespace Mashiro {

namespace Threading {

class Semaphore {
public:
	static Semaphore create( int startCount, int maxCount = 0 );
	void decrease();
	void increase( int value = 1 );

	Semaphore();
	~Semaphore();
	Semaphore( const Semaphore& );
	operator void*() const;
	Semaphore& operator=( const Semaphore& );
	void release();
	bool operator==( const Semaphore& ) const;
	bool operator!=( const Semaphore& ) const;
private:
	class Impl;
	Impl* mImpl;
};

}
}

#endif