#pragma once
#include"utility.h"

namespace MySTL
{
	template<typename T>
	class list;

	template<typename T>
	struct list_node
	{
		T data;
		list_node* prev;
		list_node* next;
		list_node(list_node* p, const T &val, list_node* n) :data(val), prev(p), next(n) {}
		list_node(list_node* p, T &&val, list_node* n) :data(MySTL::forward<T>(val)), prev(p), next(n) {}
		list_node(list_node* p, list_node* n) :data(), prev(p), next(n) {}
	};

	template<typename T>
	class list_iterator
	{
		friend class list<T>;
	private:
		list_node<T> *m_iterator;

	public:

		list_iterator() = default;
		list_iterator(list_node<T> *ptr) :m_iterator(ptr) {}
		list_iterator(const list_iterator&) = default;
		void operator=(list_iterator rhs)
		{
			m_iterator = rhs.m_iterator;
		}

		void operator=(list_node<T> *rhs)
		{
			m_iterator = rhs;
		}
		bool operator==(list_iterator rhs)
		{
			m_iterator == rhs.m_iterator;
		}
		bool operator!=(list_iterator rhs)
		{
			m_iterator != rhs.m_iterator;
		}
		list_iterator operator++()
		{
			m_iterator = m_iterator->next;
			return m_iterator;
		}
		list_iterator operator++(int)
		{
			auto res = m_iterator;
			m_iterator = m_iterator->next;
			return res;
		}
		list_iterator operator--()
		{
			m_iterator = m_iterator->prev;
			return m_iterator;
		}
		list_iterator operator--(int)
		{
			auto res = m_iterator;
			m_iterator = m_iterator->prev;
			return res;
		}

		T &operator*()
		{
			return m_iterator->data;
		}

		T* operator->()
		{
			return &(m_iterator->data);
		}
	};

	template<typename T>
	class list
	{
		using list_base = list_node<T>;
	public:
		using iterator = list_iterator<T>;
		using reverse_iterator = reverse_iterator<iterator>;
		list() :m_size(0), m_begin((list_base*) ::operator new(sizeof(list_base))),
			m_end((list_base*) ::operator new(sizeof(list_base)))
		{
			_initial();
		}
		explicit list(size_t n) :m_size(n), m_begin((list_base*) ::operator new(sizeof(list_base))),
			m_end((list_base*) ::operator new(sizeof(list_base)))
		{
			_initial();
			iterator cur = m_begin;
			for (size_t i = 0; i < n; ++i)
			{
				_insert_between(cur, m_end);
			}
		}
		list(size_t n, const T &val) :m_size(n)
		{
			_initial();
			iterator cur = m_begin;
			for (size_t i = 0; i < n; ++i)
			{
				_insert_between(cur, val, m_end);
			}
		}
		list(const list &rhs) :m_size(rhs.m_size)
		{
			_initial();
			iterator cur = m_begin;
			for (iterator i = rhs.begin(), e = rhs.end(); i != e; ++i)
			{
				cur = _insert_between(cur, *i, m_end);
			}
		}
		list(list &&rhs) :m_size(rhs.m_size), m_begin(rhs.m_begin), m_end(rhs.m_end)
		{
			rhs.m_begin = nullptr;
			rhe.m_end = nullptr;
		}

		iterator begin()
		{
			return m_begin->next;
		}

		iterator end()
		{
			return m_end;
		}

		reverse_iterator rbegin()
		{
			return m_end.m_iterator->prev;
		}
		reverse_iterator rend()
		{
			return m_begin;
		}

		void operator=(list rhs)
		{
			swap(rhs);
		}

		void swap(list &rhs)
		{
			MySTL::swap(m_size, rhs.m_size);
			MySTL::swap(m_begin, rhs.m_begin);
			MySTL::swap(m_end, rhs.m_end);
		}

		bool empty()
		{
			return m_size == 0;
		}
		iterator insert(iterator pos, const T &value)
		{
			iterator prev = pos;
			--prev;
			iterator res = _insert_between(prev, val, pos);
			m_size += 1;
			return res;
		}
		iterator insert(iterator pos, T &&value)
		{
			iterator prev = pos;
			--prev;
			iterator res = _insert_between(prev, MySTL::forward<T>(val), pos);
			m_size += 1;
			return res;
		}
		iterator erase(iterator pos)
		{
			list_base *prev = pos.m_iterator->prev;
			list_base *next = pos.m_iterator->next;
			prev->next = next;
			next->prev = prev;
			delete pos.m_iterator;
			m_size -= 1;
		}
		iterator erase(iterator first, iterator last)
		{
			list_base *prev = first.m_iterator->prev;
			list_base *cur = nullptr;
			for (; first != last;)
			{
				cur = first.m_iterator;
				++first;
				delete cur;
			}
			prev->next = last.m_iterator;
			last.m_iterator->prev = prev;
			return last;
		}
		void clear()
		{
			list_base *cur = nullptr;
			for (iterator i = begin(), e = end(); i != e;)
			{
				cur = i.m_iterator;
				++i;
				delete cur;
			}
			m_begin.m_iterator->next = m_end.m_iterator;
			m_end.m_iterator->prev = m_begin.m_iterator;
			m_size = 0;
		}
		void push_back(const T &val)
		{
			insert(end(), val);
		}
		void push_back(T &&val)
		{
			insert(end(), MySTL::forward<T>(val));
		}
		void pop_back()
		{
			iterator pos = end();
			--pos;
			erase(pos);
		}
		void pop_front()
		{
			erase(begin());
		}
		void push_front(const T &val)
		{
			_insert_between(m_begin, val, begin());
			m_size -= 1;
		}
		void push_front(T &&val)
		{
			_insert_between(m_begin, MySTL::forward<T>(val), begin());
			m_size -= 1;
		}
		void splice(iterator pos, list &other)
		{
			list_base* prev = pos.m_iterator->prev;
			iterator first = other.begin();
			prev->next = first.m_iterator;
			first.m_iterator->prev = prev;
			iterator last = --(other.end());
			last.m_iterator->next = pos.m_iterator;
			pos.m_iterator->prev = last.m_iterator;
			other.m_begin.m_iterator->next = other.m_end.m_iterator;
			other.m_end.m_iterator->prev = other.m_begin.m_iterator;
		}
		void splice(iterator pos, list &&other)
		{
			list_base* prev = pos.m_iterator->prev;
			iterator first = other.begin();
			prev->next = first.m_iterator;
			first.m_iterator->prev = prev;
			iterator last = --(other.end());
			last.m_iterator->next = pos.m_iterator;
			pos.m_iterator->prev = last.m_iterator;
			other.m_begin.m_iterator->next = nullptr;
			other.m_end.m_iterator->prev = nullptr;
		}
		T &front()
		{
			return *begin();
		}
		T &back()
		{
			return m_end.m_iterator->prev->data;
		}
		void reverse()
		{
			for (iterator i = m_begin, e = m_end; i != e;)
			{
				list_base* cur = i.m_iterator;
				list_base* next = cur->next;
				cur = next;
				MySTL::swap(cur->prev, cur->next);
				i = cur;
			}
			MySTL::swap(m_end.m_iterator->prev, m_end.m_iterator->next);
			MySTL::swap(m_begin, m_end);
		}
		~list()
		{
			list_base* cur = nullptr;
			for (iterator i = begin(), e = end(); i != e;)
			{
				cur = i.m_iterator;
				++i;
				delete cur;
			}
			::operator delete(m_begin.m_iterator);
			::operator delete(m_begin.m_iterator);
		}
	private:
		iterator m_begin;
		iterator m_end;
		size_t m_size;

		void _initial()
		{
			m_begin.m_iterator->prev = nullptr;
			m_begin.m_iterator->next = m_end.m_iterator;
			m_end.m_iterator->next = nullptr;
			m_end.m_iterator->prev = m_begin.m_iterator;
		}

		iterator _insert_between(iterator prev_ite, iterator next_ite)
		{
			list_base* temp = new list_base(prev_ite, next_ite);
			prev_ite.m_iterator->next = temp;
			next_ite.m_iterator->prev = temp;
			return temp;
		}
		iterator _insert_between(iterator prev_ite, const T &val, iterator next_ite)
		{
			list_base* temp = new list_base(prev_ite, val, next_ite);
			prev_ite.m_iterator->next = temp;
			next_ite.m_iterator->prev = temp;
			return temp;
		}
		iterator _insert_between(iterator prev_ite, T &&val, iterator next_ite)
		{
			list_base* temp = new list_base(prev_ite, MySTL::forward<T>(val), next_ite);
			prev_ite.m_iterator->next = temp;
			next_ite.m_iterator->prev = temp;
			return temp;
		}
	};
}

