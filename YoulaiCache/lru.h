#include <iostream>
#include <unordered_map>

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
	int size;
	Node* head;
	Node* tail;
	std::unordered_map<int, Node*> cache;

public:
	LRUCache(int capacity) : capacity(capacity), size(0) {
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

		if (size > capacity) {
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
		size--;
		cache.erase(node->key);
		delete node; 
	}

	void insertNode(Node* node) {
		node->next = head->next;
		node->prev = head;
		head->next->prev = node;
		head->next = node;
		size++;
		cache[node->key] = node;
	}
};

