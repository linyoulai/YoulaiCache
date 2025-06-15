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

	bool isEmpty() {
		return head->next == tail;
	}

	void insertNode(Node* node) {

	}

	void removeNode(Node* node) {

	}
};

class LFUCache {
public:
	int capacity
};
