#include <iostream>
#include <unordered_map>

/*
设置节点类，包含key、value

设置一个双向链表，用于淘汰

设置一个哈希表<键，节点指针>，用于判断是否存在和快速定位
*/

class Node {
public:
	int key;
	int value;
	Node* prev;
	Node* next;
	Node(int key, int value) : key(key), value(value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
	int capacity;
	Node* head;
	Node* tail;
	std::unordered_map<int, Node*> cache;

public:
	LRUCache(int capacity) : capacity(capacity) {
		head = new Node(0, 0); // Dummy head
		tail = new Node(0, 0); // Dummy tail
		head->next = tail;
		tail->prev = head;
	}

	~LRUCache() {
		Node* cur = head;
		while (cur) {
			Node* nxt = cur->next;
			delete cur;
			cur = nxt;
		}
	}

	int get(int key) {
		if (cache.find(key) == cache.end()) {
			return -1; // Key not found
		}
		Node* node = cache[key];
		// Move the accessed node to the front (most recently used)
		moveToHead(node);
		return node->value;
	}

	void put(int key, int value) {
		if (cache.find(key) != cache.end()) {
			cache[key]->value = value; // Update the value
			moveToHead(cache[key]); // Move to head
			return;
		}

		Node* newNode = new Node(key, value); // 想想为什么是先插入再删除？因为反过来要加特殊判断，capacity为0的情况
		insertNode(newNode);

		if (cache.size() > capacity) {
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
		cache.erase(node->key);
		delete node; 
	}

	void insertNode(Node* node) {
		node->next = head->next;
		node->prev = head;
		head->next->prev = node;
		head->next = node;
		cache[node->key] = node;
	}
};

