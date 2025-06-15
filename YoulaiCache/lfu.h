/*
LFU

��̭���ԣ���̭Ƶ�����������û�����ʵĽڵ�

���ýڵ��࣬����key��value��freq

���õ�һ����ϣ��<�����ڵ�ָ��>�������ж��Ƿ���ڣ����get��ֱ�ӷ���value���ͻ��Ƶ�Ρ�

���õڶ�����ϣ��<Ƶ�Σ�˫������ָ��>������ά��ͬƵ���ڵ�LRU����tailɾ�����һ���ڵ㡣

����һ��min_freq���ڿ�����̭���һ���ڵ㣬�����һ���ڵ㱻���ʣ�Ƶ�����ӣ��ƶ�����������Ϊ��ʱ��min_freq++�������һ���ڵ���Ϊcache������̭ʱ��min_freq=1��

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
