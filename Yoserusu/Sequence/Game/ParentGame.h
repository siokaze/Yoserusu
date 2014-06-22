#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence/Child.h"
#include "Game/State.h"

namespace Sequence{
class Parent;
namespace Game{
class Child;

class ParentGame : public Sequence::Child{
public:
	typedef Sequence::Parent GrandParent;
	enum NextSequence{
		NEXT_LOAD,
		NEXT_AUTHE,
		NEXT_PLAY,
		NEXT_RESULT,

		NEXT_NONE,
	};
	ParentGame();
	~ParentGame();
	//ÉÅÉCÉìä÷êîåS
	void update( GrandParent* );
	void moveTo( NextSequence );
	void startLoading();

	State* getState();

	static void albumSave( const Mashiro::Graphics::Bitmap& );
	static Mashiro::Graphics::Bitmap getAlbum( int i );
private:
	NextSequence mNextSequence;
	Game::Child* mChild;

	State* mState;

	static int mAlbumCount;
public:
	static Mashiro::Graphics::Bitmap mAlbum[ 3 ]; //ÉAÉãÉoÉÄ

};

} //namespace Game
} //namespace Sequence

#endif