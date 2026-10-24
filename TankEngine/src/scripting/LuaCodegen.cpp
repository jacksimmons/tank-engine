#include <Log.h>
#include "LuaCodegen.h"
#include <StringHelper.h>


#define SECTION(title) std::format("\n--- {}\n", title)


namespace Tank
{
	static void printLuaMethod(std::ostream &os, const LuaCallable &method)
	{
		GlueVariable paramAsVariable = { method.name, "" };
		for (const auto &param : method.params)
		{
			// If not the first arg, add a , separator
			if (!paramAsVariable.type.empty()) paramAsVariable.type += ",";
			paramAsVariable.type += std::format("{}: {}", param.name, param.type);
		}
		paramAsVariable.type = std::format("fun(this, {}): {}", paramAsVariable.type, method.returnType.empty() ? "nil" : method.returnType);
		os << paramAsVariable;
	}


	static void printLuaMetaMethod(std::ostream &os, const LuaOperator &metaMeth)
	{
		auto &metaFunctionNames = sol::meta_function_names();
		std::string name = metaFunctionNames[metaMeth.type];
		String::eraseAll(name, "__");

		std::string types;
		for (const auto &param : metaMeth.params)
		{
			if (!types.empty()) types += ",";
			types += param.type;
		}

		os << std::format("---@operator {}({}): {}", name, types, metaMeth.returnType) << std::endl;
	}


	std::ostream &operator<<(std::ostream &os, const GlueVariable &var)
	{
		os << std::format("---@field {} {}\n", var.name, var.type);
		return os;
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
		for (const auto &metaMeth : lc.operators)
		{
			printLuaMetaMethod(os, metaMeth);
		}

		os << SECTION("Static Fields/Methods");
		os << std::format("---@class (exact) {}", lc.name + "_Static") << std::endl;

		// Static Fields
		for (const auto &field : lc.staticFields)
		{
			os << field;
		}

		// Static Methods
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


	std::ostream &operator<<(std::ostream &os, const LuaEnum &le)
	{
		os << std::format("---@enum {}", le.name) << std::endl;
		os << std::format("{} = ", le.name) << "{" << std::endl;

		// Pairs
		for (const auto &pair : le.pairs)
		{
			os << std::format("\t{} = {},", pair.name, pair.value) << std::endl;
		}

		os << "}" << std::endl;
		return os;
	}
}