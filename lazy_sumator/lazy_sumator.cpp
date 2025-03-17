#include <iostream>
#include <vector>

template <typename T>
class Matrix {

private:
	int row, col;
	std::vector<std::vector<T>> matrix1;
	std::vector<std::vector<T>> matrix2;

public:

	Matrix(int r, int c) : row(r), col(c) {
		matrix1.resize(row, std::vector<T>(col));
		matrix2.resize(row, std::vector<T>(col));

		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				matrix1[i][j] = rand() % 20;
				matrix2[i][j] = rand() % 20;
			}
		}
	}

	void print() const {
		std::cout << "Matrix 1:" << std::endl;
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				std::cout << matrix1[i][j] << " ";
			}
			std::cout << std::endl;
		}

		std::cout << std::endl << "Matrix 2:" << std::endl;
		for (int i = 0; i < row; ++i) {
			for (int j = 0; j < col; ++j) {
				std::cout << matrix2[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}

	const std::vector<std::vector<T>>& get_matrix1() const { 
		return matrix1; 
	}

	const std::vector<std::vector<T>>& get_matrix2() const { 
		return matrix2; 
	}

	int get_row() const { 
		return row; 
	}

	int get_col() const { 
		return col; 
	}

};

template <typename T>
class lazy_summator{

private:
	const Matrix<T>& mat;

public:
	lazy_summator(const Matrix<T>& matrix) : mat(matrix) {}

	T operator() (int i, int j) {
		if (i < 0 || i >= mat.get_row() || j < 0 || j >= mat.get_col()) {
			throw std::out_of_range("Index out of range");
		}
		return mat.get_matrix1()[i][j] + mat.get_matrix2()[i][j];
	}

};

int main() {
	srand(time(0));
	Matrix<int> m(3, 3);
	m.print();

	lazy_summator<int> lazy_sum(m);

	try {
		std::cout << "Lazy Sum at (0, 0): " << lazy_sum(0, 0) << std::endl;
		std::cout << "Lazy Sum at (0, 0): " << lazy_sum(1, 1) << std::endl;
		std::cout << "Lazy Sum at (0, 0): " << lazy_sum(2, 2) << std::endl;
	}
	catch (const std::out_of_range& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
