#include "Mashiro/Mashiro.h"
#include "lua.hpp"
#include "luabind/luabind.hpp"
#include "LuaManager.h"
#include "Util/Sprite.h"
#include "Game/Enum.h"
using namespace Mashiro;

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
			.def("draw", (void(SpriteUtil::*)(int, int))&SpriteUtil::draw)
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

void LuaManager::runLua( const char* className, const char* functionName ){
	::lua_pcall( mLuaState, 0, 0, 0 );
	::luabind::object const tbl = ::luabind::call_function<::luabind::object>( mLuaState, className );
	
	assert(::luabind::type(tbl) == LUA_TTABLE);
	assert(::luabind::type(tbl[functionName]) == LUA_TFUNCTION);

	luabind::call_function<void>(tbl[functionName],tbl);
}