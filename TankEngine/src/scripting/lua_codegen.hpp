#pragma once


namespace Tank
{
	struct LuaField
	{
		std::string name;
		std::string type;
	};

	struct LuaCallable
	{
		std::string name;
		std::vector<LuaField> params;
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
		std::vector<LuaField> fields;
		std::vector<LuaField> globalFields;
		std::vector<LuaCallable> methods;
		std::vector<LuaCallable> metaMethods;
		std::vector<LuaCallable> staticMethods;
		std::vector<LuaCallable> globalFunctions;
	};

	std::ostream &operator<<(std::ostream &os, const LuaClass &lc);
	std::ostream &operator<<(std::ostream &os, const LuaField &lf);
}