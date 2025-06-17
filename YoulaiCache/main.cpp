#include <iostream>
#include <string>
#include <cassert> // 用于断言，是测试的利器

// 包含你实现的 LRUCache 头文件
#include "lru.h" // 假设你的文件名为 lru.h

// 使用你的命名空间，让代码更简洁
using namespace YoulaiCache;

// 测试函数声明
void testBasicAndUpdates();
void testEvictionPolicy();
void testLruUpdateOnGet();
void testEdgeCases();

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