#include <iostream>
#include <vector>

template <typename T>
class memory_pool {

private:
	size_t used_size;
	size_t total_size;
	std::vector<T> pool;

public:
	memory_pool(size_t size) : used_size(0), total_size(size) {
		try {
			pool.resize(size);
		}
		catch (const std::bad_alloc&) {
			throw;
		}
	}

	~memory_pool() {}

	T* allocate(size_t size) {
		if (used_size + size > total_size) {
			throw std::bad_alloc;
		}

		T* ptr = &pool[used_size];
		used_size += size;

		return ptr;
	}

	void deallocate() {
		used_size = 0;
	}

	void printUsage() const {
		std::cout << "Memory used: " << used_size << " of " << total_size << std::endl;
	}

};


int main() {
	memory_pool<int> mp(10);

	mp.printUsage();
	int* ptr = mp.allocate(5);
	mp.printUsage();
	mp.deallocate();
	mp.printUsage();

	return 0;
}
