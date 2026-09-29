#pragma once


namespace Tank
{
	/// @brief Serialises an obj into json. Compile error if no serialisation specialisation is provided.
	/// @tparam T 
	/// @param obj 
	/// @return 
	template <typename T>
	json serialise(T *outOf) = delete;

	/// @brief Deserialises json. Compile error if no deserialisation specialisation is provided.
	/// @tparam T 
	/// @param  
	/// @return 
	template <typename T>
	void deserialise(const json &, T *inTo) = delete;


	class TransformComponent;
	class TreeComponent;
	class AudioComponent;


	template <>
	json serialise<TransformComponent>(TransformComponent *);
	template <>
	void deserialise<TransformComponent>(const json &, TransformComponent *);

	template <>
	json serialise<TreeComponent>(TreeComponent *);
	template <>
	void deserialise<TreeComponent>(const json &, TreeComponent *);

	template <>
	json serialise<AudioComponent>(AudioComponent *);
	template <>
	void deserialise<AudioComponent>(const json &, AudioComponent *);
}