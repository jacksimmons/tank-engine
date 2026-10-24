#pragma once


namespace Tank
{
	struct GlueVariable
	{
		std::string name;
		std::string type;
	};

	template <typename M>
	struct GlueField : public GlueVariable
	{
		M member;
	};


	template <typename Signature>
	struct GlueCtor
	{
		std::vector<GlueVariable> params;
	};

	template <typename M>
	struct GlueMethod
	{
		std::string name;
		std::string returnType;
		std::vector<GlueVariable> params;
		M member;
	};

	template <class T>
	concept EnumValue = requires(const T &t)
	{
		(unsigned)t;
	};
	template <EnumValue V>
	struct GluePair
	{
		std::string key;
		V value;
	};

	template <EnumValue E>
	struct GlueOperator
	{
		E enumValue;
		std::string returnType;
		std::vector<GlueVariable> params;
		//M member;
	};
}