#pragma once


namespace sol { class state; }
namespace Tank
{
	namespace UserTypes
	{
		void allGLM(sol::state &);
		void allNodes(sol::state &lua);

		void all(sol::state &);
	};
}