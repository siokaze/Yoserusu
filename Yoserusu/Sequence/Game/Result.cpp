#include "Mashiro/Mashiro.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Sequence/Game/Result.h"
#include "Sequence/Game/ParentGame.h"

#include "Util/SoundManager.h"
#include "Util/ModelLoader.h"
#include "Lua/LuaManager.h"

namespace Sequence{
namespace Game{

Result::Result(){
	LuaManager::instance()->loadLua( "Lua/Result.lua", "Result" );
}

Result::~Result(){
	LuaManager::instance()->deleteLua();
	SoundManager::instance()->stopBgm();
}

void Result::update( ParentGame* parent ){
	if( Mashiro::Input::Manager::instance().mouse().isTriggered( Input::Mouse::BUTTON_LEFT ) ){
		parent->moveTo( ParentGame::NEXT_AUTHE );
	}

	float pos = LuaManager::instance()->runLua<float>( "draw" );

	if(pos>20)
	{
		parent->moveTo( ParentGame::NEXT_AUTHE );
	}


}

} //namespace Game
} //namespace Sequence