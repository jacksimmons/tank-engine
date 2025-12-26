#pragma once


namespace Tank
{
	struct LuaField
	{
		std::string name;
		std::string type;
	};

	struct LuaFunction
	{
		std::string name;
		std::vector<LuaField> params;
		std::string returnType;
	};

	struct LuaClass
	{
		std::string name;
		std::string base;
		std::vector<LuaField> fields;
		std::vector<LuaFunction> functions;
	};

	std::ostream &operator<<(std::ostream &os, const LuaClass &lc);
	std::ostream &operator<<(std::ostream &os, const LuaField &lf);
}