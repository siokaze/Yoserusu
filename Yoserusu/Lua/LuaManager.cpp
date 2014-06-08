#include "Mashiro/Mashiro.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Graphics/Texture.h"
#include "Game/Enum.h"
#include "Util/ModelObj.h"
#include "Util/Sprite.h"
#include "lua.hpp"
#include "luabind/luabind.hpp"
#include "boost/tuple/tuple.hpp"
#include "LuaManager.h"
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
			.def(luabind::constructor<>())
			.def("setColor", (void(SpriteUtil::*)(float, float, float))&SpriteUtil::setColor)
			.def("setColor", (void(SpriteUtil::*)( const Vector3& ))&SpriteUtil::setColor)
			.def("setTransparency", (void(SpriteUtil::*)(float))&SpriteUtil::setTransparency)
			.def("drawEllipse", (void(SpriteUtil::*)(int, int, int, int))&SpriteUtil::drawEllipse)
			.def("draw", (void(SpriteUtil::*)(int, int))&SpriteUtil::draw),
		luabind::class_< ModelObj >( "ModelObj" )
			.def(luabind::constructor<const char*>() )
			.def("setTexture", (void(ModelObj::*)(Mashiro::Graphics::Texture, int))&ModelObj::setTexture)
		    .def("setTrance", (void(ModelObj::*)(float))&ModelObj::setTrance)
			.def("setColor", (void(ModelObj::*)(float, float, float))&ModelObj::setColor)
			.def("draw", (void(ModelObj::*)(int))&ModelObj::draw),
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
	if( mLuaTable ){
		SAFE_DELETE( mLuaTable );
	}
}

void LuaManager::loadLua( const char* lua, const char* className ){
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

	mLuaTable = NEW LuaTable();
	mLuaTable->mTable = luabind::call_function<::luabind::object>( mLuaState, className );

	assert(luabind::type(mLuaTable->mTable) == LUA_TTABLE);
}

void LuaManager::deleteLua() {
	SAFE_DELETE( mLuaTable );
}
