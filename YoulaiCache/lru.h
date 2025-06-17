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

	void remove(const Key& key) {
		std::lock_guard<std::mutex> lock(mtx);
		auto it = keyMap.find(key);
		if (it != keyMap.end())
		{
			removeNode(it->second);
			keyMap.erase(it);
			delete it->second;
		}
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

// LRU优化：Lru-k版本。 通过继承的方式进行再优化
template<typename Key, typename Value>
class LRUKCache : public LRUCache<Key, Value> {
private:
	int k;
	LRUCache<Key, size_t> slaveCache; // key-freq
	std::unordered_map<Key, Value> slaveKeyMap; // key-value
public:
	LRUKCache(int capacity, int slaveCapacity, int k)
		: LRUCache<Key, Value>(capacity) // 调用基类构造
		, slaveCache(slaveCapacity)
		, k(k)
	{}

	Value get(const Key& key) {
		Value value{};
		bool inMainCache = LRUCache<Key, Value>::get(key, value);

		size_t freq = slaveCache.get(key);
		slaveCache.put(key, ++freq);

		if (inMainCache) {
			return value;
		}

		if (freq >= k) {
			auto it = slaveKeyMap.find(key);
			if (it != slaveKeyMap.end()) {
				Value storedValue = it->second;
				slaveCache.remove(key);
				slaveKeyMap.erase(it);

				LRUCache<Key, Value>::put(key, storedValue);
				
				return storedValue;
			}
		}
		
		return value;
	}

	void put(const Key& key, const Value& value) {
		Value oldValue{};
		bool inMainCache = LRUCache<Key, Value>::get(key, oldValue);

		if (inMainCache) {
			LRUCache<Key, Value>::put(key, value);
			return;
		}

		size_t freq = slaveCache.get(key);
		slaveCache.put(key, ++freq);

		slaveKeyMap[key] = value;

		if (freq >= k) {
			slaveCache.remove(key);
			slaveKeyMap.erase(key);
			LRUCache<Key, Value>::put(key, value);
		}
	}

};

};

