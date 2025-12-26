#include <log.hpp>
#include "lua_codegen.hpp"


#define SECTION(title) std::format("\n--- {}\n", title)


namespace Tank
{
	std::ostream &operator<<(std::ostream &os, const LuaClass &lc)
	{
		os << SECTION("Class Definition");
		os << std::format("---@class (exact) {}", lc.name) << (lc.base.empty() ? "\n" : (" : " + lc.base + "\n"));

		// Fields
		for (const auto &field : lc.fields)
		{
			os << field;
		}

		// Functions
		for (const auto &func : lc.functions)
		{
			LuaField funcAsField;
			funcAsField.name = func.name;

			funcAsField.type = "";
			for (const auto &param : func.params)
			{
				// If not the first arg, add a , separator
				if (!funcAsField.type.empty()) funcAsField.type += ",";
				funcAsField.type += std::format("{}: {}", param.name, param.type);
			}
			funcAsField.type = std::format("fun(this, {}): {}", funcAsField.type, func.returnType);
			os << funcAsField;
		}

		os << SECTION("Class Reference");
		os << "---@type " << lc.name << "\n";	// --- @type {name}
		os << lc.name << " = {}\n";				// {name} = {}
		os << "return " << lc.name << "\n";		// return {name}
		return os;
	}


	std::ostream &operator<<(std::ostream &os, const LuaField &lf)
	{
		os << std::format("---@field {} {}\n", lf.name, lf.type);
		return os;
	}
}