#include "Mashiro/Mashiro.h"
#include "Mashiro/Input/Keyboard.h"

#include "Mashiro/Input/src/InputManagerImpl.h"

namespace Mashiro{

namespace Input {	

bool Keyboard::isOn( int i ) const {
	return gManagerImpl->isKeyOn( i );
}

bool Keyboard::isTriggered( int i ) const {
	return gManagerImpl->isKeyTriggered( i );
}

}
}