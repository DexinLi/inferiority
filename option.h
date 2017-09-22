#pragma once
#include "utility.h"

namespace MySTL
{
	namespace
	{
		template<typename T>
		class large_option
		{
		public:
			large_option() = default;
			large_option(large_option &rhs) : m_element(new T(*(rhs.m_element))) {}
			large_option(large_option &&rhs) : m_element(rhs.m_element)
			{
				rhs.m_element = nullptr;
			}
			large_option(T &e) : m_element(new T(e)) {}
			large_option(T &&e) : m_element(new T(MySTL::forward<T>(e))) {}
			void operator=(large_option rhs)
			{
				swap(rhs);
			}
			void swap(large_option &rhs)
			{
				MySTL::swap(m_element, rhs.m_element);
			}
			T& getUnsafe()
			{
				return *m_element;
			}
			T& getOrElse(T &e)
			{
				if (m_element == nullptr)
				{
					return e;
				}
				return *m_element;
			}
			T getOrElse(T &&e)
			{
				if (m_element == nullptr)
				{
					return e;
				}
				return *m_element;
			}
			T* get()
			{
				return m_element;
			}
			bool isEmpty()
			{
				return m_element == nullptr;
			}
			~large_option()
			{
				delete m_element;
			}

		private:
			T *m_element;
		};

		template<typename T>
		class small_option
		{
		public:
			constexpr small_option() = default;
			constexpr small_option(small_option &rhs) :m_empty(rhs.m_empty), m_element(rhs.m_element) {}
			constexpr small_option(small_option &&rhs) : m_empty(rhs.m_empty), m_element(MySTL::forward<T>(rhs.m_element)) {}
			constexpr small_option(T &e) : m_empty(true), m_element(e) {}
			constexpr small_option(T &&e) : m_element(true), m_element(MySTL::forward<T>(e)) {}
			~small_option() = default;
			void operator=(small_option &rhs)
			{
				m_element = rhs.m_element;
				m_empty = rhs.m_empty;
			}
			void operator=(small_option &&rhs)
			{
				m_element = MySTL::forward<T>(rhs.m_element);
				m_empty = rhs.m_empty;
			}
			void swap(small_option &rhs)
			{
				MySTL::swap(m_element, rhs.m_element);
			}
			T& getUnsafe()
			{
				return m_element;
			}
			T& getOrElse(T &e)
			{
				if (m_empty)
				{
					return e;
				}
				return m_element;
			}
			T getOrElse(T &&e)
			{
				if (m_empty)
				{
					return e;
				}
				return m_element;
			}
			T* get()
			{
				return &m_element;
			}
			bool isEmpty()
			{
				return m_empty;
			}
		private:
			bool m_empty;
			T m_element;
		};

		template<bool B,class T>
		struct option_chooser
		{
			using type = large_option<T>;
		};
		template<class T>
		struct option_chooser<true,T>
		{
			using type = small_option<T>;
		};
	}
	template<typename T>
	class option
	{
	public:
		constexpr option() = default;
		constexpr option(T &e) :m_element(e) {}
		constexpr option(T &&e) :m_element(MySTL::forward<T>(e)) {}
		constexpr option(option &rhs) : m_element(rhs.m_element) {}
		constexpr option(option &&rhs) : m_element(MySTL::forward<T>(rhs.m_element)) {}
		~option() = default;
		void operator=(option &rhs)
		{
			m_element = rhs.m_element;
		}
		void operator=(option &&rhs)
		{
			m_element = MySTL::forward<T>(rhs.m_element);
		}
		void swap(option &rhs)
		{
			m_element.swap(rhs.m_element);
		}
		T& getUnsafe()
		{
			return m_element.getUnsafe();
		}
		T& getOrElse(T &e)
		{
			return m_element.getOrElse(e);
		}
		T getOrElse(T &&e)
		{
			return m_element.getOrElse(MySTL::forward<T>(e));
		}
		T* get()
		{
			return m_element.get();
		}
		bool isEmpty()
		{
			return m_element.isEmpty();
		}
	private:
		using C = typename option_chooser<sizeof(small_option<T>) <= 16, T>::type;
		C m_element;
	};
}
