#ifndef INCLUDE_LUA_MANAGER_H_
#define INCLUDE_LUA_MANAGER_H_

#include "lua.hpp"
#include "luabind/luabind.hpp"

class LuaManager{
public:
	/*
	* @brif シングルトン生成
	*/
	static void create();
	/*
	* @brif シングルトン破棄
	*/
	static void destory();
	/*
	* @brif インスタンス取得
	* @return LuaManager* mInstance;
	*/
	static LuaManager* instance();
	/*
	* @brif luaスクリプトロード
	*/
	void loadLua( const char* lua );
	/*
	* @brif lua内関数実行
	* @param const char* className - クラスの名前
	* @param const char* functionName - 関数名
	* @tparam typename RetVal - 呼び出す関数の戻り値
	*/
	template< typename RetVal > RetVal runLua( const char* className, const char* functionName ){
		::luabind::object const tbl = ::luabind::call_function<::luabind::object>( mLuaState, className );

		assert(::luabind::type(tbl) == LUA_TTABLE);
		assert(::luabind::type(tbl[functionName]) == LUA_TFUNCTION);

		return luabind::call_function<RetVal>(tbl[functionName],tbl);
	}
	template<> void runLua( const char* className, const char* functionName ){
		::luabind::object const tbl = ::luabind::call_function<::luabind::object>( mLuaState, className );

		assert(::luabind::type(tbl) == LUA_TTABLE);
		assert(::luabind::type(tbl[functionName]) == LUA_TFUNCTION);

		luabind::call_function<void>(tbl[functionName],tbl);
	}
private:
	LuaManager();
	~LuaManager();
	static LuaManager* mInstance;

	lua_State* mLuaState; 
};

#endif