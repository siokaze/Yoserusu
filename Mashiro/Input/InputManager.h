#ifndef INCLUDE_MASHIRO_INPUT_MANAGER_H_
#define INCLUDE_MASHIRO_INPUT_MANAGER_H_

namespace Mashiro{
namespace Math {
	class Vector2;
}

namespace Input {
class Keyboard;
class Mouse;
class Joystick;

class Manager {
public:
	Manager();
	static Manager instance();

	Mouse mouse() const;
	Keyboard keyboard() const;
	Joystick joystick( int i = 0 ) const;
	int joystickNumber() const;

	//ˆÈ‰º’m‚ç‚È‚­‚Ä‚¨‚‹
	static void create( void* windowHandle );
	static void destroy();
	void update( float pointerScale, const Math::Vector2& pointeroffset );
};

}
}

#endif
