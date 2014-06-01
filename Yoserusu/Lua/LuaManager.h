#ifndef INCLUDE_LUA_MANAGER_H_
#define INCLUDE_LUA_MANAGER_H_

#include "lua.hpp"

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
	*/
	void runLua( const char* className, const char* functionName );
	/*
	* @brif lua���֐������s�������Ƃɒl�����o��
	*/
	template< typename Type > Type getLuaRetrun( int numArgs = 1 );
private:
	LuaManager();
	~LuaManager();
	static LuaManager* mInstance;

	lua_State* mLuaState; 
};

#endif