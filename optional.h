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
			explicit operator bool() const
			{
				return m_element != nullptr;
			}
			void swap(large_option &rhs)
			{
				MySTL::swap(m_element, rhs.m_element);
			}
			T &value()
			{
				return *m_element;
			}
			T &value_or(T &e)
			{
				if (m_element == nullptr)
				{
					return e;
				}
				return *m_element;
			}
			T value_or(T &&e)
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
			bool empty() const
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
			constexpr small_option(small_option &rhs)
			{
				bool m = rhs.m_mark();
				if(m)
				{
					container[ele_size - 1] = m;
					::new ((void *)container) T(rhs.m_element());
				}
			}
			constexpr small_option(small_option &&rhs)
			{
				bool m = rhs.m_mark();
				if(m)
				{
					container[ele_size - 1] = e;
					::new ((void *)container) T(MySTL::forward<T>(rhs.m_element()));
					rhs.m_mark() = false;
				}
			}
			constexpr small_option(T &e)
			{
				bool is_empty = e.empty();
				m_mark() = is_empty;
				if(is_empty)
				{

				}
			}
			constexpr small_option(T &&e) : m_element(true), m_element(MySTL::forward<T>(e)) {}
			~small_option()
			{
				if(!empty())
				{
					m_element().~T();
				}
			}
			void operator=(small_option rhs)
			{
				swap(rhs);
			}
			explicit constexpr operator bool() const
			{
				return m_mark();
			}
			void swap(small_option &rhs)
			{
				MySTL::swap(m_element(), rhs.m_element());
			}
			constexpr T &value()
			{
				return m_element();
			}
			constexpr T &value_or(T &e)
			{
				if (empty())
				{
					return e;
				}
				return m_element();
			}
			constexpr T value_or(T &&e)
			{
				if (empty())
				{
					return e;
				}
				return m_element();
			}
			constexpr bool empty() const
			{
				return !m_empty();
			}
		private:
			constexpr bool &m_mark()
			{
				return m_element[ele_size-1];
			}
			constexpr T &m_element()
			{
				auto ptr = (T *)container;
				return *ptr;
			}
			static const size_t ele_size = sizeof(T) + 1;
			char container[ele_size];
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
	class optional
	{
	public:
		constexpr optional() = default;
		constexpr optional(T &e) :m_element(e) {}
		constexpr optional(T &&e) :m_element(MySTL::forward<T>(e)) {}
		constexpr optional(optional &rhs) : m_element(rhs.m_element) {}
		constexpr optional(optional &&rhs) : m_element(MySTL::forward<T>(rhs.m_element)) {}
		~optional() = default;
		void operator=(optional &rhs)
		{
			m_element = rhs.m_element;
		}
		void operator=(optional &&rhs)
		{
			m_element = MySTL::forward<T>(rhs.m_element);
		}
		constexpr explicit operator bool() const
		{
			return (bool) m_element;
		}
		void swap(optional &rhs)
		{
			m_element.swap(rhs.m_element);
		}
		T &value()
		{
			return m_element.value();
		}
		T &value_or(T &e)
		{
			return m_element.value_or(e);
		}
		T value_or(T &&e)
		{
			return m_element.value_or(MySTL::forward<T>(e));
		}
		T* get()
		{
			return m_element.get();
		}
		bool empty() const
		{
			return m_element.empty();
		}
	private:
		using C = typename option_chooser<sizeof(small_option<T>) <= 16, T>::type;
		C m_element;
	};
}
