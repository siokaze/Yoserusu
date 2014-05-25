#include "Mashiro/Mashiro.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Mashiro/Input/Joystick.h"

#include "Mashiro/Input/src/InputManagerImpl.h"

#include "Mashiro/Window/Window.h"

namespace Mashiro{

namespace Input {

ManagerImpl* gManagerImpl;

Manager::Manager(){
	//•Ê‚ÌƒXƒŒƒbƒhNG
	ASSERT( Window::isMainThread() && "you must call from MAIN thread" );
}

void Manager::create( void* wh ){
	STRONG_ASSERT( !gManagerImpl );
	gManagerImpl = NEW ManagerImpl( static_cast< HWND >( wh ) );
}

void Manager::destroy(){
	SAFE_DELETE( gManagerImpl );
}

void Manager::update( float pointerS, const Vector2& pointerOff ){
	gManagerImpl->update( pointerS, pointerOff );
}

Manager Manager::instance(){
	return Manager();
}

int Manager::joystickNumber() const {
	return gManagerImpl->mJoystickNumber;
}

Keyboard Manager::keyboard() const {
	return Keyboard();
}

Mouse Manager::mouse() const {
	return Mouse();
}

Joystick Manager::joystick( int i ) const {
	//ASSERT( i >= 0 && i < gManagerImpl->mJoystickNumber );
	return Joystick( &gManagerImpl->mJoysticks[ i ] );
}


}
}