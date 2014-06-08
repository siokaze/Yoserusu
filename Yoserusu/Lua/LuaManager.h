#ifndef INCLUDE_LUA_MANAGER_H_
#define INCLUDE_LUA_MANAGER_H_

#include "lua.hpp"
#include "luabind/luabind.hpp"

#include "Util/Util.h"

#include <memory>
#include <map>

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
	void loadLua( const char* lua, const char* className );
	/*
	* @brif luaState �j��
	*/
	void deleteLua();
	/*
	* @brif lua���֐����s
	* @param const char* className - �N���X�̖��O
	* @param const char* functionName - �֐���
	* @tparam typename tuple_t - boost/tuple lua�̈����i�[
	* @tparam typename RetVal - �Ăяo���֐��̖߂�l
	*/
	template< typename RetVal, typename tuple_t > RetVal runLua( const char* functionName, tuple_t tuple ){
		return luabind::call_function<RetVal>(mLuaTable->mTable[functionName], mLuaTable->mTable, tuple.get<0>(), tuple.get<1>(), tuple.get< 2 >(), tuple.get< 3 >() );
	}
	/*
	* @brif lua���֐����s(�����Ȃ�)
	* @param const char* className - �N���X�̖��O
	* @param const char* functionName - �֐���
	* @tparam typename RetVal - �Ăяo���֐��̖߂�l
	*/
	template< typename RetVal > RetVal runLua( const char* functionName ){
		return luabind::call_function<RetVal>(mLuaTable->mTable[functionName],mLuaTable->mTable);
	}
private:
	LuaManager();
	~LuaManager();
	static LuaManager* mInstance;

	//Lua���zVM
	lua_State* mLuaState; 

	//Lua�̉��z�e�[�u��
	struct LuaTable {
		luabind::object mTable;
	};
	LuaTable* mLuaTable;
};

#endif
