#pragma once
#include "lua_codegen.hpp"


namespace sol { class state; }
namespace Tank::UserTypes
{
	static std::vector<LuaClass> s_luaClasses;
	
	std::optional<LuaClass*> classFromName(const std::string &);
	void generate(sol::state &);

	void codegen();
}