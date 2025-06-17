#include <iostream>
#include <string>
#include <cassert> // 用于断言，是测试的利器

// 包含你实现的 LRUCache 头文件
#include "lru.h" // 假设你的文件名为 lru.h

// 使用你的命名空间，让代码更简洁
using namespace YoulaiCache;

// LRU测试函数声明
void testBasicAndUpdates();
void testEvictionPolicy();
void testLruUpdateOnGet();
void testEdgeCases();

// LUR-K测试函数声明
void testLrukPutAndColdGet();
void testLrukPromotion();
void testLrukHotItemBehavior();
void testLrukMainCacheEviction();

int main() {
    // 依次运行所有测试函数
    testBasicAndUpdates();
    testEvictionPolicy();
    testLruUpdateOnGet();
    testEdgeCases();

    // 如果所有 assert 都通过了，程序会执行到这里
    std::cout << "\n========================================" << std::endl;
    std::cout << "?? All LRUCache tests passed successfully!" << std::endl;
    std::cout << "========================================" << std::endl;

    // 依次运行所有测试函数
    testLrukPutAndColdGet();
    testLrukPromotion();
    testLrukHotItemBehavior();
    testLrukMainCacheEviction();

    // 如果所有 assert 都通过了，程序会执行到这里
    std::cout << "\n===========================================" << std::endl;
    std::cout << "?? All LRUKCache tests passed successfully!" << std::endl;
    std::cout << "===========================================" << std::endl;

    return 0;
}

/**
 * @brief 测试基本的 put, get 和更新功能
 */
void testBasicAndUpdates() {
    std::cout << "--- Running Test: Basic Functionality & Updates ---" << std::endl;

    // 使用 int作为Key, std::string作为Value来测试模板功能
    LRUCache<int, std::string> cache(2);

    // 1. 测试基本的 put
    cache.put(1, "apple");
    cache.put(2, "banana");

    // 2. 测试基本的 get
    std::string value;
    assert(cache.get(1, value) && value == "apple");
    assert(cache.get(2, value) && value == "banana");
    std::cout << "  [PASS] Basic put and get work correctly." << std::endl;

    // 3. 测试对已存在 key 的更新
    cache.put(1, "avocado");
    assert(cache.get(1, value) && value == "avocado");
    std::cout << "  [PASS] Value update works correctly." << std::endl;

    // 4. 测试 get(key) 重载版本
    assert(cache.get(2) == "banana");
    assert(cache.get(99) == ""); // get 一个不存在的 key，应返回默认构造的 string
    std::cout << "  [PASS] Overloaded get(key) works correctly." << std::endl;
}

/**
 * @brief 测试核心的 LRU 淘汰策略
 */
void testEvictionPolicy() {
    std::cout << "--- Running Test: Eviction Policy ---" << std::endl;

    LRUCache<int, std::string> cache(2);

    cache.put(1, "cat");
    cache.put(2, "dog");
    // 当前缓存状态 (最新 -> 最旧): {2:"dog"}, {1:"cat"}

    // 1. 插入新元素，触发淘汰
    // 因为 key=1 是最久未使用的，所以它应该被淘汰
    cache.put(3, "elephant");

    // 2. 验证
    std::string value;
    assert(!cache.get(1, value)); // key=1 应该已被淘汰
    assert(cache.get(2, value) && value == "dog");
    assert(cache.get(3, value) && value == "elephant");
    std::cout << "  [PASS] Correctly evicted the least recently used item." << std::endl;
}

/**
 * @brief 测试 get 操作是否会将被访问的条目更新为“最新使用”
 */
void testLruUpdateOnGet() {
    std::cout << "--- Running Test: Update on Access (get) ---" << std::endl;

    LRUCache<int, std::string> cache(2);

    cache.put(1, "ferret");
    cache.put(2, "gorilla");
    // 当前缓存状态 (最新 -> 最旧): {2:"gorilla"}, {1:"ferret"}

    // 1. 访问最久未使用的条目 key=1
    // 这应该让 key=1 变成最新使用的条目
    std::string value;
    assert(cache.get(1, value) && value == "ferret");
    // 当前缓存状态 (最新 -> 最旧): {1:"ferret"}, {2:"gorilla"}

    // 2. 插入新元素，触发淘汰
    // 现在 key=2 变成了最久未使用的，它应该被淘汰
    cache.put(3, "hippo");

    // 3. 验证
    assert(!cache.get(2, value)); // key=2 应该已被淘汰
    assert(cache.get(1, value) && value == "ferret");
    assert(cache.get(3, value) && value == "hippo");
    std::cout << "  [PASS] 'get' correctly updates item to be most recently used." << std::endl;
}

/**
 * @brief 测试容量为0和1等边缘情况
 */
void testEdgeCases() {
    std::cout << "--- Running Test: Edge Cases ---" << std::endl;

    // 1. 测试容量为 1
    LRUCache<int, int> cache_one(1);
    cache_one.put(1, 100);
    assert(cache_one.get(1) == 100);
    cache_one.put(2, 200); // 应该淘汰 key=1
    assert(cache_one.get(1) == 0); // 默认构造的int
    assert(cache_one.get(2) == 200);
    std::cout << "  [PASS] Capacity 1 works correctly." << std::endl;

    // 2. 测试容量为 0
    LRUCache<int, int> cache_zero(0);
    cache_zero.put(1, 100);
    assert(cache_zero.get(1) == 0); // 容量为0，什么也存不进去
    std::cout << "  [PASS] Capacity 0 works correctly." << std::endl;
}

/**
 * @brief 测试新数据写入和“冷”数据获取
 * - 新数据应进入历史缓存
 * - 访问次数不足k次不应晋升
 */
void testLrukPutAndColdGet() {
    std::cout << "--- Running Test: New Puts & Cold Gets ---" << std::endl;

    // 主缓存容量2, 历史缓存容量10, k=3
    LRUKCache<int, std::string> cache(2, 10, 3);

    // 1. 放入一个新数据
    cache.put(1, "apple");
    // 预期：{1:"apple"} 存在于历史区，主缓存为空

    // 2. 第一次获取 (访问历史变为2次)
    assert(cache.get(1) == "apple");

    // 3. 第二次获取 (访问历史变为3次，但get操作在频次增加前判断，所以还不晋升)
    // 这是一个细节：通常是访问第k次后，下一次访问才看到满足条件。
    // 我们假设 get() 内部逻辑是：先get历史频次，再put+1，然后判断。
    assert(cache.get(1) == "apple");

    // 为了验证它还没晋升，我们填满主缓存
    cache.put(100, "hot1");
    cache.get(100); cache.get(100); cache.get(100); // 晋升100
    cache.put(200, "hot2");
    cache.get(200); cache.get(200); cache.get(200); // 晋升200

    // 此刻主缓存应该是 {200:"hot2"}, {100:"hot1"}
    // 如果 key=1 不在主缓存，那么 get(1) 应该仍然能取到值
    assert(cache.get(1) == "apple");
    std::cout << "  [PASS] New items are correctly handled in history cache." << std::endl;
}


/**
 * @brief 测试核心的晋升机制
 * - 当一个key被访问第k次时，应从历史缓存移入主缓存
 */
void testLrukPromotion() {
    std::cout << "--- Running Test: Promotion to Main Cache ---" << std::endl;

    // 主缓存容量1, k=2
    LRUKCache<int, std::string> cache(1, 10, 2);

    // 1. 放入并访问一次 (历史访问1次)
    cache.put(5, "cat");

    // 2. 第二次访问，触发晋升
    assert(cache.get(5) == "cat");
    // 预期：{5:"cat"} 此时应该在主缓存中

    // 3. 如何验证它在主缓存？
    // 我们尝试晋升另一个key，由于主缓存容量为1，key=5应该被淘汰
    cache.put(6, "dog");
    assert(cache.get(6) == "dog"); // 第2次访问，触发key=6晋升

    // 验证key=5是否被淘汰
    // 注意：被淘汰后，它在历史区和主缓存区都不存在了
    assert(cache.get(5) == ""); // 返回默认构造的string
    std::cout << "  [PASS] Item correctly promoted and old item evicted." << std::endl;
}


/**
 * @brief 测试已晋升的“热”数据的行为
 * - 进入主缓存后，应遵循主缓存的LRU策略
 */
void testLrukHotItemBehavior() {
    std::cout << "--- Running Test: Hot Item LRU Behavior ---" << std::endl;

    // 主缓存容量2, k=2
    LRUKCache<int, std::string> cache(2, 10, 2);

    // 1. 晋升两个key到主缓存
    cache.put(1, "hot_A");
    cache.get(1); // 晋升 key=1
    cache.put(2, "hot_B");
    cache.get(2); // 晋升 key=2
    // 主缓存状态 (最新 -> 最旧): {2:"hot_B"}, {1:"hot_A"}

    // 2. 访问 key=1，使其变为最新
    assert(cache.get(1) == "hot_A");
    // 主缓存状态 (最新 -> 最旧): {1:"hot_A"}, {2:"hot_B"}

    // 3. 晋升第三个key，验证谁被淘汰
    cache.put(3, "hot_C");
    cache.get(3); // 晋升 key=3
    // 此时主缓存中最久未使用的是 key=2，它应该被淘汰

    // 4. 验证
    assert(cache.get(1) == "hot_A");
    assert(cache.get(3) == "hot_C");
    assert(cache.get(2) == ""); // key=2 应该被淘汰了
    std::cout << "  [PASS] Hot items correctly follow LRU policy in main cache." << std::endl;
}


/**
 * @brief 测试历史缓存的淘汰
 */
void testLrukMainCacheEviction() {
    std::cout << "--- Running Test: History Cache Eviction ---" << std::endl;

    // 历史缓存容量只有2, k=100 (确保不会晋升)
    LRUKCache<int, int> cache(5, 2, 100);

    cache.put(1, 10);
    cache.put(2, 20);
    // 历史区 (最新 -> 最旧): {2:20, f=1}, {1:10, f=1}

    // 访问key=1
    cache.get(1);
    // 历史区 (最新 -> 最旧): {1:10, f=2}, {2:20, f=1}

    // 插入新key=3
    cache.put(3, 30);
    // 历史缓存满了，最久未使用的key=2被淘汰

    // 验证
    assert(cache.get(2) == 0); // key=2已被淘汰
    assert(cache.get(1) == 10);
    assert(cache.get(3) == 30);
    std::cout << "  [PASS] History cache correctly evicts its own LRU item." << std::endl;
}