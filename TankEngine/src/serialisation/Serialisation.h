#pragma once


namespace Tank
{
	/// @brief Serialises an obj into json. Compile error if no serialisation specialisation is provided.
	/// @tparam T 
	/// @param obj 
	/// @return 
	template <typename T>
	json serialise(T *in) = delete;

	/// @brief Deserialises json. Compile error if no deserialisation specialisation is provided.
	/// @tparam T 
	/// @param  
	/// @return 
	template <typename T>
	void deserialise(const json &, T *out) = delete;
}