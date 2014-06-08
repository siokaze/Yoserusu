#ifndef INCLUDE_LUA_OBJECT_H_
#define INCLUDE_LUA_OBJECT_H_

#include "lua.hpp"
#include "luabind/luabind.hpp"

class LuaObject {
public:
	LuaObject();
	~LuaObject();

private:
	luabind::object mTable;
};

#endif