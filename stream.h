#include "functional.h"
#include "vector.h"
#include "optional.h"
namespace inferiority{
	//Write a macro named has_member()
	namespace
	{
		HAS_MEMBER(push)
		HAS_MEMBER(push_back)
		HAS_MEMBER(insert)
	}
	template <typename T>
	class stream
	{
		inferiority::function<option<T>()> m_get_next;
		template<typename Func>
		stream(Func func):m_get_next(func) {}
	public:
		template<typename Iter>
		stream(Iter begin, Iter end):m_get_next([=]()->optional<T>
		{
			if(begin == end)
			{
				return optional<T>();
			}
			optional<T> res(*begin);
			++begin;
			return res;
		}) {}
		template <class C,class = inferiority::check_if_t<inferiority::has_member_push,C>>
		C convert_to()
		{
			C res;
			while(auto val = m_get_next())
			{
				res.push(val);
			}
			return res;
		}
		template <class C,class = inferiority::check_if_t<inferiority::has_member_push_back,C>>
		C convert_to()
		{
			C res;
			while(auto val = m_get_next())
			{
				res.push_back(val);
			}
			return res;
		}
		template <class C,class = inferiority::check_if_t<inferiority::has_member_insert,C>>
		C convert_to()
		{
			C res;
			while(auto val = m_get_next())
			{
				res.insert(val);
			}
			return res;
		}
		template <class Func>
		auto map(Func func) &
		{
			using Ret = stream<decltype(func(m_get_next()))>;
			Ret res([m_get_next](){return func(m_get_next());});
			return res;
		}
		template <class Func>
		auto map(Func func) &&
		{
			using Ret = stream<decltype(func(m_get_next()))>;
			Ret res([m_get_next = inferiority::move(m_get_next)](){return func(m_get_next());});
			return res;
		}
		template<class Predict>
		stream<T> filter(Predict p) &
		{
			using Ret = stream<T>;
			Ret res([m_get_next]()->optional<T>
			{
				while(auto r = m_get_next();)
				{
					if(p(r))
					{
						return optional<T>(r);
					}
				}
				return optional<T>();
			});
			return res;
		}
		template<class Predict>
		stream<T> filter(Predict p) &&
		{
			using Ret = stream<T>;
			Ret res([m_get_next = inferiority::move(m_get_next)]()->optional<T>
			{
				while(auto r = m_get_next();)
				{
					if(p(r))
					{
						return optional<T>(r);
					}
				}
				return optional<T>();
			});
			return res;
		}
	};
}