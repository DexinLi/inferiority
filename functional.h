#pragma once
#include"utility.h"
namespace inferiority
{
	namespace function_impl
	{
		template<class Ret, class... Args>
		class functor_base
		{
		protected:
			functor_base() = default;
		public:
			virtual Ret operator()(Args... args) = 0;
			virtual functor_base* copy_self() = 0;
		};

		template<class Func, class Ret, class... Args>
		class functor :public functor_base<Ret, Args...>
		{
		private:
			Func m_func;
		public:
			functor(const Func &func) : m_func(func) {}
			functor(Func &&func) : m_func(inferiority::forward<Func>(func)) {}
			Ret operator()(Args... args) override
			{
				return m_func(inferiority::forward<Args>(args)...);
			}
			functor_base* copy_self() override
			{
				return new functor(m_func);
			}
		};
	}
	using namespace function_impl;
	template<class>
	class function;

	template<class Ret, class... Args>
	class function<Ret(Args...)>
	{
		
	private:
		using func_type = Ret(*)(Args...);
		func_type m_function;
		functor_base<Ret, Args...> * m_functor;
	public:
		function() : m_function(nullptr), m_functor(nullptr) {}
		function(func_type func) : m_function(func),m_functor(nullptr){}
		template<class Func, class = inferiority::check_if_t<is_class,Func>>
		function(Func func) : m_function(nullptr)
		{
			m_functor = new functor<Func, Ret, Args...>(inferiority::move(func));
		}
		function(const function &rhs) : m_function(rhs.m_function), m_functor(nullptr)
		{
			if (rhs.m_functor != nullptr)
			{
				m_functor = rhs.m_functor->copy_self();
			}
		}
		function(function &&rhs) : m_function(rhs.m_function), m_functor(rhs.m_functor)
		{
			rhs.m_functor = nullptr;
		}
		void operator=(function rhs) &
		{
			swap(rhs);
		}
		Ret operator()(Args...args)
		{
			if (m_function != nullptr)
			{
				return m_function(inferiority::forward<Args>(args)...);
			}
			return (*m_functor)(inferiority::forward<Args>(args)...);
		}
		void swap(function &rhs)
		{
			inferiority::swap(m_function, rhs.m_function);
			inferiority::swap(m_functor, rhs.m_functor);
		}
		~function()
		{
			delete m_functor;
		}
	};



	/*Not finished yet, and I'm not willing to do that.
	// specialization for variadic functions such as std::printf
	template<class Ret, class... Args>
	class function<Ret(Args......)> {};

	// specialization for function types that have cv-qualifiers
	template<class Ret, class... Args>
	class function<Ret(Args...)const> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)volatile> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const volatile> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)volatile> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const volatile> {};

	// specialization for function types that have ref-qualifiers
	template<class Ret, class... Args>
	class function<Ret(Args...) &> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const &> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)volatile &> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const volatile &> {};
	template<class Ret, class... Args>
	class function<Ret(Args......) &> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const &> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)volatile &> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const volatile &> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)  &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)volatile &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const volatile &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)  &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)volatile &&> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const volatile &&> {};

	// specializations for noexcept versions of all the above (C++17 and later)

	template<class Ret, class... Args>
	class function<Ret(Args...) noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......) noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)volatile noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const volatile noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)volatile noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const volatile noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)volatile  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const volatile  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)volatile  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const volatile  &noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)volatile  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args...)const volatile  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)volatile  &&noexcept> {};
	template<class Ret, class... Args>
	class function<Ret(Args......)const volatile  &&noexcept> {};
	*/

}
