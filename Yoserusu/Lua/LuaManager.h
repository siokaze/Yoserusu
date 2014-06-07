#ifndef INCLUDE_LUA_MANAGER_H_
#define INCLUDE_LUA_MANAGER_H_

#include "lua.hpp"
#include "luabind/luabind.hpp"

#include "Util/Util.h"

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
	* @tparam typename tuple_t - boost/tuple luaの引数格納
	* @tparam typename RetVal - 呼び出す関数の戻り値
	*/
	template< typename RetVal, typename tuple_t > RetVal runLua( const char* className, const char* functionName, tuple_t tuple ){
		::luabind::object const tbl = ::luabind::call_function<::luabind::object>( mLuaState, className );

		assert(::luabind::type(tbl) == LUA_TTABLE);
		assert(::luabind::type(tbl[functionName]) == LUA_TFUNCTION);

		return luabind::call_function<RetVal>(tbl[functionName], tbl, tuple.get<0>(), tuple.get<1>(), tuple.get< 2 >(), tuple.get< 3 >() );
	}
	/*
	* @brif lua内関数実行(引数なし)
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
private:
	LuaManager();
	~LuaManager();
	static LuaManager* mInstance;

	lua_State* mLuaState; 
};

#endif