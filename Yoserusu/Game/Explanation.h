#ifndef EXPLANATION_H
#define EXPLANATION_H

#include "Mashiro\Graphics\Bitmap.h"
#include "Game/Enum.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;

class Explanation{
	bool isCatch;
	int color;
	int count;

	Mashiro::Graphics::Bitmap mTextures[MAX];


public:
	Explanation();
	~Explanation();

	void Init();

	void ChangeTexture(int _color,bool _isCatch);

	void Draw();
};
#endif