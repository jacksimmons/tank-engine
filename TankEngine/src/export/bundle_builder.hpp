#pragma once


namespace Tank
{
	class Scene;
	namespace Export
	{
		/// <summary>
		/// Exports your project into a Player Bundle.
		/// That bundle can then be played independently of the engine.
		/// 
		/// Expects the Player to have been built.
		/// </summary>
		class TANK_API BundleBuilder
		{
		private:
			BundleBuilder();

			static void prepareSerialisedData(const fs::path &);
		public:
			static bool build(Scene *, const fs::path &);
		};
	}
}