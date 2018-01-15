#pragma once
#include<cstring>
#include"utility.h"

namespace inferiority
{
	class string
	{
		friend string operator+(const string &lhs, const string &rhs);
	private:
		char* m_data;
		size_t m_size;
		size_t m_capacity;
	public:
		static const size_t npos = -1;
		using iterator = char*;
		using reverse_iterator = reverse_iterator<iterator>;
		string() :m_data(new char[1]()), m_size(0),m_capacity(1) {}
		explicit string(size_t n) : m_size(n), m_capacity(n+1)
		{
			m_data = new char[m_capacity]();
		}
		string(size_t n, const char &val) : m_size(n), m_capacity(n + 1)
		{
			m_data = new char[m_capacity];
			if(val == 0)
			{
				m_size = 0;
			}
			else
			{
				for (size_t i = 0; i < n; ++i)
				{
					m_data[i] = val;
				}
				m_data[n] = '\0';
			}
		}
		string(const string &rhs) :m_size(rhs.m_size),m_capacity(m_size+1)
		{
			m_data = new char[m_capacity];
			strcpy(m_data, rhs.m_data);
		}
		string(string &&rhs)
		{
			m_size = rhs.m_size;
			m_data = rhs.m_data;
			m_capacity = rhs.m_capacity;
			rhs.m_data = nullptr;
		}
		string(const char* data)
		{
			m_size = strlen(data);
			m_capacity = m_size + 1;
			m_data = new char[m_capacity];
			strcpy(m_data, data);
		}
		void operator=(string rhs) &
		{
			swap(rhs);
		}

		void swap(string &rhs)
		{
			inferiority::swap(m_data,rhs.m_data);
			inferiority::swap(m_capacity, rhs.m_capacity);
			inferiority::swap(m_size, rhs.m_size);
		}
		iterator begin() 
		{
			return m_data;
		}

		reverse_iterator rbegin() 
		{
			return m_data + m_size - 1;
		}

		iterator end() 
		{
			return m_data + m_size;
		}

		reverse_iterator rend() 
		{
			return m_data - 1;
		}

		bool empty() const
		{
			return m_size == 0;
		}
		size_t size() const
		{
			return m_size;
		}
		size_t capacity() const
		{
			return m_capacity;
		}
		char &front() 
		{
			return *m_data;
		}

		char &back() 
		{
			return m_data[m_size - 1];
		}
		void clear()
		{
			m_capacity = 1;
			m_size = 0;
			*m_data = '\0';
		}
		void shrink_to_fit()
		{
			m_capacity = m_size + 1;
			char* temp = new char[m_capacity];
			strcpy(temp, m_data);
			delete[] m_data;
			m_data = temp;
		}
		const char *data() const
		{
			return m_data;
		}
		char *data()
		{
			return m_data;
		}
		const char *c_str() const
		{
			return m_data;
		}
		string substr(size_t pos=0, size_t len=npos) const
		{
			if (len >= m_size)
			{
				len = m_size;
			}
			string res(len);
			strncpy(res.m_data, m_data, len);
			(res.m_data)[len] = '\0';
			return res;
		}
		void push_back(char val)
		{
			if(m_size==m_capacity-1)
			{
				m_capacity *= 2;
				char *temp = new char[m_capacity];
				strcpy(temp, m_data);
				delete[] m_data;
				m_data = temp;
			}
			m_data[m_size] = val;
			m_size += 1;
		}
		void pop_back()
		{
			m_size -= 1;
			m_data[m_size] = '\0';
		}
		void operator+=(const string &rhs)
		{
			size_t n = m_size + rhs.size() + 1;
			if (m_capacity > n)
			{
				strcpy(m_data + m_size, rhs.m_data);
			}
			else
			{
				m_capacity = n;
				char* temp = new char[m_capacity];
				strcpy(temp, m_data);
				strcpy(temp + m_size, rhs.m_data);
				m_size = n - 1;
				delete[] m_data;
				m_data = temp;
			}
		}
		string &append(const string &rhs)
		{
			size_t n = m_size + rhs.size() + 1;
			if (m_capacity > n)
			{
				strcpy(m_data + m_size, rhs.m_data);
			}
			else
			{
				m_capacity = n;
				char* temp = new char[m_capacity];
				strcpy(temp, m_data);
				strcpy(temp + m_size, rhs.m_data);
				m_size = n - 1;
				delete[] m_data;
				m_data = temp;
			}
			return *this;
		}
		char &operator[](size_t n) 
		{
			return m_data[n];
		}

		size_t length()const
		{
			return m_size;
		}
		bool operator<(const string &rhs)
		{
			return strcmp(m_data, rhs.m_data) < 0;
		}
		bool operator<=(const string &rhs)
		{
			return strcmp(m_data, rhs.m_data) <= 0;
		}
		bool operator!=(const string &rhs)
		{
			return strcmp(m_data, rhs.m_data) != 0;
		}
		bool operator==(const string &rhs)
		{
			return strcmp(m_data, rhs.m_data) == 0;
		}
		bool operator>=(const string &rhs)
		{
			return strcmp(m_data, rhs.m_data) >= 0;
		}
		bool operator>(const string &rhs)
		{
			return strcmp(m_data, rhs.m_data) > 0;
		}


		size_t find(const string &str,size_t pos = 0)
		{
			if (str.empty()|| str.size() + pos > m_size)
			{
				return npos;
			}
			size_t n = str.size();
			size_t* next = new size_t[n];
			next[0] = 0;
			size_t k = 0;
			for (size_t i = 1; i < n; ++i)
			{
				while (str[k]!= str[i]  &&k != 0)
				{
					k = next[k];
				}
				if (str[k] == str[i])
				{
					k += 1;
				}
				next[i] = k;
			}
			bool is_matched = true;
			for (size_t e = m_size - pos; pos <= e;)
			{
				is_matched = true;
				for (size_t i = 0; i < n; ++i)
				{
					if(m_data[pos+i]!=str[i])
					{
						pos += i - next[i] + 1;
						is_matched = false;
						break;
					}
				}
				if (is_matched)
				{
					return pos;
				}
			}
			return npos;
		}
		const char &operator[](size_t n) const
		{
			return m_data[n];
		}
		~string()
		{
			delete[] m_data;
		}


	};

	string operator+(const string &lhs, const string &rhs)
	{
		size_t n = lhs.size();
		string res(n+rhs.size());
		strcpy(res.m_data, lhs.data());
		strcpy(res.m_data+n, rhs.data());
		return res;
	}
}