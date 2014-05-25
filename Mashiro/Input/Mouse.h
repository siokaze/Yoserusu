#ifndef INCLUDE_MASHIRO_INPUT_MOUSE_H_
#define INCLUDE_MASHIRO_INPUT_MOUSE_H_


namespace Mashiro{

namespace Input {

class Mouse {
public:
	enum Button {
		BUTTON_LEFT,
		BUTTON_RIGHT,
		BUTTON_MIDDLE,
	};
	int x() const;
	int y() const;
	int velocityX() const;
	int velocityY() const;
	int wheel() const;

	bool isOn( Button ) const;
	bool isTriggered( Button ) const;
};

}
}

#endif
