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
#include "Mashiro/Graphics/StringRender.h"
#include "Sequence/Parent.h"
#include "Util/DepthSingleton.h"
#include "Util/SoundManager.h"
#include "Util/ModelLoader.h"
#include "Util/DataBase.h"
#include "Game/Score.h"
#include "Lua/LuaManager.h"

#include "Util/Sprite.h"

#include "Shader/include/CocTrans.h"

#pragma warning(disable : 4099)

using namespace Mashiro;
using namespace Mashiro::Math;
using namespace Mashiro::Input;

bool f = false;

#if _DEBUG
Mashiro::Graphics::StringRender gStr;
#endif

namespace Mashiro{
	void WorkSpace::configure(WorkSpace::Configuration* c)
	{
		c->setTitle("Yoserusu");
		c->setHeight(800);
		c->setWidth(1024);		
		c->enableWindowScreen(true);
	}

	void WorkSpace::update(){
		if(!f){
			setFrameRate( 60 );
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
#if _DEBUG
			gStr = Mashiro::Graphics::StringRender::create( Font::FONT_GOSIC, 32 );
#endif

		}

		Graphics::Manager().setBackBufferTarget();



		Sequence::Parent::instance()->update();

		Input::Keyboard ikb = Input::Manager::instance().keyboard();

#if _DEBUG
		gStr << Mashiro::WorkSpace::instance().frameRate() << endl;
		gStr.setColor();
		gStr.draw();
#endif

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
#if _DEBUG
			gStr.release();
#endif
		}
	}
}
