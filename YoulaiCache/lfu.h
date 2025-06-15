/*
LFU

淘汰策略：淘汰频次最少且最久没被访问的节点

设置节点类，包含key、value、freq

设置第一个哈希表<键，节点指针>，用于判断是否存在（如果get就直接返回value）和获得频次。

设置第二个哈希表<频次，双向链表指针>，用于维护同频次内的LRU，用tail删除最后一个节点。

设置一个min_freq用于快速淘汰最后一个节点，当最后一个节点被访问，频次增加，移动后，所在链表为空时，min_freq++，当最后一个节点因为cache满被淘汰时，min_freq=1。

*/
#include <unordered_map>


class Node {
public:
	int key;
	int value;
	int freq;
	Node* prev;
	Node* next;
	Node(int key, int value) : key(key), value(value), freq(1), prev(nullptr), next(nullptr) {}
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
		Node* cur = head;
		while (cur) {
			Node* nxt = cur->next;
			delete cur;
			cur = nxt;
		}
	}

	bool isEmpty() {
		return head->next == tail;
	}

	void insertNode(Node* node) {
		node->prev = head;
		node->next = head->next;
		head->next->prev = node;
		head->next = node;
	}

	void removeNode(Node* node) {
		node->next->prev = node->prev;
		node->prev->next = node->next;
		node->prev = nullptr;
		node->next = nullptr;
	}
};

class LFUCache {
private:
	int capacity;
	int min_freq;
	std::unordered_map<int, Node*> key_map; // key to node
	std::unordered_map<int, FreqList*> freq_map; // freq to list
public:
	LFUCache(int capacity) : capacity(capacity), min_freq(1) {}
	~LFUCache() {
		for (auto& pair : key_map) {
			delete pair.second; // Delete all nodes
		}
		for (auto& pair : freq_map) {
			delete pair.second; // Delete all frequency lists
		}
		key_map.clear();
		freq_map.clear();
		min_freq = 1;

	}

	int get(int key) {
		if (key_map.find(key) == key_map.end()) {
			return -1;
		}
		Node* node = key_map[key];
		int old_freq = node->freq;
		FreqList* freq_list = freq_map[node->freq];
		node->freq++;
		moveNode(node);
		if (freq_list->isEmpty()) {
			delete freq_list; // Remove the old frequency list if it's empty
			freq_map.erase(old_freq);
			if (min_freq == old_freq) {
				min_freq++;
			}
		}
		return node->value;
	}

	void put(int key, int value) {
		if (key_map.find(key) != key_map.end()) {
			Node* node = key_map[key];
			int old_freq = node->freq;
			node->value = value;
			node->freq++;
			moveNode(node);
			if (freq_map[old_freq]->isEmpty()) {
				delete freq_map[old_freq]; // Remove the old frequency list if it's empty
				freq_map.erase(old_freq);
				if (min_freq == old_freq) {
					min_freq++;
				}
			}
			return;
		}
		Node* new_node = new Node(key, value);
		key_map[key] = new_node;
		if (freq_map.find(1) == freq_map.end()) {
			freq_map[1] = new FreqList();
		}
		freq_map[1]->insertNode(new_node);
		if (key_map.size() > capacity) {
			Node* old_node = freq_map[min_freq]->tail->prev; // Get the last node in the min_freq list
			freq_map[min_freq]->removeNode(old_node);
			key_map.erase(old_node->key); // Remove from key_map
			delete old_node; // Free memory
			if (freq_map[min_freq]->isEmpty()) {
				freq_map.erase(min_freq);
			}
		}
		min_freq = 1;
	}
private:
	void moveNode(Node* node) {
		int old_freq = node->freq - 1;
		freq_map[old_freq]->removeNode(node);
		if (freq_map.find(node->freq) == freq_map.end()) {
			freq_map[node->freq] = new FreqList();
		}
		freq_map[node->freq]->insertNode(node);
	}
};
