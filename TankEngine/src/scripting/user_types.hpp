#pragma once
#include "lua_codegen.hpp"


namespace sol { class state; }
namespace Tank::UserTypes
{
	static std::vector<LuaClass> s_luaClasses;

	void allGLM(sol::state &);
	void allNodes(sol::state &lua);
	void all(sol::state &);

	void codegen();
}