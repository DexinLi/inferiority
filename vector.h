#pragma once
#include"utility.h"
//#include<initializer_list>

namespace inferiority {
	template<typename T>
	class vector
	{
	private:
		static const int initial_capacity = 4;
		
	public:
		using iterator = T*;
		using reverse_iterator = typename reverse_iterator<iterator>;
		vector() : m_data(nullptr), m_capacity(0), m_length(0) {}
		explicit vector(size_t n) :m_capacity(n), m_data(nullptr), m_length(n)
		{
			if (n != 0)
			{
				m_data =(T*) ::operator new (n * sizeof(T));
				construct_n(n, m_data);
			}
		}
		
		vector(size_t n, const T &val) :m_capacity(n), m_data(nullptr), m_length(n)
		{
			if (n != 0)
			{
				m_data = (T*) ::operator new (n * sizeof(T));
				construct_n(n, m_data, val);
			}
		}


		vector(const vector &rhs) :m_capacity(rhs.m_capacity), m_length(rhs.m_length)
		{
			m_data = (T*) ::operator new (m_length * sizeof(T));
			_fill_in(m_length, m_data, rhs.m_data);
		}
		vector(vector &&rhs) :m_data(rhs.m_data), m_capacity(rhs.m_capacity), m_length(rhs.m_length)
		{
			rhs.m_data = nullptr;
		}
		void operator=(vector rhs) &
		{
			swap(rhs);
		}
		T &operator[](size_t n)
		{
			return m_data[n];
		}

		bool empty()
		{
			return m_length == 0;
		}

		void push_back(const T &element) &
		{
			if (m_data == nullptr)
			{
				m_data = (T*) ::operator new (initial_capacity * sizeof(T));
				construct(m_data, element);
				m_capacity = initial_capacity;
				m_length = 1;
				return;
			}
			if (m_length == m_capacity)
			{
				m_capacity += m_capacity / 2;
				m_data = _move_to(m_data, 0, m_length,0, m_capacity);
			}
			construct(m_data + m_length, element);
			m_length += 1;
		}

		void push_back(T &&element) &
		{
			if (m_data == nullptr)
			{
				m_data = (T*) ::operator new (initial_capacity * sizeof(T));
				construct(m_data, inferiority::forward<T>(element));
				m_capacity = initial_capacity;
				m_length = 1;
				return;
			}
			if (m_length == m_capacity)
			{
				m_capacity += m_capacity / 2;
				m_data = _move_to(m_data, 0, m_length,0, m_capacity);
			}
			construct(m_data + m_length, inferiority::forward<T>(element));
			m_length += 1;
		}

		void pop_back() &
		{
			destruct(m_data + m_length - 1);
			m_length -= 1;
		}
		void swap(vector &rhs)
		{
			inferiority::swap(m_data, rhs.m_data);
			inferiority::swap(m_capacity, rhs.m_capacity);
			inferiority::swap(m_length, rhs.m_length);
		}

		void clear() &
		{
			m_length = 0;
			m_capacity = 0;
		}

		void shrink_to_fit()
		{
			if (m_length == 0)
			{
				if (m_data != nullptr)
				{
					::operator delete(m_data);
				}
			}
			else if(m_length!=m_capacity)
			{
				m_data = _move_to(m_data, 0, m_length,0, m_length);
			}
			m_capacity = m_length;
		}

		iterator begin() &
		{
			return m_data;
		}


		iterator end() &
		{
			return m_data + m_length;
		}
		T &front()
		{
			return *m_data;
		}
		T &back()
		{
			return *(m_data + length - 1);
		}
		reverse_iterator rbegin() &
		{
			return m_data + m_length - 1;
		}
		reverse_iterator rend() &
		{
			return m_data - 1;
		}
		void resize(size_t n) &
		{
			m_capacity = n;
			if (n > m_capacity)
			{
				m_data = _move_to(m_data, 0, m_length,0, m_capacity);
			}
			if (n < m_length)
			{
				destruct_n(m_length - n, m_data + n);
				m_length = n;
			}
		}
		size_t size() const
		{
			return m_length;
		}
		size_t capacity() const
		{
			return m_capacity;
		}
		const T* data() const
		{
			return m_data;
		}
		T *data()
		{
			return m_data;
		}
		~vector()
		{
			destruct_n(m_length, m_data);
			::operator delete(m_data);
		}

	private:
		T* m_data;
		size_t m_capacity;
		size_t m_length;
	};

}
