#include <iostream>
#include <unordered_map>
#include "lru.h"

/*
���ýڵ��࣬����key��value

����һ��˫������������̭

����һ����ϣ��<�����ڵ�ָ��>�������ж��Ƿ���ںͿ��ٶ�λ
*/
namespace lruk {

	class Node {
	public:
		int key;
		int value;
		int freq;
		Node* prev;
		Node* next;
		Node(int key, int value) : key(key), value(value), prev(nullptr), next(nullptr) {}
	};

	class FreqList {
	public:
		Node* head;
		Node* tail;

		FreqList() {
			head = new Node(0, 0); // Dummy head
			tail = new Node(0, 0); // Dummy tail
			head->next = tail;
			tail->prev = head;
		}

		~FreqList() {
			delete head;
			delete tail;
		}

		bool isEmpty() {
			return head->next == tail;
		}

		// ����ڵ㵽����ͷ��
		void insertNode(Node* node) {
			node->prev = head;
			node->next = head->next;
			head->next->prev = node;
			head->next = node;
		}

		// ���������Ƴ��ڵ�
		void removeNode(Node* node) {
			node->next->prev = node->prev;
			node->prev->next = node->next;
			node->prev = nullptr;
			node->next = nullptr;
		}

	};
	// ��̭���ԣ���̭��Ծ�б�β�ڵ㣬ֱ�Ӷ���
	class LRUKCache : public lru::LRUCache {
	private:
		int capacity;
		int wait_capacity; // �ȴ�����
		int k;
		FreqList* active_list; // ��Ծ�б� freq >= k
		//FreqList* inactive_list; // �ǻ�Ծ�б� freq < k
		std::unordered_map<int, Node*> key_map; 

	public:
		LRUKCache(int capacity, int wait_capacity, int k) {
			lru::LRUCache()
		}

		~LRUKCache() {
			delete active_list;
			//delete inactive_list;
			for (auto& pair : key_map) {
				delete pair.second;
			}
		}

		int get(int key) {
			if (key_map.find(key) == key_map.end()) {
				return -1; 
			}
			if (wait_capacity )
			Node* node = key_map[key];
			size++;
			node->freq++;

			if (node->freq < k) {
				return -1;
			}
			moveToHead(node);
			return node->value;
		}

		void put(int key, int value) {
			if (key_map.find(key) != key_map.end()) {
				key_map[key]->value = value; // Update the value
				moveToHead(key_map[key]); // Move to head
				return;
			}

			Node* newNode = new Node(key, value); // ����Ϊʲô���Ȳ�����ɾ������Ϊ������Ҫ�������жϣ�capacityΪ0�����
			insertNode(newNode);

			if (key_map.size() > capacity) {
				removeNode(tail->prev); // Remove the least recently used node (tail's previous node)
			}
		}

	private:

		void moveToHead(Node* node) {
			if (node == head->next) {
				return;
			}
			// rmeove from linked list
			node->prev->next = node->next;
			node->next->prev = node->prev;

			// insert to head
			node->next = head->next;
			node->prev = head;
			head->next->prev = node;
			head->next = node;
		}

		void removeNode(Node* node) {
			node->prev->next = node->next;
			node->next->prev = node->prev;
			key_map.erase(node->key);
			delete node;
		}

		void insertNode(Node* node) {
			node->next = head->next;
			node->prev = head;
			head->next->prev = node;
			head->next = node;
			key_map[node->key] = node;
		}
	};

}
