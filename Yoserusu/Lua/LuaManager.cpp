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
	//luaVM起動
	mLuaState = lua_open();

	//Lua標準ライブラリを開く
	luaL_openlibs( mLuaState );

	luabind::open( mLuaState );
	//Luabindに関数登録
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
	//LuaのVMを閉じる
	lua_close( mLuaState );
}

void LuaManager::loadLua( const char* lua ){
	int top = -1;
	int ret = -1;
	while( true ){
		top = lua_gettop(mLuaState);	//スタックの要素数を取得
		ret = luaL_dofile(mLuaState, lua);
		if (ret != 0) {
			//エラー処理
			const char* erro = lua_tostring(mLuaState,-1);
			MESSAGE( erro );
			printf("error:%s\n", erro );
		} else {
			break;
		}
	}
	lua_settop(mLuaState,top);
}
