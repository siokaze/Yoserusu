#include "Mashiro/Mashiro.h"
#include "Mashiro/WorkSpace/WorkSpace.h"
#include "Mashiro/Input/InputManager.h"
#include "Mashiro/Input/Keyboard.h"
#include "Mashiro/Input/Mouse.h"
#include "Mashiro/Math/Functions.h"
#include "Mashiro/Kinect/KinectManager.h"
#include "Mashiro/Graphics/Bitmap.h"
#include "Mashiro/Graphics/SpriteManager.h"
#include "Mashiro/Graphics/GraphicsManager.h"
#include "Mashiro/Graphics/Shader.h"
#include "Sequence/Parent.h"
#include "Util/DepthSingleton.h"
#include "Util/SoundManager.h"
#include "Util/ModelLoader.h"
#include "Util/DataBase.h"
#include "Game/Score.h"
#include "Lua/LuaManager.h"
#pragma warning(disable : 4099)

using namespace Mashiro;
using namespace Mashiro::Math;
using namespace Mashiro::Input;

bool f = false;

Graphics::Shader gShader;

namespace Mashiro{
	void WorkSpace::configure(WorkSpace::Configuration* c)
	{
		c->setTitle("Yoserusu");
		c->setHeight(800);
		c->setWidth(1024);		
		c->enableWindowScreen(true);
		setFrameRate( 60 );
	}

	void WorkSpace::update(){
		if(!f){
			WorkSpace::kinectCreate();
			SoundManager::create();
			DepthSingleton::create();
			ModelLoader::create();
			LuaManager::create();
			Score::create();
			DataBase::create();
			DataBase::instance()->fileOpen( "res/ini/data.txt" );
			Sequence::Parent::create();
			f = true;

			gShader = Shader::create( ShaderType::SHADER_COCTRANS );
		}

		Graphics::Manager().setBackBufferTarget();

		Graphics::Manager().setShader( gShader);

		Sequence::Parent::instance()->update();

		Input::Keyboard ikb = Input::Manager::instance().keyboard();

		if(ikb.isTriggered(Input::Keyboard::KEY_ESC))
		{
			WorkSpace::requestEnd();
		}

		if(WorkSpace::isEndRequested())
		{
			Sequence::Parent::destroy();
			DataBase::destory();
			SoundManager::destory();
			DepthSingleton::destory();
			ModelLoader::destory();
			Score::destory();
			LuaManager::destory();
			gShader.release();
		}
	}
}
