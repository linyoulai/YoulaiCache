#include <iostream>
#include <cassert>
#include "lfu.h"
#include "lru.h"

void testLRUCache() {
    // 1. 测试基本功能和更新
    lru::LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    assert(cache.get(1) == 1);
    cache.put(2, 22); // 更新
    assert(cache.get(2) == 22);

    // 2. 测试淘汰 (key=1 应该是最久未使用的)
    cache.put(3, 3); // 此时 key=1 应该被淘汰
    assert(cache.get(1) == -1);
    assert(cache.get(2) == 22);
    assert(cache.get(3) == 3);

    // 3. 测试访问后更新LRU顺序
    // 当前顺序 (新->旧): 3, 2
    cache.get(2); // 访问 key=2，使其变为最新
    // 当前顺序 (新->旧): 2, 3
    cache.put(4, 4); // 此时应该淘汰 key=3
    assert(cache.get(3) == -1);
    assert(cache.get(2) == 22);
    assert(cache.get(4) == 4);

    std::cout << "All LRUCache tests passed!" << std::endl;
}

void testLFUCache() {
	// 1. 测试基本功能和更新
	lfu::LFUCache cache(2);
	cache.put(1, 1); // (key, value, freq) = (1, 1, 1)
	cache.put(2, 2); // (key, value, freq) = (2, 2, 1)
	assert(cache.get(1) == 1); // (1, 1, 2), (2, 2, 1)
	cache.put(2, 22); // (1, 1, 2), (2, 22, 2)
	assert(cache.get(2) == 22); // (1, 1, 2), (2, 22, 3)

	// 2. 测试淘汰 (key=1 应该是最久未使用的)
	cache.put(3, 3); // (2, 22, 3) (3, 3, 1)
	assert(cache.get(1) == -1); // 
	assert(cache.get(2) == 22); // (2, 22, 4) (3, 3, 1)
	assert(cache.get(3) == 3); // (2, 22, 4) (3, 3, 2)

	// 3. 测试访问后更新LFU顺序
	// 当前顺序 (新->旧): 3, 2
	cache.get(2); // (2, 22, 5) (3, 3, 2)
	// 当前顺序 (新->旧): 2, 3
	cache.put(4, 4); // (2, 22, 5) (4, 4, 1)
	assert(cache.get(3) == -1); 
	assert(cache.get(2) == 22); // (2, 22, 6) (4, 4, 1)
    assert(cache.get(4) == 4); // (2, 22, 6) (4, 4, 2)
    assert(cache.get(4) == 4); // (2, 22, 6) (4, 4, 3)
    assert(cache.get(4) == 4); // (2, 22, 6) (4, 4, 4)
    assert(cache.get(4) == 4); // (2, 22, 6) (4, 4, 5)
    assert(cache.get(4) == 4); // (4, 4, 6) (2, 22, 6) 
	cache.put(5, 50); //  (4, 4, 6) (5, 50, 1)
    assert(cache.get(2) == -1);

	std::cout << "All LFUCache tests passed!" << std::endl;
}

int main() {
    testLFUCache();
    return 0;
}


