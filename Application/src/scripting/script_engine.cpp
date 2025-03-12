#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>
#include <mono/jit/jit.h>
#include "log.hpp"
#include "file.hpp"
#include "script_engine.hpp"
#include "script_glue.hpp"


/// <summary>
/// Credit: https://www.youtube.com/watch?v=EKnJOwGhPj4
/// </summary>
namespace Tank
{
	struct ScriptEngineData
	{
		MonoDomain *rootDomain = nullptr;
		MonoDomain *appDomain = nullptr;

		MonoAssembly *coreAssembly = nullptr;
		MonoImage *coreAssemblyImage = nullptr;

		ScriptClass nodeClass;
	};
	static ScriptEngineData *s_data = nullptr;


	static MonoAssembly *loadMonoAssembly(const std::filesystem::path &fp)
	{
		int fileSize = 0;
		char *fileData = File::readBytes(fp, &fileSize);

		MonoImageOpenStatus status;
		MonoImage *image = mono_image_open_from_data_full(fileData, fileSize, true, &status, false);

		if (status != MONO_IMAGE_OK)
		{
			TE_CORE_CRITICAL(mono_image_strerror(status));
			return nullptr;
		}

		MonoAssembly *assembly = mono_assembly_load_from_full(image, fp.string().c_str(), &status, false);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}


	static void printAssemblyTypes(MonoAssembly *assembly)
	{
		MonoImage *image = mono_assembly_get_image(assembly);
		const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int i = 0; i < numTypes; i++)
		{
			unsigned cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char *nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			TE_CORE_INFO(std::string{} + nameSpace + "." + name);
		}
	}


	void ScriptEngine::init()
	{
		s_data = new ScriptEngineData();
		initMono();
		loadAssembly(std::string{} + SCRIPTCORE_DIRECTORY + "Tank-ScriptCore.dll");

		ScriptGlue::registerFunctions();

		// Instantiate
		s_data->nodeClass = ScriptClass("Tank", "Node");
		MonoObject *instance = s_data->nodeClass.instantiate();
	}


	void ScriptEngine::initMono()
	{
		const std::string path = std::string{} + APP_DIRECTORY + "mono/lib";
		mono_set_assemblies_path(path.c_str());

		// Create root domain
		MonoDomain *rootDomain = mono_jit_init("TankJITRuntime");
		if (!rootDomain)
		{
			TE_CORE_CRITICAL("Failed to init script engine.");
			return;
		}
		s_data->rootDomain = rootDomain;
	}


	void ScriptEngine::shutdown()
	{
		shutdownMono();
		delete s_data;
	}


	void ScriptEngine::shutdownMono()
	{
		mono_jit_cleanup(s_data->appDomain);
		s_data->appDomain = nullptr;

		mono_jit_cleanup(s_data->rootDomain);
		s_data->rootDomain = nullptr;
	}


	void ScriptEngine::loadAssembly(const std::filesystem::path &fp)
	{
		// Create app domain
		char appDomainName[] = "TankScriptRuntime";
		s_data->appDomain = mono_domain_create_appdomain(appDomainName, nullptr);
		mono_domain_set(s_data->appDomain, true);

		// Load C# assembly
		s_data->coreAssembly = loadMonoAssembly(fp);
		s_data->coreAssemblyImage = mono_assembly_get_image(s_data->coreAssembly);
		//printAssemblyTypes(s_data->coreAssembly);
	}


	MonoObject *ScriptEngine::instantiateClass(MonoClass *monoClass)
	{
		MonoObject *instance = mono_object_new(s_data->appDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}


	ScriptClass::ScriptClass(const std::string &nameSpace, const std::string &name)
		: m_namespace(nameSpace), m_name(name)
	{
		m_monoClass = mono_class_from_name(s_data->coreAssemblyImage, nameSpace.c_str(), name.c_str());
	}


	MonoObject *ScriptClass::instantiate()
	{
		return ScriptEngine::instantiateClass(m_monoClass);
	}


	MonoMethod *ScriptClass::getMethod(const std::string &name, int paramCount)
	{
		return mono_class_get_method_from_name(m_monoClass, name.c_str(), paramCount);
	}


	MonoObject *invokeMethod(MonoObject *instance, MonoMethod *method, void **params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}
}