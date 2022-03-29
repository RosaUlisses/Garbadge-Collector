#pragma once
#ifndef HASH_SET_H
#define HASH_SET_H

#include <cmath>
#include <cstdlib>
#include <functional>

#define SIZE (100)


template<typename T>
class Hash_set
{

	protected:
			template<typename T>
			struct Node
			{
				T value;
				Node<T>* next;

				Node(T value, Node* next)
				{
					this->value = value;
					this->next = next;
				}
			};

	private:
			Node<T>* table[SIZE] = { 0 };
			unsigned int element_count;

			size_t hashFunction(T key)
			{
				return std::hash<T>()(key) % SIZE;
			}

	public:
			Hash_set()
			{
				element_count = 0;
			}
			bool contains(T key)
			{
				size_t hash_code = hashFunction(key);
				Node<T>* current = table[hash_code];

				while (current != nullptr)
				{
					if (current->value == key) return true;
					current = current->next;
				}

				return false;
			}

			void insert(T key)
			{
				element_count++;
				size_t hash_code = hashFunction(key);
				Node<T>* current = table[hash_code];

				if (current == nullptr)
				{
					table[hash_code] = (Node<T>*)std::malloc(sizeof(Node<T>));
					*table[hash_code] = Node<T>(key, nullptr);
					return;
				}

				Node<T>* node = (Node<T>*)std::malloc(sizeof(Node<T>));
				*node = Node<T>(key, table[hash_code]);
				table[hash_code] = node;
			}

			void erase(T key)
			{
				element_count--;
				size_t hash_code = hashFunction(key);
				Node<T>* current = table[hash_code];
				Node<T>* previous = nullptr;

				if (current->next == nullptr)
				{
					std::free(current);
					table[hash_code] = nullptr;
					return;
				}

				while (current != nullptr)
				{
					previous = current;
					if (current->value == key) {
						previous->next = current->next;
						std::free(current);
						break;
					}
					current = current->next;
				}
			}

			int get_Element_count()
			{
				return element_count;
			}

			Node<T>** get_table()
			{
				return &table[0];
			}

			template<typename T>
			class Set_iterator
			{
				private:
					Node<T>* ptr = nullptr;
					int index = 0;
					Hash_set<T>* hash;

				public:
					Set_iterator(Node<T>* ptr, int index, Hash_set<T>* hash)
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

					bool operator !=(Set_iterator& other) const
					{
						return other.getNode() != ptr;
					}

					bool operator ==(Set_iterator& other) const
					{
						return other.getNode() == ptr;
					}

					Node<T>* getNode()
					{
						return ptr;
					}

					T getValue()
					{
						return ptr->value;
					}
			};

			Hash_set<T>::Set_iterator<T> begin()
			{
				for (int i = 0; i < SIZE; i++)
				{
					Node<T>* node = table[i];
					if (node != nullptr) return Set_iterator<T>(node, i, this);
				}
			}

			Hash_set<T>::Set_iterator<T> end()
			{
				if (element_count == 1) return begin();

				return Set_iterator<T>(nullptr, SIZE - 1, this);
			}

			~Hash_set()
			{
				for(int i = 0; i < SIZE; i++)
				{
					Node<T>* node = table[i];

					while(node != nullptr)
					{
						Node<T>* next = node->next;
						std::free(node);
						node = next;
					}
				}
			}
};





#endif

