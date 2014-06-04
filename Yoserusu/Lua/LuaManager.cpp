#include "Mashiro/Mashiro.h"
#include "lua.hpp"
#include "luabind/luabind.hpp"
#include "LuaManager.h"
#include "Util/Sprite.h"
#include "Game/Enum.h"
#include "boost/tuple/tuple.hpp"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
using namespace Mashiro;
using namespace Mashiro::Math;

LuaManager* LuaManager::mInstance = nullptr;

void LuaManager::create(){
	ASSERT( !mInstance );
	mInstance = NEW LuaManager();
}

void LuaManager::destory(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

LuaManager* LuaManager::instance(){
	return mInstance;
}

LuaManager::LuaManager(){
	//luaVM�N��
	mLuaState = lua_open();

	//Lua�W�����C�u�������J��
	luaL_openlibs( mLuaState );

	luabind::open( mLuaState );
	//Luabind�Ɋ֐��o�^
	luabind::module( mLuaState )[
		luabind::class_<SpriteUtil>("SpriteUtil")
			.def(luabind::constructor<const char*>())
			.def("draw", (void(SpriteUtil::*)(int, int))&SpriteUtil::draw),
		luabind::class_< Vector2 >( "Vector2" )
			.def( luabind::constructor< float, float >() )
			.def_readwrite( "x", &Vector2::x )
			.def_readwrite( "y", &Vector2::y ),
		luabind::class_< Vector3 >( "Vector3" )
			.def( luabind::constructor< float, float, float >() )
			.def_readwrite( "x", &Vector3::x )
			.def_readwrite( "y", &Vector3::y )
			.def_readwrite( "z", &Vector3::z )
	];
}

LuaManager::~LuaManager(){
	//Lua��VM�����
	lua_close( mLuaState );
}

void LuaManager::loadLua( const char* lua ){
	int top = -1;
	int ret = -1;
	while( true ){
		top = lua_gettop(mLuaState);	//�X�^�b�N�̗v�f�����擾
		ret = luaL_dofile(mLuaState, lua);
		if (ret != 0) {
			//�G���[����
			const char* erro = lua_tostring(mLuaState,-1);
			MESSAGE( erro );
			printf("error:%s\n", erro );
		} else {
			break;
		}
	}
	lua_settop(mLuaState,top);
}
