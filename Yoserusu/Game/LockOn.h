#ifndef INCLUDE_GAME_LOCKON_H_
#define INCLUDE_GAME_LOCKON_H_

#include "Mashiro/Graphics/Bitmap.h"

class LockOn {
public:
	LockOn();
	~LockOn();

	void setFlag();

	void update( bool flag );

	void draw();

	void draw(Vector2 pos);

private:
	Mashiro::Graphics::Bitmap mBitmap;
	bool mFlag;
	float mTrance;
};

#endif