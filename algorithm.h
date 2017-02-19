#pragma once
#include"functional.h"
#include"vector.h"
namespace MySTL
{
	template<class InputIt, class OutputIt>
	OutputIt copy(InputIt first, InputIt last,
		OutputIt d_first)
	{
		while (first != last) {
			*d_first = *first;
			++d_first;
			++first;
		}
		return d_first;
	}



	template <class RandomIt, class diff, class Func>
	void heaptify(RandomIt first, diff index, diff n, Func comp)
	{
		auto e = n / 2;
		while (index < e)
		{
			diff l = index * 2 + 1;
			diff r = l + 1;
			auto left = first + l;
			auto cur = first + index;
			if (r < n)
			{
				auto right = first + r;
				if (comp(*right, *left))
				{
					if (comp(*cur, *left))
					{
						MySTL::swap(*cur, *left);
						index = r;
					}
				}
				else
				{
					if (comp(*cur, *right))
					{
						MySTL::swap(*cur, *right);
						index = l;
					}
				}
			}
			else
			{
				if (comp(*cur, *left))
				{
					MySTL::swap(*cur, *left);
					index = l;
				}
			}
		}
	}

	template <class RandomIt, class diff>
	void heaptify(RandomIt first, diff index, diff n)
	{
		auto e = n / 2;
		while (index < e)
		{
			diff l = index * 2 + 1;
			diff r = l + 1;
			auto left = first + l;
			auto cur = first + index;
			if (r < n)
			{
				auto right = first + r;
				if (*right < *left)
				{
					if (*cur<*left)
					{
						MySTL::swap(*cur, *left);
						index = r;
					}
				}
				else
				{
					if (*cur < *right)
					{
						MySTL::swap(*cur, *right);
						index = l;
					}
				}
			}
			else
			{
				if (*cur < *left)
				{
					MySTL::swap(*cur, *left);
					index = l;
				}
			}
		}
	}


	template <class RandomIt>
	void make_heap(RandomIt first, RandomIt last)
	{
		auto n = last - first;
		if (n < 3)
		{
			return;
		}
		for (auto i = (n - 2) / 2; i >= 0; --i)
		{
			heaptify(first, i, n);
		}
	}

	template <class RandomIt, class Compare>
	void make_heap(RandomIt first, RandomIt last, Compare comp)
	{
		auto n = last - first;
		if (n < 3)
		{
			return;
		}
		for (auto i = (n - 2) / 2; i >= 0; --i)
		{
			heaptify(first, i, n, comp);
		}
	}

	template <class RandomIt>
	void push_heap(RandomIt first, RandomIt last)
	{
		for (auto n = last - first - 1; n >= 1;)
		{
			auto ac = n / 2;
			auto father = (first + ac);
			auto cur = (first + n);
			if (*cur < *father)
			{
				return;
			}
			MySTL::swap(*cur, *father);
			n = ac;
		}
	}

	template <class RandomIt, class Compare>
	void push_heap(RandomIt first, RandomIt last, Compare comp)
	{
		for (auto n = last - first - 1; n >= 1;)
		{
			auto ac = n / 2;
			auto father = (first + ac);
			auto cur = (first + n);
			if (!comp(*cur, *father))
			{
				return;
			}
			MySTL::swap(*cur, *father);
			n = ac;
		}
	}

	template <class RandomIt>
	void pop_heap(RandomIt first, RandomIt last)
	{
		auto n = last - first;
		if (n <= 1)
		{
			return;
		}
		MySTL::swap(*first, *(last - 1));
		heaptify(first, 0, n);
	}

	template <class RandomIt, class Compare>
	void pop_heap(RandomIt first, RandomIt last, Compare comp)
	{
		auto n = last - first;
		if (n <= 1)
		{
			return;
		}
		MySTL::swap(*first, *(last - 1));
		heaptify(first, 0, n, comp);
	}

	template <class RandomIt>
	void sort_heap(RandomIt first, RandomIt last)
	{
		make_heap(first, last);
		while (first != last)
		{
			pop_heap(first, last);
		}
	}
	template <class RandomIt, class Compare>
	void sort_heap(RandomIt first, RandomIt last,
		Compare comp)
	{
		make_heap(first, last, comp);
		while (first != last)
		{
			pop_heap(first, last, comp);
		}
	}


	template <class InputIterator1, class InputIterator2, class OutputIterator>
	void merge(InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first2 != last2)
		{
			if (first1 == last1)
			{
				return copy(first2, last2, result);

			}
			if (*first1 < *first2)
			{
				*result = *first1;
				++result;
				++first1;
			}
			else
			{
				*result = *first2;
				++result;
				++first2;
			}
		}
		return copy(first1, last1, result);
	}


	template< class BidirIt >
	void inplace_merge(BidirIt first, BidirIt middle, BidirIt last)
	{
		auto cur = middle;
		while (middle != first&&middle != last)
		{
			if (*middle < *first)
			{
				MySTL::swap(*first, *middle);
				++middle;
			}
			if (*cur < *first)
			{
				MySTL::swap(*first, *cur);
				++cur;
			}
			++first;
		}
	}

	template< class BidirIt, class Compare>
	void inplace_merge(BidirIt first, BidirIt middle, BidirIt last, Compare comp)
	{
		auto cur = middle;
		while (middle != first&&middle != last)
		{
			if (comp(*middle, *first))
			{
				MySTL::swap(*first, *middle);
				++middle;
			}
			if (comp(*cur, *first))
			{
				MySTL::swap(*first, *cur);
				++cur;
			}
			++first;
		}
	}

	template< class ForwardIt, class UnaryPredicate >
	ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
	{
		if (first == last)
		{
			return;
		}
		auto middle = first;
		++middle;
		while (middle != last)
		{
			while ((first != middle) && p(*first))
			{
				++first;
			}
			while ((middle != last) && (!p(*middle)))
			{
				++middle;
			}
			if (middle != last)
			{
				MySTL::swap(*first, *middle);
			}
		}
		return first;
	}

	template< class RandomIt >
	void sort(RandomIt first, RandomIt last)
	{
		if (first == last) return;
		auto pivot = *first;
		RandomIt middle1 = MySTL::partition(first, last,
			[pivot](const auto& x) { return x < pivot; });
		RandomIt middle2 = MySTL::partition(middle1, last,
			[pivot](const auto& x) { return !(pivot < x); });
		sort(first, middle1);
		sort(middle2, last);

	}

	template< class RandomIt, class Compare >
	void sort(RandomIt first, RandomIt last, Compare comp)
	{
		if (first == last) return;
		auto pivot = *first;
		RandomIt middle1 = MySTL::partition(first, last,
			[pivot,&comp](const auto& x) { return comp(x,pivot); });
		RandomIt middle2 = MySTL::partition(middle1, last,
			[pivot,&comp](const auto& x) { return !comp(pivot,x); });
		sort(first, middle1);
		sort(middle2, last);
	}


	/*template <class ForwardIt>
	ForwardIt rotate(ForwardIt first, ForwardIt n_first, ForwardIt last)
	{
		if (first == n_first)
		{
			return last;
		}
		if (n_first == last)
		{
			return first;
		}
		ForwardIt next = n_first;

	}*/







	template<class RandomIt, class T>
	RandomIt lower_bound(RandomIt first, RandomIt last, const T& value)
	{
		RandomIt it;
		size_t count, step;
		count = last - first;

		while (count > 0) {
			it = first;
			step = count / 2;
			it += step;
			if (*it < value) {
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}


	template<class RandomIt, class T, class Compare>
	RandomIt lower_bound(RandomIt first, RandomIt last, const T& value, Compare comp)
	{
		RandomIt it;
		size_t count, step;
		count = last - first;

		while (count > 0) {
			it = first;
			step = count / 2;
			it += step;
			if (comp(*it, value)) {
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}

	template<class RandomIt, class T>
	bool binary_search(RandomIt first, RandomIt last, const T& value)
	{
		first = MySTL::lower_bound(first, last, value);
		return (!(first == last) && !(value < *first));
	}


	template<class RandomIt, class T, class Compare>
	bool binary_search(RandomIt first, RandomIt last, const T& value, Compare comp)
	{
		first = MySTL::lower_bound(first, last, value, comp);
		return (!(first == last) && !(comp(value, *first)));
	}


	template<class RandomIt, class T>
	RandomIt upper_bound(RandomIt first, RandomIt last, const T& value)
	{
		RandomIt it;
		size_t count, step;
		count = last - first;

		while (count > 0) {
			it = first;
			step = count / 2;
			it += step;
			if (!(value < *it)) {
				first = ++it;
				count -= step + 1;
			}
			else count = step;
		}
		return first;
	}


	template<class RandomIt, class T, class Compare>
	RandomIt upper_bound(RandomIt first, RandomIt last, const T& value, Compare comp)
	{
		RandomIt it;
		size_t count, step;
		count = last - first;

		while (count > 0) {
			it = first;
			step = count / 2;
			it += step;
			if (!comp(value, *it)) {
				first = ++it;
				count -= step + 1;
			}
			else count = step;
		}
		return first;
	}

	template<class ForwardIt, class T>
	MySTL::pair<ForwardIt, ForwardIt>
		equal_range(ForwardIt first, ForwardIt last,const T& value)
	{
		return MySTL::make_pair(MySTL::lower_bound(first,last,value),
			MySTL::upper_bound(first, last, value));
	}


	template<class ForwardIt, class T, class Compare>
	MySTL::pair<ForwardIt, ForwardIt>
		equal_range(ForwardIt first, ForwardIt last,const T& value, Compare comp)
	{
		return MySTL::make_pair(MySTL::lower_bound(first, last, value, comp),
			MySTL::upper_bound(first, last, value, comp));
	}



}