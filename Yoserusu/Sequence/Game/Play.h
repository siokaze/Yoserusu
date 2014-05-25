#ifndef INCLUDE_SEQUENCE_GAME_PLAY_H_
#define INCLUDE_SEQUENCE_GAME_PLAY_H_

#include "Sequence/Game/Child.h"
#include "Mashiro/Graphics/Bitmap.h"

namespace Sequence{
namespace Game{
class ParentGame;

class Play : public Child{
public:
	Play();
	~Play();
	void update( ParentGame* );
	//タイムアップシーン
	void timeUpExecute( ParentGame* );
	void timeUpDraw();

	void resultExeute();
	void resultDraw();
private:
	//ゲームが終わったあとの結果表示
	Mashiro::Graphics::Bitmap mRank;
	Mashiro::Graphics::Bitmap mCountNum[10];
	Mashiro::Graphics::Bitmap timeup;
	Mashiro::Graphics::Bitmap mGoodTex;
	Mashiro::Graphics::Bitmap mBadTex;

	int mCount;
	int EndCount;
	int tenCount;
	int oneCount;

	bool endSeFlag;
	float trance;

	int mPositionX;

	bool mFirst;
};

} //namespace Game
} //namespace Sequence

#endif