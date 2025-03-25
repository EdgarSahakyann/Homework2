#include <iostream>

template<typename T>
struct Allocator {

public:
	Allocator() {}

	T* allocate(size_t n) {

		if (n == 0) {
			return nullptr;
		}

		try {
			T* ptr = static_cast<T*>(::operator new(n * sizeof(T)));
			return ptr;
		}
		catch const (std::bad_alloc&) {
			std::cerr << "Allocation failed." << std::endl;
			return nullptr;
		}
	}

	void deallocate(T* ptr) {
		::operator delete(ptr);
	}
};

int main() {
	Allocator<int> a;

	int* b = a.allocate(10);
	a.deallocate(b);
}
