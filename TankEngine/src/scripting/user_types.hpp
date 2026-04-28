#pragma once
#include "lua_codegen.hpp"


template <class T>
concept Named = requires(const T &t)
{
	t.name;
};


template <class T>
concept StreamWritable = requires(std::ofstream &stream, const T &t)
{
	stream << t;
};


namespace sol { class state; }
namespace Tank::UserTypes
{
	static std::vector<LuaClass> s_luaClasses;
	static std::vector<LuaEnum> s_luaEnums;

	template <Named T>
	T *findItem(std::vector<T> &, const std::string &);
	template <class T>
	void generate(sol::state &);
	void generate(sol::state &);
	
	void codegen();


	template <StreamWritable T>
	void generateFile(const Res &res, const T &t);
}