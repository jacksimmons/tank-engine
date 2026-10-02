#pragma once


namespace Tank
{
	struct GlueVariable
	{
		const char *name;
		const char *type;
	};

	template <typename M>
	struct GlueField : public GlueVariable
	{
		M member;
	};


	template <typename M>
	struct GlueCtor
	{
		std::vector<GlueVariable> params;
		M member;
	};

	template <typename M>
	struct GlueMethod
	{
		const char *name;
		const char *returnType;
		std::vector<GlueVariable> params;
		M member;
	};

	struct GlueOp
	{
		unsigned enumName;
		const char *returnType;
		std::vector<GlueVariable> params;
		//M member;
	};


	template <typename V>
	struct GluePair
	{
		const char *key;
		V value;
	};
}