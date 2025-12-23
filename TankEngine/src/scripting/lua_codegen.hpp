#pragma once


namespace Tank
{
	struct LuaField
	{
		std::string name;
		std::string type;
	};

	struct LuaClass
	{
		std::string name;
		std::string base;
		std::vector<LuaField> fields;
	};

	std::ostream &operator<<(std::ostream &os, const LuaClass &lc);
	std::ostream &operator<<(std::ostream &os, const LuaField &lf);
}