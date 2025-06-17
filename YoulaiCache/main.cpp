#include <iostream>
#include <string>
#include <cassert> // ���ڶ��ԣ��ǲ��Ե�����

// ������ʵ�ֵ� LRUCache ͷ�ļ�
#include "lru.h" // ��������ļ���Ϊ lru.h

// ʹ����������ռ䣬�ô�������
using namespace YoulaiCache;

// LRU���Ժ�������
void testBasicAndUpdates();
void testEvictionPolicy();
void testLruUpdateOnGet();
void testEdgeCases();

// LUR-K���Ժ�������
void testLrukPutAndColdGet();
void testLrukPromotion();
void testLrukHotItemBehavior();
void testLrukMainCacheEviction();

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

    // �����������в��Ժ���
    testLrukPutAndColdGet();
    testLrukPromotion();
    testLrukHotItemBehavior();
    testLrukMainCacheEviction();

    // ������� assert ��ͨ���ˣ������ִ�е�����
    std::cout << "\n===========================================" << std::endl;
    std::cout << "?? All LRUKCache tests passed successfully!" << std::endl;
    std::cout << "===========================================" << std::endl;

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

/**
 * @brief ����������д��͡��䡱���ݻ�ȡ
 * - ������Ӧ������ʷ����
 * - ���ʴ�������k�β�Ӧ����
 */
void testLrukPutAndColdGet() {
    std::cout << "--- Running Test: New Puts & Cold Gets ---" << std::endl;

    // ����������2, ��ʷ��������10, k=3
    LRUKCache<int, std::string> cache(2, 10, 3);

    // 1. ����һ��������
    cache.put(1, "apple");
    // Ԥ�ڣ�{1:"apple"} ��������ʷ����������Ϊ��

    // 2. ��һ�λ�ȡ (������ʷ��Ϊ2��)
    assert(cache.get(1) == "apple");

    // 3. �ڶ��λ�ȡ (������ʷ��Ϊ3�Σ���get������Ƶ������ǰ�жϣ����Ի�������)
    // ����һ��ϸ�ڣ�ͨ���Ƿ��ʵ�k�κ���һ�η��ʲſ�������������
    // ���Ǽ��� get() �ڲ��߼��ǣ���get��ʷƵ�Σ���put+1��Ȼ���жϡ�
    assert(cache.get(1) == "apple");

    // Ϊ����֤����û��������������������
    cache.put(100, "hot1");
    cache.get(100); cache.get(100); cache.get(100); // ����100
    cache.put(200, "hot2");
    cache.get(200); cache.get(200); cache.get(200); // ����200

    // �˿�������Ӧ���� {200:"hot2"}, {100:"hot1"}
    // ��� key=1 ���������棬��ô get(1) Ӧ����Ȼ��ȡ��ֵ
    assert(cache.get(1) == "apple");
    std::cout << "  [PASS] New items are correctly handled in history cache." << std::endl;
}


/**
 * @brief ���Ժ��ĵĽ�������
 * - ��һ��key�����ʵ�k��ʱ��Ӧ����ʷ��������������
 */
void testLrukPromotion() {
    std::cout << "--- Running Test: Promotion to Main Cache ---" << std::endl;

    // ����������1, k=2
    LRUKCache<int, std::string> cache(1, 10, 2);

    // 1. ���벢����һ�� (��ʷ����1��)
    cache.put(5, "cat");

    // 2. �ڶ��η��ʣ���������
    assert(cache.get(5) == "cat");
    // Ԥ�ڣ�{5:"cat"} ��ʱӦ������������

    // 3. �����֤���������棿
    // ���ǳ��Խ�����һ��key����������������Ϊ1��key=5Ӧ�ñ���̭
    cache.put(6, "dog");
    assert(cache.get(6) == "dog"); // ��2�η��ʣ�����key=6����

    // ��֤key=5�Ƿ���̭
    // ע�⣺����̭��������ʷ����������������������
    assert(cache.get(5) == ""); // ����Ĭ�Ϲ����string
    std::cout << "  [PASS] Item correctly promoted and old item evicted." << std::endl;
}


/**
 * @brief �����ѽ����ġ��ȡ����ݵ���Ϊ
 * - �����������Ӧ��ѭ�������LRU����
 */
void testLrukHotItemBehavior() {
    std::cout << "--- Running Test: Hot Item LRU Behavior ---" << std::endl;

    // ����������2, k=2
    LRUKCache<int, std::string> cache(2, 10, 2);

    // 1. ��������key��������
    cache.put(1, "hot_A");
    cache.get(1); // ���� key=1
    cache.put(2, "hot_B");
    cache.get(2); // ���� key=2
    // ������״̬ (���� -> ���): {2:"hot_B"}, {1:"hot_A"}

    // 2. ���� key=1��ʹ���Ϊ����
    assert(cache.get(1) == "hot_A");
    // ������״̬ (���� -> ���): {1:"hot_A"}, {2:"hot_B"}

    // 3. ����������key����֤˭����̭
    cache.put(3, "hot_C");
    cache.get(3); // ���� key=3
    // ��ʱ�����������δʹ�õ��� key=2����Ӧ�ñ���̭

    // 4. ��֤
    assert(cache.get(1) == "hot_A");
    assert(cache.get(3) == "hot_C");
    assert(cache.get(2) == ""); // key=2 Ӧ�ñ���̭��
    std::cout << "  [PASS] Hot items correctly follow LRU policy in main cache." << std::endl;
}


/**
 * @brief ������ʷ�������̭
 */
void testLrukMainCacheEviction() {
    std::cout << "--- Running Test: History Cache Eviction ---" << std::endl;

    // ��ʷ��������ֻ��2, k=100 (ȷ���������)
    LRUKCache<int, int> cache(5, 2, 100);

    cache.put(1, 10);
    cache.put(2, 20);
    // ��ʷ�� (���� -> ���): {2:20, f=1}, {1:10, f=1}

    // ����key=1
    cache.get(1);
    // ��ʷ�� (���� -> ���): {1:10, f=2}, {2:20, f=1}

    // ������key=3
    cache.put(3, 30);
    // ��ʷ�������ˣ����δʹ�õ�key=2����̭

    // ��֤
    assert(cache.get(2) == 0); // key=2�ѱ���̭
    assert(cache.get(1) == 10);
    assert(cache.get(3) == 30);
    std::cout << "  [PASS] History cache correctly evicts its own LRU item." << std::endl;
}