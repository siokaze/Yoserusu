#ifndef INCLUDE_GAME_STATE_H_
#define INCLUDE_GAME_STATE_H_

#include "Game/Timer.h"
#include "Game/ArmLeft.h"
#include "Game/ArmRight.h"
#include "Mashiro/Graphics/Bitmap.h"

#include <memory>

class Ball;
class Wall;
class LockOn;

//ゲームプレイ本体
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
	std::unique_ptr< Ball > ball;
	std::unique_ptr< Wall > wall;
	std::unique_ptr< Timer > timer;
	std::unique_ptr< ArmLeft > lArm;
	std::unique_ptr< ArmRight > rArm;
	std::unique_ptr< LockOn > mLockOn;

};

#endif