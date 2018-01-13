#pragma once
#include"utility.h"
#include"hash.h"



namespace MySTL 
{
	template<typename Key, typename Hash = MySTL::hash<Key>, typename KeyEqual = MySTL::equal_to<Key>>
	class unordered_set
	{
		static const int initial_capacity = 4;
		using slot_type = MySTL::list<Key>*;
		slot_type* m_data;
		size_t m_size;
		size_t m_capacity;
		Hash hash_func;
		KeyEqual equal;
	public:
		using iterator = hash_set_iterator<Key>;
		unordered_set() :m_size(0), hash_func(), equal(),
			m_capacity(initial_capacity), m_data(new slot_type[initial_capacity]()) {}


		explicit unordered_set(size_t bucket_count = initial_capacity, const Hash &hash = Hash(),
			const KeyEqual &equal = KeyEqual())
			:m_size(0), m_capacity(bucket_count>initial_capacity ? bucket_count : initial_capacity),
			m_data(new slot_type[initial_capacity]()), hash_func(hash), equal(equal)
		{}


		unordered_set(const unordered_set &rhs) :m_size(rhs.m_size), hash_func(rhs.hash_func), equal(rhs.equal),
			m_capacity(rhs.m_capacity)
		{
			m_data = new slot_type[m_capacity]();
			for (size_t i = 0; i < m_capacity; ++i)
			{
				if (rhs.m_data[i] != nullptr)
				{
					m_data[i] = new list<Key>(*(m_data[i]));
				}
			}
		}
		unordered_set(unordered_set &&rhs) :m_size(rhs.m_size), hash_func(rhs.hash_func), equal(rhs.equal),
			m_capacity(rhs.m_capacity)
		{
			m_data = rhs.m_data;
			rhs.m_data = nullptr;
			rhs.m_size = 0;
		}

		void operator=(unordered_set rhs) &
		{
			swap(rhs);
		}

		void swap(unordered_set &rhs)
		{
			MySTL::swap(m_size, rhs.m_size);
			MySTL::swap(m_capacity, rhs.m_capacity);
			MySTL::swap(m_data, rhs.m_data);
		}
		iterator insert(const Key &key)
		{
			if (m_size >= m_capacity)
			{
				m_capacity = m_capacity * 2;
				slot_type* temp = new slot_type[m_capacity]();
				for (size_t i = 0; i < m_capacity; ++i)
				{
					if (m_data[i] != nullptr)
					{
						for (auto j = m_data[i]->begin(); !m_data[i]->empty(); )
						{
							size_t index = hash_func(*j) % m_capacity;
							if (temp[index] == nullptr)
							{
								temp[index] = new MySTL::list<Key>();
							}
							temp[index]->push_back(MySTL::move(*j));
							m_data[i]->erase(j)
								j = m_data[i]->begin();
						}
						delete m_data[i];
					}
				}
				delete m_data;
				m_data = temp;
			}
			size_t index = hash_func(key) % m_capacity;
			if (m_data[index] == nullptr)
			{
				m_data[index] = new MySTL::list<Key>();
			}
			m_data[index]->push_front(key);
			return{ this,index,m_data[index]->begin() };
		}

		iterator insert(Key &&key)
		{
			if (m_size >= m_capacity)
			{
				m_capacity = m_capacity * 2;
				slot_type* temp = new slot_type[m_capacity]();
				for (size_t i = 0; i < m_capacity; ++i)
				{
					if (m_data[i] != nullptr)
					{
						for (auto j = m_data[i]->begin(); !m_data[i]->empty(); )
						{
							size_t index = hash_func(*j) % m_capacity;
							if (temp[index] == nullptr)
							{
								temp[index] = new MySTL::list<Key>();
							}
							temp[index]->push_back(MySTL::move(*j));
							m_data[i]->erase(j)
								j = m_data[i]->begin();
						}
						delete m_data[i];
					}
				}
				delete m_data;
				m_data = temp;

			}
			size_t index = hash_func(key) % m_capacity;
			if (m_data[index] == nullptr)
			{
				m_data[index] = new MySTL::list<Key>();
			}
			m_data[index]->push_front(MySTL::forward<Key>(key));
			return{ this, index, m_data[index]->begin() };
		}

		iterator erase(const Key &key)
		{
			size_t index = hash_func(key) % m_capacity;
			if (m_data[index] == nullptr)
			{
				++index;
				while (index<m_capacity)
				{
					if (m_data[index] != nullptr)
					{
						return{ this,index,m_data[index]->begin() };
					}
					++index;
				}
			}
			for (auto i = m_data[index]->begin(), e = m_data[index]->end(); i != e; ++i)
			{
				if (equal(*i, key))
				{
					iterator res(this, index, i);
					++res;
					m_data[index]->erase(i);
					return res;
				}
			}
			return end();
		}
		bool empty()
		{
			return m_size == 0;
		}

		size_t size()
		{
			return m_size;
		}
		iterator begin()
		{
			for (size_t index = 0; index < m_capacity; ++index)
			{
				if (m_data[index] != nullptr)
				{
					return{ this,index,m_data[index]->begin() };
				}
			}
			return end();
		}
		iterator end()
		{
			return{ this,m_capacity,nullptr };
		}
		iterator erase(iterator ite)
		{
			iterator res = ite;
			++res;
			m_data[ite.index]->erase(ite.list_ite);
			return res;
		}
		iterator find(const Key &key)
		{
			size_t index = hash_func(key) % m_capacity;
			if (m_data[index] == nullptr)
			{
				return end();
			}
			for (auto i = m_data[index]->begin(), e = m_data[index]->end(); i != e; ++i)
			{
				if (equal(*i, key))
				{
					return{ this, index, i };
				}
			}
			return end();
		}

		void clear()
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				if (m_data[i] != nullptr)
				{
					delete m_data[i];
				}
			}
			delete[] m_data;
			m_capacity = initial_capacity;
			m_data = new slot_type[initial_capacity]();
			m_size = 0;
		}

		~unordered_set()
		{
			for (size_t i = 0; i < m_capacity; ++i)
			{
				if (m_data[i] != nullptr)
				{
					delete m_data[i];
				}
			}
			delete[] m_data;
		}
	};





	template<typename Key>
	struct hash_set_iterator
	{
		unordered_set<Key>* set;
		size_t index;
		MySTL::list<Key>::iterator list_ite;
	public:
		hash_set_iterator(unordered_set<Key>* s, size_t i, MySTL::list<Key>::iterator ite) :set(s), index(i), list_ite(ite) {}
		hash_set_iterator(const hash_set_iterator &rhs) = default;
		void operator=(const hash_set_iterator &rhs)
		{
			set = rhs.set;
			index = rhs.index;
			list_ite = rhs.list_ite;
		}
		bool operator==(const hash_set_iterator &rhs)
		{
			if (set == rhs.set)
			{
				if (index == set->m_capacity)
				{
					return  rhs.index == index;
				}
				else if (index == rhs.index)
				{
					return list_ite == rhs.list_ite;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		hash_set_iterator operator++()
		{
			++list_ite;
			if (list_ite == (set->m_data)[index]->end())
			{
				++index;
				while (index<set->m_capacity)
				{
					if ((set->m_data)[index] != nullptr)
					{
						list_ite = (set->m_data)[index]->begin();
						return *this;
					}
					++index;
				}
			}
			return *this;
		}
		hash_set_iterator operator++(int)
		{
			hash_set_iterator res = *this;
			++list_ite;
			if (list_ite == (set->m_data)[index]->end())
			{
				++index;
				while (index<set->m_capacity)
				{
					if ((set->m_data)[index] != nullptr)
					{
						list_ite = (set->m_data)[index]->begin();
						return res;
					}
					++index;
				}
			}
			return res;
		}
		hash_set_iterator operator--()
		{
			if (index == set->m_capacity || list_ite == (set->m_data)[index]->begin())
			{
				--index;
				while (index >= 0)
				{
					if ((set->m_data)[index] != nullptr)
					{
						list_ite = (set->m_data)[index]->end();
						--list_ite;
						return *this;
					}
					--index;
				}
			}
			return *this;
		}
		hash_set_iterator operator--(int)
		{
			hash_set_iterator res = *this;
			if (index == set->m_capacity || list_ite == (set->m_data)[index]->begin())
			{
				--index;
				while (index >= 0)
				{
					if ((set->m_data)[index] != nullptr)
					{
						list_ite = (set->m_data)[index]->end();
						--list_ite;
						return res;
					}
					--index;
				}
			}
			return res;
		}
	};
}