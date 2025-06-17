#include <iostream>
#include <string>
#include <cassert> // ���ڶ��ԣ��ǲ��Ե�����

// ������ʵ�ֵ� LRUCache ͷ�ļ�
#include "lru.h" // ��������ļ���Ϊ lru.h

// ʹ����������ռ䣬�ô�������
using namespace YoulaiCache;

// ���Ժ�������
void testBasicAndUpdates();
void testEvictionPolicy();
void testLruUpdateOnGet();
void testEdgeCases();

int main() {
    // �����������в��Ժ���
    testBasicAndUpdates();
    testEvictionPolicy();
    testLruUpdateOnGet();
    testEdgeCases();

    // ������� assert ��ͨ���ˣ������ִ�е�����
    std::cout << "\n========================================" << std::endl;
    std::cout << "?? All LRUCache tests passed successfully!" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

/**
 * @brief ���Ի����� put, get �͸��¹���
 */
void testBasicAndUpdates() {
    std::cout << "--- Running Test: Basic Functionality & Updates ---" << std::endl;

    // ʹ�� int��ΪKey, std::string��ΪValue������ģ�幦��
    LRUCache<int, std::string> cache(2);

    // 1. ���Ի����� put
    cache.put(1, "apple");
    cache.put(2, "banana");

    // 2. ���Ի����� get
    std::string value;
    assert(cache.get(1, value) && value == "apple");
    assert(cache.get(2, value) && value == "banana");
    std::cout << "  [PASS] Basic put and get work correctly." << std::endl;

    // 3. ���Զ��Ѵ��� key �ĸ���
    cache.put(1, "avocado");
    assert(cache.get(1, value) && value == "avocado");
    std::cout << "  [PASS] Value update works correctly." << std::endl;

    // 4. ���� get(key) ���ذ汾
    assert(cache.get(2) == "banana");
    assert(cache.get(99) == ""); // get һ�������ڵ� key��Ӧ����Ĭ�Ϲ���� string
    std::cout << "  [PASS] Overloaded get(key) works correctly." << std::endl;
}

/**
 * @brief ���Ժ��ĵ� LRU ��̭����
 */
void testEvictionPolicy() {
    std::cout << "--- Running Test: Eviction Policy ---" << std::endl;

    LRUCache<int, std::string> cache(2);

    cache.put(1, "cat");
    cache.put(2, "dog");
    // ��ǰ����״̬ (���� -> ���): {2:"dog"}, {1:"cat"}

    // 1. ������Ԫ�أ�������̭
    // ��Ϊ key=1 �����δʹ�õģ�������Ӧ�ñ���̭
    cache.put(3, "elephant");

    // 2. ��֤
    std::string value;
    assert(!cache.get(1, value)); // key=1 Ӧ���ѱ���̭
    assert(cache.get(2, value) && value == "dog");
    assert(cache.get(3, value) && value == "elephant");
    std::cout << "  [PASS] Correctly evicted the least recently used item." << std::endl;
}

/**
 * @brief ���� get �����Ƿ�Ὣ�����ʵ���Ŀ����Ϊ������ʹ�á�
 */
void testLruUpdateOnGet() {
    std::cout << "--- Running Test: Update on Access (get) ---" << std::endl;

    LRUCache<int, std::string> cache(2);

    cache.put(1, "ferret");
    cache.put(2, "gorilla");
    // ��ǰ����״̬ (���� -> ���): {2:"gorilla"}, {1:"ferret"}

    // 1. �������δʹ�õ���Ŀ key=1
    // ��Ӧ���� key=1 �������ʹ�õ���Ŀ
    std::string value;
    assert(cache.get(1, value) && value == "ferret");
    // ��ǰ����״̬ (���� -> ���): {1:"ferret"}, {2:"gorilla"}

    // 2. ������Ԫ�أ�������̭
    // ���� key=2 ��������δʹ�õģ���Ӧ�ñ���̭
    cache.put(3, "hippo");

    // 3. ��֤
    assert(!cache.get(2, value)); // key=2 Ӧ���ѱ���̭
    assert(cache.get(1, value) && value == "ferret");
    assert(cache.get(3, value) && value == "hippo");
    std::cout << "  [PASS] 'get' correctly updates item to be most recently used." << std::endl;
}

/**
 * @brief ��������Ϊ0��1�ȱ�Ե���
 */
void testEdgeCases() {
    std::cout << "--- Running Test: Edge Cases ---" << std::endl;

    // 1. ��������Ϊ 1
    LRUCache<int, int> cache_one(1);
    cache_one.put(1, 100);
    assert(cache_one.get(1) == 100);
    cache_one.put(2, 200); // Ӧ����̭ key=1
    assert(cache_one.get(1) == 0); // Ĭ�Ϲ����int
    assert(cache_one.get(2) == 200);
    std::cout << "  [PASS] Capacity 1 works correctly." << std::endl;

    // 2. ��������Ϊ 0
    LRUCache<int, int> cache_zero(0);
    cache_zero.put(1, 100);
    assert(cache_zero.get(1) == 0); // ����Ϊ0��ʲôҲ�治��ȥ
    std::cout << "  [PASS] Capacity 0 works correctly." << std::endl;
}