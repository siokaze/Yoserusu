#ifndef INCLUDE_GAME_BACKGRAOUND_H_
#define INCLUDE_GAME_BACKGRAOUND_H_

#include "Mashiro/Graphics/Bitmap.h"

class BackGround {
public:
	BackGround();
	~BackGround();

	void draw();
private:
	Mashiro::Graphics::Bitmap mBitmap;
};

#endif