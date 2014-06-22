#ifndef INCLUDED_SEQUENCE_GAME_RESULT_H
#define INCLUDED_SEQUENCE_GAME_RESULT_H

#include "Sequence/Game/Child.h"

#include "Util/ModelObj.h"
#include "Util/Sprite.h"
#include <memory>
#include "boost/array.hpp"

namespace Sequence{
namespace Game{
class ParentGame;

class Result : public Child{
public:
	Result();
	~Result();
	void update( ParentGame* );
	void rank( int good, int nice, int bad );
private:
	std::unique_ptr< ModelObj > mBall;

	std::unique_ptr< SpriteUtil > mStrTex;
	std::unique_ptr< SpriteUtil > mBackGround;

	boost::array< std::unique_ptr< SpriteUtil >, 3 > mAlbum;

	Vector3 mBallPos;
	Vector3 mBallAng;
	Vector3 mBallScale;

	bool mFirst;
};

} //namespace Game
} //namespace Sequence

#endif