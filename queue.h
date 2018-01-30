#pragma once
#include"vector.h"
#include"algorithm.h"
namespace inferiority
{
	template<typename T>
	class deque
	{
	private:
		T* m_data;
		size_t m_capacity;
		size_t m_begin;
		size_t m_size;
		static const int initial_capacity = 4;
		static const int initial_begin = 1;
	public:
		deque():m_data(nullptr),m_begin(0),m_capacity(0),m_size(0){}
		deque(size_t n,const T &val):m_data(nullptr),m_capacity(n),m_begin(0),m_size(n)
		{
			if (n != 0)
			{
				m_data = (T*) ::operator new(sizeof(T)*n);
				construct_n(n, m_data, val);
			}
		}
		explicit deque(size_t n):m_data(nullptr),m_capacity(n), m_begin(0), m_size(n)
		{
			if (n != 0)
			{
				m_data = (T*) ::operator new(sizeof(T)*n);
				construct_n(n, m_data);
			}
		}

		deque(const deque &rhs):m_data(nullptr),m_size(rhs.size),m_begin(0),m_capacity(m_size)
		{
			if (m_size != 0)
			{
				m_data = (T*) ::operator new(sizeof(T)*rhs.m_size);
				_fill_in(m_size, m_data, rhs.m_data + rhs.m_begin);
			}
		}
		deque(deque &&rhs):m_data(rhs.m_data),m_size(rhs.m_size),
			m_begin(rhs.m_begin),m_capacity(rhs.m_capacity)
		{
			rhs.m_data = nullptr;
		}
		void operator=(deque rhs)
		{
			swap(rhs);
		}

		void push_back(const T &val)
		{
			if (m_data == nullptr)
			{
				m_size = initial_capacity;
				m_capacity = initial_capacity;
				m_begin = initial_begin;
				m_data = (T*) ::operator new(sizeof(T)*initial_capacity);
				construct(m_data + m_begin, val);
				return;
			}
			if (m_begin + m_size == m_capacity)
			{
				m_capacity += m_size;
				m_data = _move_to(m_data, m_begin, m_size,m_begin, m_capacity);
			}
			construct(m_data + m_begin + m_size, val);
			m_size += 1;
			
		}

		void push_back(T &&val)
		{
			if (m_data == nullptr)
			{
				m_size = initial_capacity;
				m_capacity = initial_capacity;
				m_begin = initial_begin;
				m_data = (T*) ::operator new(sizeof(T)*initial_capacity);
				construct(m_data + m_begin, inferiority::forward<T>(val));
				return;
			}
			if (m_begin + m_size == m_capacity)
			{
				m_capacity += m_size;
				m_data = _move_to(m_data, m_begin, m_size,m_begin, m_capacity);
			}
			construct(m_data + m_begin + m_size, inferiority::forward<T>(val));
			m_size += 1;
		}

		void push_front(const T &val)
		{
			if (m_data == nullptr)
			{
				m_size = initial_capacity;
				m_capacity = initial_capacity;
				m_begin = initial_begin;
				m_data = (T*) ::operator new(sizeof(T)*initial_capacity);
				construct(m_data + m_begin, val);
				return;
			}
			if (m_begin == 0)
			{
				m_capacity += m_size;
				m_data = _move_to(m_data, 0, m_size,m_size,m_capacity);
				m_begin = m_size - 1;
			}
			m_size += 1;
			construct(m_data + m_begin, val);

		}

		void push_front(T &&val)
		{
			if (m_data == nullptr)
			{
				m_size = initial_capacity;
				m_capacity = initial_capacity;
				m_begin = initial_begin;
				m_data = (T*) ::operator new(sizeof(T)*initial_capacity);
				construct(m_data + m_begin,inferiority::forward<T>(val));
				return;
			}
			if (m_begin == 0)
			{
				m_capacity += m_size;
				m_data = _move_to(m_data, 0, m_size, m_size, m_capacity);
				m_begin = m_size - 1;
			}
			m_size += 1;
			construct(m_data + m_begin,inferiority::forward<T>(val));
		}

		void pop_back()
		{
			destruct(m_data + m_begin + m_size);
			m_size -= 1;
		}

		void pop_front()
		{
			destruct(m_data + m_begin);
			m_size -= 1;
		}

		bool empty()
		{
			return m_size == 0;
		}

		T &front()
		{
			return *(m_data + m_begin);
		}

		T &back()
		{
			return *(m_data + m_begin + m_size - 1);
		}

		T &operator[](size_t n)
		{
			return *(m_data + m_begin + n);
		}

		void swap(const deque &rhs)
		{
			inferiority::swap(m_begin,rhs.m_begin);
			inferiority::swap(m_capacity, rhs.m_capacity);
			inferiority::swap(m_data, rhs.m_data);
		}

		size_t size()
		{
			return m_size;
		}
		~deque()
		{
			destruct_n(m_size, m_data + m_begin);
			::operator delete(m_data);
		}
	};

	template<typename T>
	class queue
	{
	private:
		T* m_data;
		size_t m_begin;
		size_t m_size;
		size_t m_capacity;
		static const int initial_capacity = 4;
	public:
		queue():m_data(nullptr),m_begin(0),m_size(0),m_capacity(0){}
		explicit queue(size_t n):m_data(nullptr),m_size(0),m_begin(0),m_capacity(n) 
		{
			if (n != 0)
			{
				m_data = (T*) ::operator new(sizeof(T)*n);
				construct_n(n, m_data);
			}
		}
		queue(size_t n, const T &val) :m_data(nullptr), m_capacity(n), m_begin(0), m_size(n)
		{
			if (n != 0)
			{
				m_data = (T*) ::operator new(sizeof(T)*n);
				construct_n(n, m_data, val);
			}
		}
		queue(const queue &rhs) :m_data(nullptr), m_size(rhs.size), m_begin(0), m_capacity(m_size)
		{
			if (m_size != 0)
			{
				m_data = (T*) ::operator new(sizeof(T)*rhs.m_size);
				_fill_in(m_size, m_data, rhs.m_data + rhs.m_begin);
			}
		}
		queue(queue &&rhs) :m_data(rhs.m_data), m_size(rhs.m_size),
			m_begin(rhs.m_begin), m_capacity(rhs.m_capacity)
		{
			rhs.m_data = nullptr;
		}
		void operator=(queue rhs)
		{
			swap(rhs);
		}
		size_t size()
		{
			return m_size;
		}
		void push(const T &val)
		{
			if (m_data == nullptr)
			{
				m_size = initial_capacity;
				m_capacity = initial_capacity;
				m_data = (T*) ::operator new(sizeof(T)*initial_capacity);
				construct(m_data, val);
				return;
			}
			if (isFull())
			{
				m_capacity == m_size * 2;
				m_data = _move_to(m_data, m_begin, m_size, 0, m_capacity);
				m_begin = 0;
				m_end = m_size + 1;
			}
			construct(m_data + m_begin + m_size, val);
			m_size += 1;
		}
		void push(T &&val)
		{
			if (m_data == nullptr)
			{
				m_size = initial_capacity;
				m_capacity = initial_capacity;
				m_data = (T*) ::operator new(sizeof(T)*initial_capacity);
				construct(m_data, inferiority::forward<T>(val));
				return;
			}
			if (isFull())
			{
				m_capacity == m_size*2;
				m_data = _move_to(m_data, m_begin, m_size, 0, m_capacity);
			}
			construct(m_data + m_begin + m_size,inferiority::forward<T>(val));
			m_size += 1;
		}
		void pop()
		{
			destruct(m_data + m_begin);
			m_begin += 1;
			m_size -= 1;
		}
		T &front()
		{
			return *(m_data + m_begin);
		}
		T &back()
		{
			return *(m_data + m_begin + m_size - 1);
		}
		bool empty()
		{
			return m_size == 0;
		}
	private:
		bool isFull()
		{
			return m_size == m_capacity;
		}
	};



	template <typename T, typename Comparer= inferiority::less<T>,typename Container = inferiority::vector<T>>
	class priority_queue
	{
	public:
		explicit priority_queue() :m_container(), m_comparer() {}
		priority_queue(const Comparer &comparer, const Container &cont):m_comparer(comparer),m_container(cont)
		{
			inferiority::make_heap(m_container.begin(), m_container.end(), m_comparer);
		}
		priority_queue(const Comparer &comparer, Container &&cont) :m_comparer(comparer),
			m_container(inferiority::forward<Container>(cont)) 
		{
			inferiority::make_heap(m_container.begin(), m_container.end(), m_comparer);
		}
		priority_queue(const Comparer &comparer):m_comparer(comparer),m_container(){}
		priority_queue(const priority_queue &rhs) = default;
		priority_queue(priority_queue &&rhs) = default;
		void operator=(priority_queue rhs)
		{
			swap(rhs);
		}
		void swap(priority_queue &rhs)
		{
			inferiority::swap(rhs.m_container);
			inferiority::swap(rhs.m_comparer);
		}
		const T &top() const
		{
			return m_container.front();
		}
		size_t size() const
		{
			return m_container.size();
		}
		bool empty()
		{
			return m_container.empty();
		}
		void push(const T &val)
		{
			m_container.push_back(val);
			inferiority::push_heap(m_container.begin(), m_container.end(),m_comparer);
		}
		void push(T &&val)
		{
			m_container.push_back(inferiority::forward<T>(val));
			inferiority::push_heap(m_container.begin(), m_container.end(),m_comparer);
		}
		void pop()
		{
			inferiority::pop_heap(m_container.begin(), m_container.end(), m_comparer);
			m_container.pop_back();
		}
	private:
		Container m_container;
		Comparer m_comparer;
	};
}