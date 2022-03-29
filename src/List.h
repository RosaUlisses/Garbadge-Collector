#pragma once
#ifndef LIST_H
#define LIST_H

#include <cstdlib>

template<typename T>
class List
{
		protected:
			template<typename T>
			struct Node
			{
				T value;
				Node<T>* next;

				Node(T value, Node<T>* next)
				{
					this->value = value;
					this->next = next;
				}
			};

	private:
		Node<T>* head;
		int size;

	public:

		List()
		{
			head = nullptr;
			size = 0;
		}

		void push_front(T value)
		{
			Node<T>* node = (Node<T>*)std::malloc(sizeof(Node<T>));
			*node = Node<T>(value, head);
			head = node;
			size++;
		}

		void pop_front()
		{
			if (size == 0) return;

			Node<T>* aux = head;
			head = head->next;
			std::free(head);
			size--;
		}

		template<typename T>
		class List_iterator
		{
			private:
				Node<T>* node;

			public:

				List_iterator(Node<T>* node)
				{
					this->node = node;
				}

				Node<T>* getNode()
				{
					return node;
				}

				T getValue()
				{
					return node->value;
				}

				void operator ++(int)
				{
					node = node->next;
				}

				bool operator ==(List_iterator& other) const
				{
					return node == other.getNode();
				}

				bool operator !=(List_iterator& other) const
				{
					return node != other.getNode();
				}
		};

		List_iterator<T> begin()
		{
			return List_iterator<T>(head);
		}

		List_iterator<T> end()
		{
			return List_iterator<T>(nullptr);
		}

		~List()
		{
			Node<T>* node = head;

			while(node != nullptr)
			{
				Node<T>* next = node->next;
				std::free(node);
				node = next;
			}
		}

};

#endif
