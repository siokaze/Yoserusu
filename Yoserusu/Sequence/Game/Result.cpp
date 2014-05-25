#include "Mashiro/Mashiro.h"
#include "Mashiro\Graphics\GraphicsManager.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Sequence/Game/Result.h"
#include "Sequence/Game/ParentGame.h"

#include "Util/SoundManager.h"
#include "Util/ModelLoader.h"

namespace Sequence{
namespace Game{

Result::Result(){
	ball = ModelLoader::instance()->createModel("res/model/Ball.pmd");
	mStrTex = Mashiro::Graphics::Bitmap::create("res/image/Ending.png");
	mBackGround = Mashiro::Graphics::Bitmap::create( "res/image/Ending_bg.png");

	SoundManager::instance()->playBgm(SoundManager::BGM_ENDING);
	ballPos =Vector3(0,-20,0);
	AngY=0;
}

Result::~Result(){
	SoundManager::instance()->stopBgm();
}

void Result::update( ParentGame* parent ){
	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( ParentGame::NEXT_AUTHE );
	}
	AngY +=3.0f;

	if(ballPos.y<20)
	{
		ballPos.y+=0.1f*1.5;
	}
	if(ballPos.y>20)
	{
		parent->moveTo( ParentGame::NEXT_AUTHE );
	}
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.setTrance(1.f);
	sp.setBitmap( mBackGround );
	sp.setBitmapRectangle(Vector2(0,0));
	sp.draw();
	
	//É{Å[Éã
	ball.setPosition(Vector3( ballPos ));
	ball.setScale(Vector3(0.5f));
	ball.setAngle(Vector3(0,AngY,-23.4f));
	ball.draw();
	sp.setBitmap( mStrTex );
	sp.setBitmapRectangle(Vector2(0));
	Graphics::Manager gm = Graphics::Manager::instance();
	sp.draw();
}

} //namespace Game
} //namespace Sequence