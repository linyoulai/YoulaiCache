#include <iostream>
#include <unordered_map>
#include <mutex>
#include "CachePolicy.h"

namespace YoulaiCache {

template<typename Key, typename Value>
class LRUCache;

template<typename Key, typename Value>
class LRUNode {
private:
	Key key;
	Value value;
	size_t freq;
	LRUNode* prev;
	LRUNode* next;
public:
	LRUNode(const Key& key, const Value& value) {
		this->key = key;
		this->value = value;
		this->freq = 1; // 初始频次为1
	}

	Key getKey() const {
		return key;
	}

	Value getValue() const {
		return value;
	}

	void setValue(const Value& value) {
		this->value = value;
	}

	size_t getFreq() const {
		return freq;
	}

	void incrementFreq() {
		++freq;
	}

	friend class LRUCache<Key, Value>;
};
template<typename Key, typename Value>
class LRUCache : public CachePolicy<Key, Value> {
private:
	int capacity;
	LRUNode<Key, Value>* head;
	LRUNode<Key, Value>* tail;
	std::unordered_map<Key, LRUNode<Key, Value>*> keyMap;

	mutable std::mutex mtx;
public:
	LRUCache(int capacity) : capacity(capacity) {
		head = new LRUNode<Key, Value>(Key(), Value());
		tail = new LRUNode<Key, Value>(Key(), Value());
		head->next = tail;
		tail->prev = head;
	}

	~LRUCache() override {
		std::lock_guard<std::mutex> lock(mtx);
		LRUNode<Key, Value>* cur = head;
		while (cur) {
			LRUNode<Key, Value>* nxt = cur->next;
			delete cur;
			cur = nxt;
		}
	}

	void put(const Key& key, const Value& value) override {
		std::lock_guard<std::mutex> lock(mtx);
		put_nolock(key, value);
	}

	bool get(const Key& key, Value& value) override {
		std::lock_guard<std::mutex> lock(mtx);
		return get_nolock(key, value);
	}

	Value get(const Key& key) override {
		std::lock_guard<std::mutex> lock(mtx);
		Value value{};
		get_nolock(key, value);
		return value;
	}
private:

	void put_nolock(const Key& key, const Value& value) {
		if (capacity <= 0) {
			return;
		}

		auto it = keyMap.find(key);
		if (it != keyMap.end()) { // it 指向entry <key, node>
			updateNode(it->second, value);
			return;
		}

		// node不存在
		addNode(key, value);
	}

	bool get_nolock(const Key& key, Value& value) {
		auto it = keyMap.find(key);
		if (it != keyMap.end()) {
			moveToHead(it->second);
			value = it->second->getValue();
			return true;
		}
		return false;
	}

	void updateNode(LRUNode<Key, Value>* node, const Value& value) {
		node->setValue(value);
		moveToHead(node);
	}

	void addNode(const Key& key, const Value& value) {
		// assert capcity > 0
		if (keyMap.size() >= capacity) {
			evictNode();
		}

		LRUNode<Key, Value>* node = new LRUNode<Key, Value>(key, value);
		insertNode(node);
		keyMap[key] = node;
	}
	
	void evictNode() {
		LRUNode<Key, Value>* leastRecentNode = tail->prev;
		removeNode(leastRecentNode);
		keyMap.erase(leastRecentNode->getKey());
		delete leastRecentNode;
	}

	void moveToHead(LRUNode<Key, Value>* node) {
		if (node->prev == head) {
			return;
		}

		node->next->prev = node->prev;
		node->prev->next = node->next;

		node->next = head->next;
		node->prev = head;
		head->next->prev = node;
		head->next = node;
	}
	
	void removeNode(LRUNode<Key, Value>* node) {
		node->next->prev = node->prev;
		node->prev->next = node->next;
		node->next = nullptr;
		node->prev = nullptr;
	}

	void insertNode(LRUNode<Key, Value>* node) {
		node->next = head->next;
		node->prev = head;
		head->next->prev = node;
		head->next = node;
	}
};

};

