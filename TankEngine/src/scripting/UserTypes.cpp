#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <Log.h>
#include <fs/File.h>
#include <assets/Resource.h>
#include "UserTypes.h"
#include "sol/sol.hpp"
#include "LuaCodegen.h"
// The types to-be-defined as usertypes
#include <glm/glm.hpp>
#include <KeyInput.h>
#include <scene/GameEntity.h>
#include <components/Camera.h>
#include <components/Transform.h>
#include <components/Tree.h>
#include <static/Time.h>
#include "Glue.h"


#define PAIR(a, b) GluePair { a, b }
#define KEY(x) PAIR(#x, GLFW_KEY_##x)


namespace Tank
{
	template <Named T>
	T *UserTypes::findItem(std::vector<T> &items, const std::string &name)
	{
		// First UserType class whose name matches the key
		auto it = std::find_if(items.begin(), items.end(), [&name](const T &cls) { return name == cls.name; });
		if (it == items.end())
		{
			TE_CORE_ERROR(std::format("Codegen > Tried to find item with name {}, but it didn't exist.", name));
			return {};
		}

		return &(*it);
	}


	template<>
	void UserTypes::generate<glm::vec3>(sol::state &lua)
	{
		auto ut = Glue::newType<glm::vec3>(lua, "Vec3");
		Glue::ctors(ut, "Vec3",
			GlueCtor { {}, sol::constructors<glm::vec3()>() },
			GlueCtor { {{"x", "number"}, {"y", "number"}, {"z", "number"}}, sol::constructors<glm::vec3(float, float, float)>() }
		);
		Glue::fields(ut, "Vec3",
			GlueField { "x", "number", &glm::vec3::x },
			GlueField { "y", "number", &glm::vec3::y },
			GlueField { "z", "number", &glm::vec3::z }
		);
		Glue::operators(ut, "Vec3",
			GlueOp { sol::meta_function::addition, "Vec3", {{ "other", "Vec3" }} },
			GlueOp { sol::meta_function::subtraction, "Vec3", {{ "other", "Vec3" }} },
			GlueOp { sol::meta_function::multiplication, "Vec3", {{ "scalar", "float" }} },
			GlueOp { sol::meta_function::division, "Vec3", {{ "scalar", "float" }} },
			GlueOp { sol::meta_function::unary_minus, "Vec3" },
			GlueOp { sol::meta_function::equal_to, "Vec3", {{ "other", "Vec3" }} },
			GlueOp { sol::meta_function::to_string, "string" }
		);
	}

	template<>
	void UserTypes::generate<KeyState>(sol::state &lua)
	{
		// New KeyCode type to represent GLFW_KEY_ values
		Glue::newEnum(lua, "KeyCode",
			KEY(W),
			KEY(A),
			KEY(S),
			KEY(D)
		);

		Glue::newEnum(lua, "KeyState",
			PAIR("Held", (int)KeyState::Held),
			PAIR("Pressed", (int)KeyState::Pressed),
			PAIR("NotPressed", (int)KeyState::NotPressed),
			PAIR("Released", (int)KeyState::Released)
		);
	}

	template<>
	void UserTypes::generate<KeyInput>(sol::state &lua)
	{
		auto ut = Glue::newType<KeyInput>(lua, "KeyInput");
		Glue::methods(ut, "KeyInput",
			GlueMethod { "get_key_state", "KeyState", {{ "code", "KeyCode" }}, &KeyInput::getKeyState }
		);
	}

	template<>
	void UserTypes::generate<GameEntity>(sol::state &lua)
	{
		auto ut = Glue::newType<GameEntity>(lua, "GameEntity");
		Glue::fields(ut, "GameEntity",
			GlueField { "name", "string", sol::property(&GameEntity::name, &GameEntity::setName) },
			GlueField { "transform", "Transform", sol::property(&GameEntity::getComponent<TransformComponent>) },
			GlueField { "tree", "Tree", sol::property(&GameEntity::getComponent<TreeComponent>) },
			GlueField { "key_input", "KeyInput", sol::property(&GameEntity::keyInput) }
		);
		Glue::globalInstances(ut, "GameEntity", "entity");
	}

	template<>
	void UserTypes::generate<TransformComponent>(sol::state &lua)
	{
		auto ut = Glue::newType<TransformComponent>(lua, "Transform");
		Glue::fields(ut, "Transform",
			GlueField { "translation", "Vec3", &TransformComponent::Translation },
			GlueField { "rotation", "Vec3", &TransformComponent::Rotation },
			GlueField { "scale", "Vec3", &TransformComponent::Scale }
		);
	}

	template<>
	void UserTypes::generate<TreeComponent>(sol::state &lua)
	{
		auto ut = Glue::newType<TreeComponent>(lua, "Tree");
		Glue::methods(ut, "Tree",
			GlueMethod { "get_child", "GameEntity", {{ "index", "number" }}, static_cast<GameEntity * (TreeComponent:: *)(int) const>(&TreeComponent::getChild) },
			GlueMethod { "get_child", "GameEntity", {{ "name", "string" }}, static_cast<GameEntity * (TreeComponent:: *)(const std::string &) const>(&TreeComponent::getChild) }
		);
	}

	template<>
	void UserTypes::generate<Camera>(sol::state &lua)
	{
		auto ut = Glue::newType<Camera>(lua, "Camera");
		Glue::methods(ut, "Camera",
			GlueMethod { "set_pos", "", {{ "pos", "Vec3" }}, &Camera::setPosition }
		);
	}

	template<>
	void UserTypes::generate<Scene>(sol::state &lua)
	{
		auto ut = Glue::newType<Scene>(lua, "Scene");
		Glue::methods(ut, "Scene",
			GlueMethod { "active_camera", "Camera", {}, &Scene::getActiveCamera },
			GlueMethod { "current", "Scene", {}, &Scene::getActiveScene }
		);
	}

	template<>
	void UserTypes::generate<Time>(sol::state &lua)
	{
		auto ut = Glue::newType<Time>(lua, "Time");
		Glue::staticFields(ut, "Time",
			GlueField { "delta", "number", sol::property(&Time::getFrameDelta) }
		);
	}

	
	void UserTypes::generate(sol::state &lua)
	{
		generate<glm::vec3>(lua);
		
		generate<KeyState>(lua);
		generate<KeyInput>(lua);
		
		generate<GameEntity>(lua);
		generate<TransformComponent>(lua);
		generate<TreeComponent>(lua);
		
		generate<Scene>(lua);
		generate<Camera>(lua);

		generate<Time>(lua);
	}


	void UserTypes::codegen()
	{
		fs::path classes = fs::path{ "lua" } / "codegen" / "classes";
		fs::path enums = fs::path{ "lua" } / "codegen" / "enums";
		fs::create_directories(Res(classes, false).resolvePath());
		fs::create_directories(Res(enums, false).resolvePath());
		for (const LuaClass &lc : s_luaClasses)
		{
			Res codegenPath = Res(classes / (lc.name + ".lua"), false);
			generateFile(codegenPath, lc);
		}
		for (const LuaEnum &le : s_luaEnums)
		{
			Res codegenPath = Res(enums / (le.name + ".lua"), false);
			generateFile(codegenPath, le);
		}

		s_luaClasses.clear();
		s_luaEnums.clear();
	}


	template <StreamWritable T>
	void UserTypes::generateFile(const Res &res, const T &t)
	{
		TE_CORE_TRACE(std::format("Codegen > Generating file for type {}", t.name));
		std::ofstream stream;

		stream.open(res.resolvePathStr(), std::ofstream::out | std::ofstream::trunc);

		if (!stream)
		{
			TE_CORE_ERROR(std::format("Codegen > Error generating file for type {}", t.name));
			return;
		}

		stream << "---@meta (GENERATED)\n";
		stream << t << "\n";
		stream.close();
	}
}
