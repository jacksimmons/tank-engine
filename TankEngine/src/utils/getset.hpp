#pragma once


namespace Tank
{
	template <class T>
	using Getter = std::function<const T&(const T&)>;

	template <class T>
	class Get
	{
	private:
		T& m_gettableRef;
		Getter<T> m_getter;
	public:
		Get(T &ref) : m_gettableRef(ref), m_getter([](auto &ref) { return ref; }) {}
		Get(T &ref, Getter<T> getter) : m_gettableRef(ref), m_getter(getter) {}

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
			return m_getter(m_gettableRef);
		}
	};


	template <class T>
	using Setter = std::function<void(T&, const T&)>;

	template <class T>
	class Set
	{
	private:
		T& m_settableRef;
		Setter<T> m_setter;
	public:
		Set(T &ref) : m_settableRef(ref), m_setter([](T &oldVal, const T &newVal) { oldVal = newVal; }) {};
		Set(T &ref, Setter<T> setter) : m_settableRef(ref), m_setter(setter) {};


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
		GetSet(
			T &ref,
			Getter<T> getter,
			Setter<T> setter
		) : Get<T>(ref, getter), Set<T>(ref, setter) {};


		using Set<T>::operator =;
	};
}