#pragma once
#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <cstdlib>
#include <functional>


#define SIZE (100)

template<typename T1, typename T2>
class Hash_map
{

	protected:
		template<typename T1, typename T2>
		struct Node
		{
			T1 key;
			T2 value;
			Node<T1, T2>* next;

			Node(T1 key, T2 value, Node* next)
			{
				this->key = key;
				this->value = value;
				this->next = next;
			}
		};

	private:
		Node<T1, T2>* table[SIZE] = { 0 };
		unsigned int element_count;

		size_t hashFunction(T1 key)
		{
			return std::hash<T1>()(key) % SIZE;
		}

	public:

		Hash_map()
		{
			element_count = 0;
		}

		bool contains(T1 key)
		{
			size_t hash_code = hashFunction(key);
			Node<T1, T2>* current = table[hash_code];

			while (current != nullptr)
			{
				if (current->key == key) return true;
				current = current->next;
			}

			return false;
		}

		T2 get(T1 key)
		{
			size_t hash_code = hashFunction(key);
			Node<T1, T2>* current = table[hash_code];

			while (current != nullptr)
			{
				if (current->key == key) return current->value;
				current = current->next;
			}
		}

		void set(T1 key, T2 value)
		{
			size_t hash_code = hashFunction(key);
			Node<T1, T2>* current = table[hash_code];

			while (current != nullptr)
			{
				if (current->key == key) current->value = value;
				current = current->next;
			}
		}

		void insert(T1 key, T2 value)
		{
			element_count++;
			size_t hash_code = hashFunction(key);
			Node<T1, T2>* current = table[hash_code];

			if (current == nullptr)
			{
				table[hash_code] = (Node<T1, T2>*)std::malloc(sizeof(Node<T1, T2>));
				*table[hash_code] = Node<T1, T2>(key, value, nullptr);
				return;
			}

			Node<T1, T2>* node = (Node<T1, T2>*)std::malloc(sizeof(Node<T1, T2>));
			*node = Node<T1, T2>(key, value, table[hash_code]);
			table[hash_code] = node;
		}

		void erase(T1 key)
		{
			element_count--;
			size_t hash_code = hashFunction(key);
			Node<T1, T2>* current = table[hash_code];
			Node<T1, T2>* previous = nullptr;

			if(current->next == nullptr)
			{
				std::free(current);
				table[hash_code] = nullptr;
				return;
			}

			while (current != nullptr)
			{
				previous = current;
				if (current->key == key) {
					previous->next = current->next;
					std::free(current);
					break;
				}
				current = current->next;
			}
		}

		Node<T1, T2>** get_table()
		{
			return &table[0];
		}

		template<typename T1, typename T2>
		class Map_iterator
		{
		private:
			Node<T1, T2>* ptr = nullptr;
			int index = 0;
			Hash_map<T1, T2>* hash;

		public:
			Map_iterator(Node<T1, T2>* ptr, int index, Hash_map<T1, T2>* hash)
			{
				this->ptr = ptr;
				this->index = index;
				this->hash = hash;
			}

			void operator ++(int)
			{
				if (ptr->next != nullptr)
				{
					ptr = ptr->next;
					return;
				}

				index++;
				ptr = hash->get_table()[index];

				while (index < SIZE && ptr == nullptr)
				{
					index++;
					ptr = hash->get_table()[index];
				}

				if (index == SIZE) ptr = nullptr;
			}

			bool operator !=(Map_iterator& other) const
			{
				return other.getNode() != ptr;
			}

			bool operator ==(Map_iterator& other) const
			{
				return other.getNode() == ptr;
			}

			Node<T1, T2>* getNode()
			{
				return ptr;
			}

			T1 getKey()
			{
				return ptr->key;
			}

			T2 getValue()
			{
				return ptr->value;
			}
		};

		Hash_map<T1, T2>::Map_iterator<T1, T2> begin()
		{
			for (int i = 0; i < SIZE; i++)
			{
				Node<T1, T2>* node = table[i];
				if (node != nullptr) return Map_iterator<T1, T2>(node, i, this);
			}
		}

		Hash_map<T1, T2>::Map_iterator<T1, T2> end()
		{
			if (element_count == 1) return begin();
			return Map_iterator<T1, T2>(nullptr, SIZE - 1, this);
		}

		~Hash_map()
		{
			for (int i = 0; i < SIZE; i++)
			{
				Node<T1,T2>* node = table[i];

				while (node != nullptr)
				{
					Node<T1,T2>* next = node->next;
					std::free(node);
					node = next;
				}
			}
		}
};

#endif