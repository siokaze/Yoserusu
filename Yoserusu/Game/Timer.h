#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

#include "Util/TextureObj.h"
#include "Mashiro/Graphics/Bitmap.h"
#include <map>

class Timer {
public:
	Timer();
	~Timer();

	void update();
	bool IsEnd(){ 
		if(mTime <= 0) return true;
		return false;
	}

	void setTime() {
		mTime = 60;
	}

	TextureObj draw_10();
	TextureObj draw_01();
	void startDraw();

	bool isStart();

	int time() const;

	void warrningCheck();
	void warrningDraw();
private:
	//タイマーMap
	typedef std::map< int, TextureObj > TextureMap;
	typedef std::map< int, Mashiro::Graphics::Bitmap > TexBitmap;
	TextureMap m01Tex; //一の位
	TextureMap m10Tex; //十の位

	TexBitmap mCountNumber;

	Mashiro::Graphics::Bitmap mRWarning;
	Mashiro::Graphics::Bitmap mLWarning;
	int mWarningpos;

	int mStartTimer;
	int mHeight;
	int mWidth;
	bool mFlag;
	int mTime;
	int mCount;
	unsigned int timer;

	bool mWarrningFlag;
	int mWarningCount;

	float mPitch;
};

#endif
