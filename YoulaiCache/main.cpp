#include <iostream>
#include "lru.h"


int main() {
	LRUCache cache(2); // Create a cache with capacity 2

	cache.put(1, 1); // Cache is {1=1}
	cache.put(2, 2); // Cache is {1=1, 2=2}
	std::cout << cache.get(1) << std::endl; // Returns 1
	cache.put(3, 3); // Evicts key 2, Cache is {1=1, 3=3}
	std::cout << cache.get(2) << std::endl; // Returns -1 (not found)
	cache.put(4, 4); // Evicts key 1, Cache is {3=3, 4=4}
	std::cout << cache.get(1) << std::endl; // Returns -1 (not found)
	std::cout << cache.get(3) << std::endl; // Returns 3
	std::cout << cache.get(4) << std::endl; // Returns 4

	return 0;
}

