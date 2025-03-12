#include "script_glue.hpp"
#include <mono/metadata/object.h>
#include "nodes/node.hpp"


namespace Tank
{
#define TE_ADD_INTERNAL_CALL(signature, cppClass) mono_add_internal_call("Tank." #cppClass "::" #signature, signature)

	static void Node_SetName(Node *node, MonoString *name)
	{
		char *cName = mono_string_to_utf8(name);
		node->setName(cName);
	}


	static void Node_GetName(Node *node, MonoString *outName)
	{
		//*outName = mono_string_new( node->getName(cName);
	}


	void ScriptGlue::registerFunctions()
	{
		TE_ADD_INTERNAL_CALL(Node_SetName, Node);
		TE_ADD_INTERNAL_CALL(Node_GetName, Node);
	}
}