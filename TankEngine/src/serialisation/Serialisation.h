#pragma once


namespace Tank
{
	class Serialisation
	{
	public:
		/// @brief Serialises an obj into json. Compile error if no serialisation specialisation is provided.
		/// @tparam T 
		/// @param obj 
		/// @return 
		template <typename T>
		static json serialise(T *in) = delete;

		/// @brief Deserialises json. Compile error if no deserialisation specialisation is provided.
		/// @tparam T 
		/// @param  
		/// @return 
		template <typename T>
		static T deserialise(const json &) = delete;
	};


	//template <typename T>
	//concept Serialisable = requires(T * t)
	//{
	//	Tank::serialise<T>(t);
	//};

	//template <typename T>
	//concept Deserialisable = requires(const json & j)
	//{
	//	Tank::deserialise<T>(j);
	//};
}