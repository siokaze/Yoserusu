#include <Windows.h>
#undef min
#undef max
#include "Mashiro/Mashiro.h"
#include "Mashiro/Threading/Mutex.h"
#include "Mashiro/Base/Impl/ReferenceType.h"

namespace Mashiro {

namespace Threading {

class Mutex::Impl : public Mashiro::ReferenceType {
public:
	Impl() : mHandle( 0 ){
		mHandle = CreateSemaphore( NULL, 1, 1, NULL );
	}
	~Impl(){
		CloseHandle( mHandle );
	}
	void lock(){
		WaitForSingleObject( mHandle, INFINITE );
	}
	void unlock(){
		LONG prev;
		ReleaseSemaphore( mHandle, 1, &prev );
		STRONG_ASSERT( prev == 0 && "unlock() is called twice! Check lock-unlock pair." );
	}
	HANDLE mHandle;
};

Mutex Mutex::create(){
	Mutex r;
	r.mImpl = NEW Impl();
	return r;
}

void Mutex::lock(){
	mImpl->lock();
}

void Mutex::unlock() {
	mImpl->unlock();
}

#define TYPE Mutex
#include "Mashiro/Base/Impl/ReferenceTypeTemplate.h"

}
}