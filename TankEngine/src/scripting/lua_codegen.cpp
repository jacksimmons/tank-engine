#include "lua_codegen.hpp"


namespace Tank
{
	std::ostream &operator<<(std::ostream &os, const LuaClass &lc)
	{
		os << std::format("---@class {}", lc.name) << (lc.base.empty() ? "\n" : (" : " + lc.base + "\n"));
		for (const auto &field : lc.fields)
		{
			os << field;
		}
		return os;
	}


	std::ostream &operator<<(std::ostream &os, const LuaField &lf)
	{
		os << std::format("---@field {} {}\n", lf.name, lf.type);
		return os;
	}
}