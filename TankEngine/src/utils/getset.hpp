#pragma once


namespace Tank
{
	template <class T>
	class Get
	{
	private:
		T& m_gettableRef;
		std::function<const T&()> m_getter;
	public:
		Get(T &ref) : m_gettableRef(ref), m_getter([this]() { return m_gettableRef; }) {}
		Get(T &ref, std::function<const T&()> getter) : m_gettableRef(ref), m_getter(getter) {}

		// @todo
		// Not sure if this is robust enough: It fails in cases like
		// std::format("{}", x) if x is Get<float>.
		// It needs 2 conversions, only 1 conversion is allowed by compiler.
		///// <summary>
		///// Implicit conversion function to const T&.
		///// </summary>
		//operator const T&() const
		//{
		//	return m_gettableRef;
		//}

		/// <summary>
		/// Explicit operator() to const T&.
		/// </summary>
		const T& operator()() const
		{
			return m_getter();
		}
	};


	template <class T>
	class Set
	{
	private:
		T& m_settableRef;
	public:
		Set(T &ref) : m_settableRef(ref) {};


		/// <summary>
		/// Overwrite value, and return this.
		/// </summary>
		Set<T>& operator =(T newValue)
		{
			m_settableRef = newValue;
			return *this;
		}
	};


	template <class T>
	class GetSet : public Get<T>, public Set<T>
	{
	public:
		GetSet(T &ref) : Get<T>(ref), Set<T>(ref) {};
		GetSet(T &ref, std::function<const T&()> getter) : Get<T>(ref, getter), Set<T>(ref) {};


		using Set<T>::operator =;
	};
}