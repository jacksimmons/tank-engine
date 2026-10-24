#pragma once
#include "GlueData.h"


namespace Tank
{
	struct LuaCallable
	{
		std::string name;
		std::vector<GlueVariable> params;
		std::string returnType;
	};

	struct LuaOperator
	{
		unsigned type;
		std::vector<GlueVariable> params;
		std::string returnType;
	};

	/// <summary>
	/// Consists of all data relevant to one Lua class.
	/// Each class has its own file.
	/// </summary>
	struct LuaClass
	{
		std::string name;
		std::string base;
		std::vector<GlueVariable> fields;
		std::vector<GlueVariable> staticFields;
		std::vector<GlueVariable> globalFields;
		std::vector<LuaCallable> methods;
		std::vector<LuaOperator> operators;
		std::vector<LuaCallable> staticMethods;
		std::vector<LuaCallable> globalFunctions;
	};

	struct LuaEnumPair
	{
		std::string name;
		unsigned value;
	};

	struct LuaEnum
	{
		std::string name;
		std::vector<LuaEnumPair> pairs;
	};

	std::ostream &operator<<(std::ostream &os, const GlueVariable &var);
	std::ostream &operator<<(std::ostream &os, const LuaClass &lc);
	std::ostream &operator<<(std::ostream &os, const LuaEnum &le);
}