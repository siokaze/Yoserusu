#ifndef INCLUDE_LUA_MANAGER_H_
#define INCLUDE_LUA_MANAGER_H_

#include "lua.hpp"
#include "luabind/luabind.hpp"

class LuaManager{
public:
	/*
	* @brif �V���O���g������
	*/
	static void create();
	/*
	* @brif �V���O���g���j��
	*/
	static void destory();
	/*
	* @brif �C���X�^���X�擾
	* @return LuaManager* mInstance;
	*/
	static LuaManager* instance();
	/*
	* @brif lua�X�N���v�g���[�h
	*/
	void loadLua( const char* lua );
	/*
	* @brif lua���֐����s
	* @param const char* className - �N���X�̖��O
	* @param const char* functionName - �֐���
	* @tparam typename RetVal - �Ăяo���֐��̖߂�l
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