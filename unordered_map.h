#pragma once
#include"utility.h"
#include"hash.h"



namespace MySTL 
{
	template<typename Key, typename Value, typename Hash = MySTL::hash<Key>, typename KeyEqual = MySTL::equal_to<Key>>
	class unordered_map
	{
		static const int initial_capacity = 4;
		using Pair = MySTL::pair<Key, Value>;
		using slot_type = MySTL::list<Pair>*;
		slot_type* m_data;
		size_t m_size;
		size_t m_capacity;
		Hash hash_func;
		KeyEqual equal;
	public:
		using iterator = hash_set_iterator<Pair>;
		unordered_map() :m_size(0), hash_func(), equal(),
			m_capacity(initial_capacity), m_data(new slot_type[initial_capacity]()) {}


		explicit unordered_map(size_t bucket_count = initial_capacity, Hash hash = Hash(),
			KeyEqual equal = KeyEqual())
			:m_size(0), m_capacity(bucket_count>initial_capacity ? bucket_count : initial_capacity),
			m_data(new slot_type[initial_capacity]()), hash_func(hash), equal(equal)
		{}


		unordered_map(const unordered_map &rhs) :m_size(rhs.m_size), hash_func(rhs.hash_func), equal(rhs.equal),
			m_capacity(rhs.m_capacity)
		{
			m_data = new slot_type[m_capacity]();
			for (size_t i = 0; i < m_capacity; ++i)
			{
				if (rhs.m_data[i] != nullptr)
				{
					m_data[i] = new list<Pair>(*(m_data[i]));
				}
			}
		}
		unordered_map(unordered_map &&rhs) :m_size(rhs.m_size), hash_func(rhs.hash_func), equal(rhs.equal),
			m_capacity(rhs.m_capacity)
		{
			m_data = rhs.m_data;
			rhs.m_data = nullptr;
			rhs.m_size = 0;
		}

		void operator=(unordered_map rhs) &
		{
			swap(rhs);
		}

		void swap(unordered_map &rhs)
		{
			MySTL::swap(m_size, rhs.m_size);
			MySTL::swap(m_capacity, rhs.m_capacity);
			MySTL::swap(m_data, rhs.m_data);
		}
		iterator insert(const Pair &p)
		{
			Key key = p.first;
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
								temp[index] = new MySTL::list<Pair>();
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
				m_data[index] = new MySTL::list<Pair>();
			}
			m_data[index]->push_front(p);
			return{ this,index,m_data[index]->begin() };
		}

		iterator insert(Pair &&p)
		{
			Key key = p.first;
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
								temp[index] = new MySTL::list<Pair>();
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
				m_data[index] = new MySTL::list<Pair>();
			}
			m_data[index]->push_front(MySTL::forward<Pair>(p));
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
				if (equal(i->first, key))
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
				if (equal(i->first, key))
				{
					return{ this, index, i };
				}
			}
			return end();
		}
		Value &operator[](const Key &key)
		{
			size_t index = hash_func(key) % m_capacity;
			if (m_data[index] == nullptr)
			{
				m_data[index] = new MySTL::list<Pair>(1, Value());
				return m_data[index]->front().second;
			}
			for (auto i = m_data[index]->begin(), e = m_data[index]->end(); i != e; ++i)
			{
				if (equal(i->first, key))
				{
					return i->second;
				}
			}
			m_data[index]->push_back({ key,Value() });
			return m_data[index]->back().second;

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
			m_data = new slot_type[initial_capacity]();
			m_capacity = initial_capacity;
			m_size = 0;
		}

		~unordered_map()
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

	template<typename Key, typename Value>
	struct hash_map_iterator
	{
		unordered_map<Key, Value>* set;
		size_t index;
		MySTL::list<Pair>::iterator list_ite;
		using Pair = MySTL::pair<Key, Value>;
	public:
		hash_map_iterator(unordered_map<Key, Value>* s, size_t i, MySTL::list<Pair>::iterator ite) :set(s), index(i), list_ite(ite) {}
		hash_map_iterator(const hash_map_iterator &rhs) = default;
		void operator=(const hash_map_iterator &rhs)
		{
			set = rhs.set;
			index = rhs.index;
			list_ite = rhs.list_ite;
		}
		bool operator==(const hash_map_iterator &rhs)
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
		hash_map_iterator operator++()
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
		hash_map_iterator operator++(int)
		{
			hash_map_iterator res = *this;
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
		hash_map_iterator operator--()
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
		hash_map_iterator operator--(int)
		{
			hash_map_iterator res = *this;
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
