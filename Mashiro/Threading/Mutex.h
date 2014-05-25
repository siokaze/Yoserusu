#ifndef INCLUDE_MASHIRO_THREADING_MUTEX_H_
#define INCLUDE_MASHIRO_THREADING_MUTEX_H_

namespace Mashiro {

namespace Threading {

class Mutex {
public:
	static Mutex create();
	void lock();
	void unlock();

	Mutex();
	~Mutex();
	Mutex( const Mutex& );
	operator void*() const;
	Mutex& operator=( const Mutex& );
	void release();
	bool operator==( const Mutex& ) const;
	bool operator!=( const Mutex& ) const;
private:
	class Impl;
	Impl* mImpl;
};

}
}

#endif