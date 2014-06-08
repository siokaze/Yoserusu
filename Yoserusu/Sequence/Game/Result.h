#ifndef INCLUDED_SEQUENCE_GAME_RESULT_H
#define INCLUDED_SEQUENCE_GAME_RESULT_H

#include "Sequence/Game/Child.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Scene/Model.h"
#include "Mashiro/Graphics/Bitmap.h"

#include "Util/ModelObj.h"

class Animation;
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

	ModelObj ball;
	Mashiro::Math::Vector3 ballPos;

	float AngY;

	Mashiro::Graphics::Bitmap mStrTex;
	Mashiro::Graphics::Bitmap mBackGround;

	int Execute1st();	
};

} //namespace Game
} //namespace Sequence

#endif