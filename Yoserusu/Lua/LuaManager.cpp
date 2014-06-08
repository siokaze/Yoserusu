#include "Mashiro/Mashiro.h"
#include "Mashiro/Math/Vector2.h"
#include "Mashiro/Math/Vector3.h"
#include "Mashiro/Graphics/Texture.h"
#include "Game/Enum.h"
#include "Util/TextureObj.h"
#include "Util/ModelObj.h"
#include "Util/Sprite.h"
#include "lua.hpp"
#include "luabind/luabind.hpp"
#include "boost/tuple/tuple.hpp"
#include "LuaManager.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/ParentGame.h"
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
	mLuaState = luaL_newstate();

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
			.def("drawRectangle", (void(SpriteUtil::*)(int, int, int, int))&SpriteUtil::drawRectangle)
			.def("draw", (void(SpriteUtil::*)(int, int))&SpriteUtil::draw),
		luabind::class_< TextureObj >( "TextureObj" )
			.def(luabind::constructor<const char*>()),
		luabind::class_< ModelObj >( "ModelObj" )
			.def(luabind::constructor<const char*>() )
			.def("setTexture", (void(ModelObj::*)(TextureObj, int))&ModelObj::setTexture)
		    .def("setTrance", (void(ModelObj::*)(float))&ModelObj::setTrance)
			.def("setColor", (void(ModelObj::*)(float, float, float))&ModelObj::setColor)
			.def("setPosition", (void(ModelObj::*)(const Vector3&))&ModelObj::setPosition )
			.def("setAngle", (void(ModelObj::*)(const Vector3&))&ModelObj::setAngle )
			.def("setScale", (void(ModelObj::*)(const Vector3&))&ModelObj::setScale )
			.def("draw", (void(ModelObj::*)(int))&ModelObj::draw),
		luabind::class_< Vector2 >( "Vector2" )
			.def( luabind::constructor< float, float >() )
			.def_readwrite( "x", &Vector2::x )
			.def_readwrite( "y", &Vector2::y ),
		luabind::class_< Vector3 >( "Vector3" )
			.def( luabind::constructor< float, float, float >() )
			.def_readwrite( "x", &Vector3::x )
			.def_readwrite( "y", &Vector3::y )
			.def_readwrite( "z", &Vector3::z ),
		luabind::class_< Sequence::Parent >( "Parent" )
			.def( "moveTo", (void(Sequence::Parent::*)(int))&Sequence::Parent::moveTo ),
		luabind::class_< Sequence::Game::ParentGame >( "ParentGame" )
			.def( "moveTo", (void(Sequence::Game::ParentGame::*)(int))&Sequence::Game::ParentGame::moveTo )
	];
}

LuaManager::~LuaManager(){
	//LuaのVMを閉じる
	if( mLuaTable ){
		SAFE_DELETE( mLuaTable );
	}
	lua_close( mLuaState );
}

void LuaManager::loadLua( const char* lua, const char* className ){
	int ret = -1;
	while( true ){
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

	mLuaTable = NEW LuaTable();
	mLuaTable->mTable = luabind::call_function<::luabind::object>( mLuaState, className );

	assert(luabind::type(mLuaTable->mTable) == LUA_TTABLE);
}

void LuaManager::deleteLua() {
	SAFE_DELETE( mLuaTable );
}
