#pragma once


namespace Tank
{
	/// <summary>
	/// An object which can startup (when Play mode starts) and shutdown (when Play mode ends).
	/// </summary>
	class TANK_API IScriptable
	{
	public:
		virtual void startup() = 0;
		virtual void shutdown() = 0;
	};
}