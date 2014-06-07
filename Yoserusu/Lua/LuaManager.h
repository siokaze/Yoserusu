#ifndef INCLUDE_LUA_MANAGER_H_
#define INCLUDE_LUA_MANAGER_H_

#include "lua.hpp"
#include "luabind/luabind.hpp"

#include "Util/Util.h"

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
	* @tparam typename tuple_t - boost/tuple lua�̈����i�[
	* @tparam typename RetVal - �Ăяo���֐��̖߂�l
	*/
	template< typename RetVal, typename tuple_t > RetVal runLua( const char* className, const char* functionName, tuple_t tuple ){
		::luabind::object const tbl = ::luabind::call_function<::luabind::object>( mLuaState, className );

		assert(::luabind::type(tbl) == LUA_TTABLE);
		assert(::luabind::type(tbl[functionName]) == LUA_TFUNCTION);

		return luabind::call_function<RetVal>(tbl[functionName], tbl, tuple.get<0>(), tuple.get<1>(), tuple.get< 2 >(), tuple.get< 3 >() );
	}
	/*
	* @brif lua���֐����s(�����Ȃ�)
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
private:
	LuaManager();
	~LuaManager();
	static LuaManager* mInstance;

	lua_State* mLuaState; 
};

#endif