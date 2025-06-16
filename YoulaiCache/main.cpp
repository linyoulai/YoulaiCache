#include <iostream>
#include "lfu.h"

//void test_lru() {
//	LRUCache cache(2); // Create a cache with capacity 2
//
//	cache.put(1, 1); // Cache is {1=1}
//	cache.put(2, 2); // Cache is {1=1, 2=2}
//	std::cout << cache.get(1) << std::endl; // Returns 1
//	cache.put(3, 3); // Evicts key 2, Cache is {1=1, 3=3}
//	std::cout << cache.get(2) << std::endl; // Returns -1 (not found)
//	cache.put(4, 4); // Evicts key 1, Cache is {3=3, 4=4}
//	std::cout << cache.get(1) << std::endl; // Returns -1 (not found)
//	std::cout << cache.get(3) << std::endl; // Returns 3
//	std::cout << cache.get(4) << std::endl; // Returns 4
//}

void test_lfu() {
	LFUCache cache(5); // Create a cache with capacity 2

	cache.put(1, 10);
	cache.put(2, 20);
	cache.put(3, 30);
	cache.put(4, 40);
	cache.put(5, 50);
	std::cout << cache.get(1) << std::endl; // Returns 10
	cache.put(6, 60); // Evicts key 2, Cache is {1=10, 3=30, 4=40, 5=50, 6=60}
	std::cout << cache.get(2) << std::endl; // Returns -1 (not found)
	cache.put(7, 70); // Evicts key 3, Cache is {1=10, 4=40, 5=50, 6=60, 7=70}
	std::cout << cache.get(3) << std::endl; // Returns -1 (not found)
	std::cout << cache.get(4) << std::endl; // Returns 40
	std::cout << cache.get(5) << std::endl; // Returns 50
	std::cout << cache.get(6) << std::endl; // Returns 60
	std::cout << cache.get(7) << std::endl; // Returns 70
	cache.put(8, 80); // Evicts key 1, Cache is {4=40, 5=50, 6=60, 7=70, 8=80}
	std::cout << cache.get(1) << std::endl; // Returns -1 (not found)
}

int main() {
	test_lfu();

	return 0;
}

