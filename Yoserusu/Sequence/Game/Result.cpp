#include "Mashiro/Mashiro.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Sequence/Game/Result.h"
#include "Sequence/Game/ParentGame.h"

#include "Util/SoundManager.h"
#include "Util/ModelLoader.h"

namespace Sequence{
namespace Game{

Result::Result() : 
mBallPos( 0.0, -20.0, 0.0 ),
mBallAng( 0.0, 0.0, 0.0 ),
mBallScale( 0.5, 0.5, 0.5 ){
	mBall = std::unique_ptr< ModelObj >( NEW ModelObj( "res/model/Ball.pmd" ) );

	mStrTex = std::unique_ptr< SpriteUtil >( NEW SpriteUtil( "res/image/Ending.png" ) );
	mBackGround = std::unique_ptr< SpriteUtil >( NEW SpriteUtil(  "res/image/Ending_bg.png" ) );
}

Result::~Result(){
	SoundManager::instance()->stopBgm();
}

void Result::update( ParentGame* parent ){
	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( ParentGame::NEXT_AUTHE );
	}

	mBallAng.y = mBallAng.y + 3.0;

	if ( mBallPos.y < 20 ){
		mBallPos.y = mBallPos.y + 0.1 * 1.5;
	}

	mBackGround->setTransparency( 1.0 );
	mBackGround->draw( 0, 0 );

	//ƒ{[ƒ‹
	mBall->setPosition( mBallPos );
	mBall->setScale( mBallScale );
	mBall->setAngle( mBallAng );
	mBall->draw( CocTrans::TYPE_BALL );

	mStrTex->draw( 0, 0 );

	if( mBallPos.y > 20 )
	{
		parent->moveTo( ParentGame::NEXT_AUTHE );
	}


}

} //namespace Game
} //namespace Sequence