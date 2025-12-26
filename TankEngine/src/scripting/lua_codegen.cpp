#include <log.hpp>
#include "lua_codegen.hpp"
#include <string.hpp>


#define SECTION(title) std::format("\n--- {}\n", title)


namespace Tank
{
	static void printLuaMethod(std::ostream &os, const LuaCallable &meth)
	{
		LuaField funcAsField;
		funcAsField.name = meth.name;

		funcAsField.type = "";
		for (const auto &param : meth.params)
		{
			// If not the first arg, add a , separator
			if (!funcAsField.type.empty()) funcAsField.type += ",";
			funcAsField.type += std::format("{}: {}", param.name, param.type);
		}
		funcAsField.type = std::format("fun(this, {}): {}", funcAsField.type, meth.returnType.empty() ? "nil" : meth.returnType);
		os << funcAsField;
	}


	static void printLuaMetaMethod(std::ostream &os, const LuaCallable &metaMeth)
	{
		std::string name = metaMeth.name;
		String::eraseAll(name, "__");

		std::string types;
		for (const auto &param : metaMeth.params)
		{
			if (!types.empty()) types += ",";
			types += param.type;
		}

		os << std::format("---@operator {}({}): {}", name, types, metaMeth.returnType) << std::endl;
	}


	std::ostream &operator<<(std::ostream &os, const LuaClass &lc)
	{
		os << SECTION("Fields/Methods");
		os << std::format("---@class (exact) {}", lc.name) << (lc.base.empty() ? "" : (" : " + lc.base)) << std::endl;

		// Fields
		for (const auto &field : lc.fields)
		{
			os << field;
		}

		// Methods
		for (const auto &meth : lc.methods)
		{
			printLuaMethod(os, meth);
		}
		for (const auto &metaMeth : lc.metaMethods)
		{
			printLuaMetaMethod(os, metaMeth);
		}

		// Static Methods
		os << SECTION("Static Methods");
		os << std::format("---@class (exact) {}", lc.name + "_Static") << std::endl;
		for (const auto &meth : lc.staticMethods)
		{
			printLuaMethod(os, meth);
		}

		os << SECTION("Static Reference");
		os << "---@type " << lc.name << "_Static" << std::endl;	// --- @type {name}_Static
		os << lc.name << " = {}" << std::endl;					// {name} = {}

		// Global Fields
		os << SECTION("Global Fields");
		for (const auto &field : lc.globalFields)
		{
			os << "---@type " << field.type << std::endl;
			os << field.name << " = {}" << std::endl;
		}

		// Global Functions
		os << SECTION("Global Functions");
		for (const auto &func : lc.globalFunctions)
		{
			std::string paramDocs;
			std::string args;

			for (const auto &param : func.params)
			{
				// Params (---@param)
				paramDocs += std::format("---@param {} {}\n", param.name, param.type);

				// Args (func(...))
				if (!args.empty()) args += ",";
				args += param.name + " ";
			}

			os << paramDocs << std::endl;
			os << std::format("function {}({}) end", func.name, args) << std::endl;
		}

		return os;
	}


	std::ostream &operator<<(std::ostream &os, const LuaField &lf)
	{
		os << std::format("---@field {} {}\n", lf.name, lf.type);
		return os;
	}
}