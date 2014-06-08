#ifndef INCLUDED_SEQUENCE_GAME_RESULT_H
#define INCLUDED_SEQUENCE_GAME_RESULT_H

#include "Sequence/Game/Child.h"

namespace Sequence{
namespace Game{
class ParentGame;

class Result : public Child{
public:
	Result();
	~Result();
	void update( ParentGame* );
	void rank( int good, int nice, int bad );
};

} //namespace Game
} //namespace Sequence

#endif