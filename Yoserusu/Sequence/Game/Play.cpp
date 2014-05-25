#include "Mashiro\Mashiro.h"
#include "Mashiro\Graphics\GraphicsManager.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro\WorkSpace\WorkSpace.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"

#include "Play.h"
#include "Game/State.h"
#include "Util/DepthSingleton.h"
#include "Util/SoundManager.h"
#include "Util/DataBase.h"
#include "Game/Score.h"

#include <sstream>

#include "Sequence/Game/Play.h"
#include "Sequence/Game/ParentGame.h"

using namespace Mashiro;
using namespace Mashiro::Graphics;
using namespace std;

namespace Sequence{
namespace Game{

Play::Play() : mCount( 0 ){
	ostringstream  oss;
	//mGoodTex = Mashiro::Graphics::Bitmap::create( "res/image/Clear.png" );
	//mBadTex = Mashiro::Graphics::Bitmap::create( "res/image/Failure.png");

	timeup = Mashiro::Graphics::Bitmap::create( "res/image/TimeUp.png" );

	mRank = Mashiro::Graphics::Bitmap::create( "res/image/Rank.png" );
	for(int i= 0; i<10; i++){
		oss.str( "" );
		oss << "res/image/score_" << i << ".png";
		mCountNum[i] = Mashiro::Graphics::Bitmap::create( oss.str().c_str() );
	}
	
	//音楽再生
	SoundManager::instance()->playBgm(SoundManager::BGM_GAMEPLAY);
	EndCount = 0;
	tenCount = 0;
	oneCount = 0;
	mPositionX = 0;

	endSeFlag = mFirst =  false;
	trance = 0.f;
	Score::instance()->Init();
}

Play::~Play(){
	SoundManager::instance()->stopBgm();
}

void Play::update( ParentGame* parent ){
	State* state = parent->getState();
	if( !mFirst ){
		state->init();
		mFirst = true;
	}

	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( ParentGame::NEXT_RESULT );
	}

	//ゲーム更新
	state->update();
	
	//描画
	state->draw();

	//タイムアップしてるなら表示処理
	if( state->timeUp() ){
		timeUpExecute( parent );
	}
}

void Play::timeUpExecute( ParentGame* parent ){
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	//少し待機
	if(EndCount <= 300)
	{
		if( !endSeFlag ){
			endSeFlag = true;
			SoundManager::instance()->playSe( SoundManager::SE_END );
		}
		EndCount++;
		mPositionX += 6;
		sp.setTrance( 1.f );
		sp.setBitmap( timeup );
		sp.setBitmapRectangle( Vector2( -780.f+static_cast<float>(mPositionX), -30.f ) );
		sp.draw();
		sp.setTrance( 1.f );
		sp.setBitmap( timeup );
		sp.setBitmapRectangle( Vector2( 1000.f-static_cast<float>(mPositionX), 600.f ) );
		sp.draw();
		return;
	}
	sp.setTrance( 1.f );
	sp.setBitmap( mRank );
	sp.setBitmapRectangle( Vector2( 0 ) );
	sp.draw();
	//二桁どうかチェック
	if( Score::instance()->getScore()> 9){
		
		int ten = Score::instance()->getScore() / 10;
		if(ten > tenCount) tenCount++;
		int one = Score::instance()->getScore() % 10;
		if(one > oneCount) oneCount++;
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNum[ tenCount ] );
		sp.setBitmapRectangle( Vector2(100,220));
		sp.draw();
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNum[ oneCount ] );
		sp.setBitmapRectangle( Vector2(400,220));
		sp.draw();
	}else{
		sp.setTrance( 1.f );
		sp.setBitmap( mCountNum[Score::instance()->getScore()] );
		sp.setBitmapRectangle( Vector2(400,200));
		sp.draw();
	}

	//コレ以上こえていればおめでとう！
	//if( Score::instance()->getScore() >= DataBase::instance()->clearLine() ){
	//	//おめでとう！！
	//	sp.setTrance( 1.f );
	//	sp.setBitmap( mGoodTex );
	//	sp.setBitmapRectangle( Vector2(100,550));
	//	sp.draw();
	//} else {
	//	//ざんねん！！
	//	sp.setTrance( 1.f );
	//	sp.setBitmap( mBadTex );
	//	sp.setBitmapRectangle( Vector2(100,550));
	//	sp.draw();
	//}
	if( mCount >= 480 ){
		mCount = 0;
		parent->moveTo( ParentGame::NEXT_RESULT );
	}
	//シーンカウント
	++mCount;
}

void Play::timeUpDraw(){
	Mashiro::Graphics::Sprite sp = Mashiro::Graphics::Sprite::instance();
	sp.draw();
}

}
}