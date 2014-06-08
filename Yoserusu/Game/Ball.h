#ifndef BALL_H
#define BALL_H

#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Graphics/Texture.h"
#include "Game/Arm.h"
#include "Game/Enum.h"

#include "Util/ModelObj.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace Mashiro::Math;

class Ball{
public:
	Ball();
	~Ball();
	void Update(Arm* rhand, Arm* lhand);
	void Draw( Mashiro::Graphics::Texture, Mashiro::Graphics::Texture );
	int Color();
	Mode IsMode();
	void Initi();
	float Depth();

private:

	void DepthChange(Arm* hand,int n);
	void KeepCheck(Arm* hand);
	void DepthChange(Arm* rhand,Arm* lhand);
	void DepthMove(Arm* hand);
	void Move();

	bool Check(int n);
	bool Check(Arm* hand);
	void Check(Arm* rhand, Arm* lhand);

	int color;
	float depth;
	ModelObj mBall;
	Mode mode;
	int pos;
	int count;
};
#endif