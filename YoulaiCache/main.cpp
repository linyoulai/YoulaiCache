#include <iostream>
#include <cassert>
#include "lfu.h"
#include "lru.h"

void testLRUCache() {
    // 1. ���Ի������ܺ͸���
    lru::LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    assert(cache.get(1) == 1);
    cache.put(2, 22); // ����
    assert(cache.get(2) == 22);

    // 2. ������̭ (key=1 Ӧ�������δʹ�õ�)
    cache.put(3, 3); // ��ʱ key=1 Ӧ�ñ���̭
    assert(cache.get(1) == -1);
    assert(cache.get(2) == 22);
    assert(cache.get(3) == 3);

    // 3. ���Է��ʺ����LRU˳��
    // ��ǰ˳�� (��->��): 3, 2
    cache.get(2); // ���� key=2��ʹ���Ϊ����
    // ��ǰ˳�� (��->��): 2, 3
    cache.put(4, 4); // ��ʱӦ����̭ key=3
    assert(cache.get(3) == -1);
    assert(cache.get(2) == 22);
    assert(cache.get(4) == 4);

    std::cout << "All LRUCache tests passed!" << std::endl;
}

void testLFUCache() {
	// 1. ���Ի������ܺ͸���
	lfu::LFUCache cache(2);
	cache.put(1, 1); // (key, value, freq) = (1, 1, 1)
	cache.put(2, 2); // (key, value, freq) = (2, 2, 1)
	assert(cache.get(1) == 1); // (1, 1, 2), (2, 2, 1)
	cache.put(2, 22); // (1, 1, 2), (2, 22, 2)
	assert(cache.get(2) == 22); // (1, 1, 2), (2, 22, 3)

	// 2. ������̭ (key=1 Ӧ�������δʹ�õ�)
	cache.put(3, 3); // (2, 22, 3) (3, 3, 1)
	assert(cache.get(1) == -1); // 
	assert(cache.get(2) == 22); // (2, 22, 4) (3, 3, 1)
	assert(cache.get(3) == 3); // (2, 22, 4) (3, 3, 2)

	// 3. ���Է��ʺ����LFU˳��
	// ��ǰ˳�� (��->��): 3, 2
	cache.get(2); // (2, 22, 5) (3, 3, 2)
	// ��ǰ˳�� (��->��): 2, 3
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


