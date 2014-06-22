#ifndef INCLUDE_GAME_STATE_H_
#define INCLUDE_GAME_STATE_H_

#include "Game/Timer.h"
#include "Game/ArmLeft.h"
#include "Game/ArmRight.h"
#include "Mashiro/Graphics/Bitmap.h"

class Ball;
class Wall;
class LockOn;
class Explanation;

//�Q�[���v���C�{��
class State{
public:
	State();
	~State();

	void update();
	void draw();
	void init();
	bool timeUp() const;
	bool countDown() const;
	bool targetCheck() const;
private:
	Ball* ball;
	Wall* wall;
	Timer* timer;
	ArmLeft* lArm;
	ArmRight* rArm;
	LockOn* mLockOn;
	Explanation* mExplanation;

};

#endif