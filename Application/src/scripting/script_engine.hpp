#pragma once

#include <filesystem>


extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}
namespace Tank
{
	class ScriptEngine
	{
		friend class ScriptClass;
	public:
		static void init();
		static void shutdown();
		static void loadAssembly(const std::filesystem::path &fp);
	private:
		static void initMono();
		static void shutdownMono();
		static MonoObject *instantiateClass(MonoClass *monoClass);
	};


	class ScriptClass
	{
	private:
		std::string m_namespace;
		std::string m_name;
		MonoClass *m_monoClass = nullptr;
	public:
		ScriptClass() = default;
		ScriptClass(const std::string &nameSpace, const std::string &name);
		MonoObject *instantiate();
		MonoMethod *getMethod(const std::string &name, int paramCount);
		MonoObject *invokeMethod(MonoObject *instance, MonoMethod *method, void **params = nullptr);
	};
}