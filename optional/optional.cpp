#include <iostream>

template <typename T>
class optional {

public:
	optional() : has_value(false) {}

	optional(const T& data) : has_value(true), val(data) {}

	optional(const optional& other) noexcept : has_value(other.has_value) {
		if (has_value) {
			new(&val) T(other.val);
		}
	}

	optional(optional&& other) noexcept : has_value(other.has_value) {
		if(has_value) {
			new(&val) T(std::move(other.val));
			other.has_value = false;
		}
	}

	~optional() {
		reset();
	}

	bool has_value_() {
		return has_value;
	}

	T value() {
		if (has_value) {
			return val;
		}
		else {
			throw std::runtime_error("There is no value!");
		}
	}

	void emplace(T&& value) {
		if (has_value) {
			val.~T();
		}
		new(&val) T(std::forward<T>(value));
		has_value = true;
	}

	void reset() {
		if (has_value) {
			val.~T();
			has_value = false;
		}
	}

	void swap(optional& other) {
		std::swap(val, other.val);
		std::swap(has_value, other.has_value);
	}

	T value_or(const T& def_value) const {
		return has_value ? val : def_value;
	}

private:
	T val;
	bool has_value;

};

int main() {
	optional<int> op1;
	optional<int> op2(11);

	if (op2.has_value_()) {
		std::cout << "op2 has value: " << op2.value() << std::endl;
	}

	op1.emplace(22);
	std::cout << "op1 after emplace: " << op1.value() << std::endl;

	std::cout << "op1 value or default: " << op1.value_or(100) << std::endl;

	op1.reset();
	std::cout << "op1 after reset: " << op1.value_or(100) << std::endl;

	return 0;
}
