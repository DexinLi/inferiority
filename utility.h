#pragma once
using size_t = decltype(sizeof(0));
namespace MySTL
{
	template<typename T>
	class reverse_iterator
	{
		T iterator;
	public:
		reverse_iterator(const T &iter) :iterator(iter) {}
		void operator=(const reverse_iterator &rhs)
		{
			iterator = rhs.iterator;
		}
		reverse_iterator operator++()
		{
			--iterator;
			return *this;
		}
		reverse_iterator operator++(int)
		{
			auto res = iterator;
			--iterator;
			return res;
		}
		reverse_iterator operator--()
		{
			++iterator;
			return *this;
		}
		reverse_iterator operator--(int n)
		{
			auto res = iterator;
			++iterator;
			return res;
		}
		reverse_iterator operator-(int n)
		{
			return iterator + n;
		}
		reverse_iterator operator+(int n)
		{
			return iterator - n;
		}

		void operator+=(int n)
		{
			iterator -= n;
		}

		void operator-=(int n)
		{
			iterator += n;
		}
		auto operator*()->decltype(*iterator)
		{
			return *iterator
		}
		auto operator->()->decltype(iterator.operator->())
		{
			return iterator.operator->();
		}
	private:

	};

	template<typename T, typename Num>
	T advance(T x, Num n)
	{
		for (auto i = 0; i < n; ++i)
		{
			++x;
		}
		return x;
	}


	template<typename T>
	struct less
	{
		bool operator()(const T&lhs, const T &rhs)
		{
			return lhs < rhs;
		}
	};

	template<typename T>
	struct equal_to
	{
		bool operator()(const T&lhs, const T &rhs)
		{
			return lhs == rhs;
		}
	};

	struct true_type
	{
		static const bool value = true;
	};

	struct false_type
	{
		static const bool value = false;
	};


	template< bool B, class T = void >
	struct enable_if;

	template<class T>
	struct enable_if<true, T>
	{
		using type = typename T;
	};
	template<bool B, class T>
	using enable_if_t = typename enable_if<B, T>::type;


	template<template<class> class T, class C>
	struct check_if
	{
		using type = typename enable_if_t<T<C>::value, C>;
	};

	template<template<class> class T, class C>
	using check_if_t = typename check_if<T, C>::type;

	namespace {
		template<class T> static true_type is_class_test(int T::*);
		template<class T> static false_type is_class_test(...);
	}

	template<class T>
	struct is_class
	{
		static const bool value = decltype(is_class_test<T>(0))::value;
	};

	template<class T>
	constexpr bool is_class_v = is_class<T>::value;
	#define HAS_MEMBER(name) template<class T>\
	struct has_member_##name_helper\
	{\
		template<class C>\
		static true_type test(decltype(&(C::##name)));\
		template<class C>\
		static false_type test(...);\
	};\
	template<class T>\
	struct has_member_##name\
	{\
		static const bool value = decltype(has_member_##name_helper<T>::test<T>(0))::value;\
	};
	namespace {
		template<class T>
		struct is_swappable_helper
		{
			template<class C>
			static true_type test(decltype(&(C::swap)));
			template<class C>
			static false_type test(...);
		};
		template<class T>
		struct is_swappable
		{
			static const bool value = decltype(is_swappable_helper<T>::test<T>(0))::value;
		};
		template<typename T, typename C>
		struct get_swap
		{
			static void swap(T&x, T&y)
			{
				T temp = MySTL::move(x);
				x = MySTL::move(y);
				y = MySTL::move(temp);
			}
		};

		template<typename T>
		struct get_swap<T, true_type>
		{
			static void swap(T&x, T&y)
			{
				x.swap(y);
			}
		};
	}


	template<typename T>
	inline void swap(T &x, T &y)
	{
		using namespace swap_impl;
		get_swap<T, is_swappable<T>::value>::swap(x, y);
	}

	template<class _Ty>
	struct remove_reference
	{	// remove reference
		typedef _Ty type;
	};

	template<class _Ty>
	struct remove_reference<_Ty&>
	{	// remove reference
		typedef _Ty type;
	};

	template<class _Ty>
	struct remove_reference<_Ty&&>
	{	// remove rvalue reference
		typedef _Ty type;
	};


	template<typename T>
	inline constexpr typename remove_reference<T>::type &&move(T &&x) noexcept
	{
		return static_cast<typename remove_reference<T>::type&&>(x);
	}

	template<typename T>
	inline constexpr T &&forward(typename remove_reference<T>::type &&x) noexcept
	{
		return static_cast<T&&>(x);
	}

	template<typename T>
	inline constexpr T &&forward(typename remove_reference<T>::type &x) noexcept
	{
		return static_cast<T&&>(x);
	}


	template <class T, class... Args>
	void construct(T* p, Args&&... args)
	{
		::new ((void*)p) T(MySTL::forward<Args>(args)...);
	}

	template <class T>
	void _fill_in(size_t n, T* dst, const T* src)
	{
		for (size_t i = 0; i < n; ++i)
		{
			construct(dst + i, src[i]);
		}
	}
	template<class T>
	T* _move_to(T* src, size_t begin, size_t origin, size_t fresh_begin, size_t fresh_size)
	{
		T* temp = (T*) ::operator new (fresh_size * sizeof(T));
		_fill_in(origin, temp + fresh_begin, src + begin);
		destruct_n(origin, src + begin);
		::operator delete(src);
		return T*;
	}

	template<class T, class...Args>
	void construct_n(size_t n, T* p, Args&&...args)
	{
		for (size_t i = 0; i < n; ++i)
		{
			construct(p + i, MySTL::forward<Args>(args)...);
		}
	}


	template<class T>
	void destruct(T* p)
	{
		p->~T();
	}

	template<class T>
	void destruct_n(size_t n, T* p)
	{
		for (size_t i = n - 1; i >= 0; ++i)
		{
			(p + i)->~T();
		}
	}

	template<typename T1, typename T2>
	struct pair
	{
		T1 first;
		T2 second;
		constexpr pair() :first(), second() {}
		constexpr pair(const T1 &val1, const T2 &val2) : first(val1), second(val2) {}
		constexpr pair(const T1 &val1, T2 &&val2) : first(val1), second(MySTL::forward<T2>(val2)) {}
		constexpr pair(T1 &&val1, const T2 &val2) : first(MySTL::forward<T1>(val1)), second(val2) {}
		constexpr pair(T1 &&val1, T2 &&val2) : first(MySTL::forward<T1>(val1)), second(MySTL::forward<T2>(val2)) {}

		pair(const pair&) = default;
		pair(pair&&) = default;
		void operator=(const pair &rhs) &
		{
			first = rhs.first;
			second = rhs.second;
		}

		void operator=(pair &&rhs) &
		{
			first = MySTL::forward < pair<T1, T2>>(rhs).first;
			second = MySTL::forward < pair<T1, T2>>(rhs).second;
		}
		constexpr bool operator<(const pair &rhs)
		{
			return (first == rhs.first) ? (second < rhs.second) : (first < rhs.first);
		}

		constexpr bool operator<=(const pair &rhs)
		{
			return (first == rhs.first) ? (second <= rhs.second) : (first <= rhs.first);
		}
		constexpr bool operator>(const pair &rhs)
		{
			return (first == rhs.first) ? (second > rhs.second) : (first > rhs.first);
		}

		constexpr bool operator>=(const pair &rhs)
		{
			return (first == rhs.first) ? (second >= rhs.second) : (first >= rhs.first);
		}

		constexpr bool operator==(const pair &rhs)
		{
			return (first == rhs.first)  &&(second == rhs.second);
		}

		void swap(const pair &rhs)
		{
			MySTL::swap(first, rhs.first);
			MySTL::swap(second, rhs.second);
		}

	};

	template<typename T1, typename T2>
	constexpr pair<T1, T2> make_pair(T1 val1, T2 val2)
	{
		return pair<T1, T2>(MySTL::move(val1), MySTL::move(val2));
	}

}
