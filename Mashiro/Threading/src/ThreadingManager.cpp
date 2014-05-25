#include "Mashiro/Mashiro.h"
#include "Mashiro/Threading/ThreadingManager.h"
#include "Mashiro/Threading/src/ThreadingManagerImpl.h"
#include "Mashiro/Window/Window.h"

namespace Mashiro {
namespace Threading {

ManagerImpl* gManagerImpl = 0;

Manager::Manager() {
	//•Ê‚ÌƒXƒŒƒbƒhNG
	ASSERT( Window::isMainThread() && "you must call From MAIN thread" );
}

void Manager::create( int addThreadNumber ){
	STRONG_ASSERT( !gManagerImpl );
	gManagerImpl = NEW ManagerImpl( addThreadNumber );
}

int Manager::getCoreNumber() const {
	return gManagerImpl->getCoreNumber();
}

void Manager::destroy() {
	SAFE_DELETE( gManagerImpl );
}

Manager Manager::instance() {
	return Manager();
}

}
}