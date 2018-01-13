#pragma once
#include"utility.h"
#include"string.h"
#include"list.h"
namespace MySTL
{
	template<typename Key>
	struct hash
	{
		size_t operator()(Key key)
		{
			return static_cast<size_t>(key);
		}
	};

	template<>
	struct hash<MySTL::string>
	{
		size_t operator()(MySTL::string &str)
		{
			size_t h = initial_val;
			for (size_t i = 0, e = str.length(); i != e; ++i)
			{
				h = ((h*a) + str[i]);
			}
			return h;

		}
	private:
		static const size_t initial_val = 0;
		static const size_t a = 31;
	};

	template<>
	struct hash< char *>
	{
		size_t operator()(const char* str)
		{
			size_t h = initial_val;
			for (size_t i = 0; str[i] != 0; ++i)
			{
				h = ((h*a) + str[i]);
			}
			return h;

		}
	private:
		static const size_t initial_val = 0;
		static const size_t a = 31;
	};



	

	
}