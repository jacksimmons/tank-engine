#include <sol/sol.hpp>
#include "LuaCodegen.h"
#include "GlueData.h"


// Get lua classes/enums by name
#define GET_SOL_CLASS(name) Tank::UserTypes::findItem<LuaClass>(UserTypes::s_luaClasses, name)
#define GET_SOL_ENUM(name) Tank::UserTypes::findItem<LuaEnum>(UserTypes::s_luaEnums, name)


namespace Tank
{
	/// @brief An API to setup sol2 usertypes and type hinting.
	/// 
	/// Think of it as "Glue::X = I want to Glue some X"
	/// E.g. "I want to Glue some fields"
	class Glue
	{
	public:
		template <typename T>
		static sol::usertype<T> newType(sol::state &lua, const char *className)
		{
			// Register in type hints
			UserTypes::s_luaClasses.push_back({ className });
			return lua.new_usertype<T>(className);
		}

		template <typename ...Vs>
		static void newEnum(sol::state &lua, const char *enumName, GluePair<Vs> ...pairs)
		{
			// Register in type hints
			UserTypes::s_luaEnums.push_back({ enumName, {} });
			
			// Register pairs in type hints
			((GET_SOL_ENUM(enumName)->pairs.push_back({ pairs.key, (unsigned)pairs.value })), ...);

			// Unpack pairs into enum constructor
			lua.new_enum(
				enumName,
				{ std::pair<std::string_view, Vs>{ pairs.key, pairs.value }... }
			);
		}

	private:
		template <typename T, typename ...MemberTypes>
		static void addMethods(sol::usertype<T> &ut, std::vector<LuaCallable> &methodsVector, GlueMethod<MemberTypes> ...methods)
		{
			// Register each method in type hints
			((methodsVector.push_back({ methods.name, methods.params, methods.returnType })), ...);

			// Register the methods in sol2
			// Expand the parameter pack, into format (ut[method0.name] = method0.member), (ut[method1.name] = method1.member), ...
			((ut[methods.name] = methods.member), ...);
		}


		template <typename T, typename ...MemberTypes>
		static void addFields(sol::usertype<T> &ut, std::vector<GlueVariable> &fieldsVector, GlueField<MemberTypes> ...fields)
		{
			// Register each field in type hints
			((fieldsVector.push_back({ fields.name, fields.type })), ...);

			// Register the fields in sol2
			// Expand the parameter pack, into format (ut[field0.name] = field0.member), (ut[field1.name] = field1.member), ...
			((ut[fields.name] = std::move(fields.member)), ...);
		}

	public:
		template <typename T, typename ...Signatures>
		static void ctors(sol::usertype<T> &ut, const char *className, GlueCtor<Signatures> ...ctors)
		{
			// Register each ctor in type hints only
			((GET_SOL_CLASS(className)->methods.push_back({ "new", ctors.params, className })), ...);

			// Register the ctors in sol2
			ut[sol::call_constructor] = sol::constructors<Signatures...>();
		}

		template <typename T, typename ...Ms>
		static void methods(sol::usertype<T> &ut, const char *className, GlueMethod<Ms> ...methods)
		{
			Glue::addMethods(ut, GET_SOL_CLASS(className)->methods, methods...);
		}

		template <typename T, typename ...Ms>
		static void staticMethods(sol::usertype<T> &ut, const char *className, GlueMethod<Ms> ...methods)
		{
			Glue::addMethods(ut, GET_SOL_CLASS(className)->staticMethods, methods...);
		}

		template <typename T, typename ...Ops>
		static void operators(sol::usertype<T> &ut, const char *className, GlueOperator<Ops> ...operators)
		{
			// Register each operator in type hints only
			((GET_SOL_CLASS(className)->operators.push_back({ (unsigned)operators.enumValue, operators.params, operators.returnType })), ...);
		}


		template <typename T, typename ...Ms>
		static void fields(sol::usertype<T> &ut, const char *className, const GlueField<Ms> &...fields)
		{
			Glue::addFields(ut, GET_SOL_CLASS(className)->fields, fields...);
		}

		template <typename T, typename ...Ms>
		static void staticFields(sol::usertype<T> &ut, const char *className, const GlueField<Ms> &...fields)
		{
			Glue::addFields(ut, GET_SOL_CLASS(className)->staticFields, fields...);
		}

		template <typename T, std::same_as<const char *> ...Names>
		static void globalInstances(sol::usertype<T> &ut, const char *className, Names ...instances)
		{
			// Register each field in type hints only
			((GET_SOL_CLASS(className)->globalFields.push_back({ instances, className })), ...);
		}
	};
}