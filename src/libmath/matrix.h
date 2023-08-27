#pragma once

#include <libmath/math_exception.h>
#include <libmath/math_settings.h>
#include <libmath/boolean.h>

#include <vector>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#include <chrono>

namespace math
{
	//! Enum class for matrix representation definition
	//! @sa repr_
	enum class MatRep
	{
		Row = 0,
		Column
	};
	//! Class Matrix
	/* Class representing matrix of type T
	 */
	template <typename T>
	class Matrix
	{

	private:
		//! Number of rows
		size_t rows_;
		//! Numner of columns
		size_t cols_;
		//! Internal serial container for matrix storage
		std::vector<T> mvec_;
		//! Type of matrix representation
		/*! Representation can be:
			- row (=0, storage row by row)
			- column (=1, storage column by column)
			Defaults to row orientation (=0)
		*/
		MatRep repr_ = MatRep::Row;

	public:
		/**
		 * @brief Default constructor
		 * @return Empty row-oriented Matrix of T
		 */
		Matrix();
		/**
		 * @brief The copy constructor
		 */
		Matrix(const Matrix<T>& matrix);

		/**
		 * @brief Square matrix constructor
		 * @param size Dimensions of square matrix
		 * @param repr Representation (0 - row,default; 1 - column)
		 * @return Matrix size*size of T
		 */
		explicit Matrix(size_t size, MatRep repr = MatRep::Row);

		/**
		 * @brief Arbitrary size matrix constructor
		 * @param rows Number of rows
		 * @param cols Number of columns
		 * @param repr Representation (0 - row,default; 1 - column)
		 * @return Matrix rows*columns of T
		 */
		Matrix(size_t rows, size_t cols, MatRep repr = MatRep::Row);

		/**
		 * @brief Column-(N*1) or row-vector (1*N) constructor by copy from std::vector
		 * @param vector Vector for initialization
		 * @param vertical=1 Default orientation (vertical, i.e. column-vector).
		 * If orientation is vertical then representation will be column (1) and
		 * vise-versa
		 * @return Column-matrix vector.size()*1
		 */
		template <class T1>
		explicit Matrix(const std::vector<T1>& vector, bool vertical = true);

		//! Matrix from list initialization (initializer_list)
		/*! Initialization from initializer list with check for list
		 *  correctness. Matrix is in row representation
		 * @param listMatrix matrix initializer_list
		 * @return Arbitrary matrix
		 * @throws math::Exception::Type::IncorrectInitializerList
		 */
		template <class T1>
		Matrix(std::initializer_list<std::initializer_list<T1>> listMatrix);

		~Matrix()
		{

		};

		/**
		* @brief Matrix representation
		*/
		MatRep representation()
		{
			return repr_;
		}

		/**
		 * @brief number of rows
		 */
		size_t rows() const
		{
			return rows_;
		}

		/**
		 * @brief number of columns
		 */
		size_t cols() const
		{
			return cols_;
		}

		/**
		 * @brief total number of elements in matrix
		 */
		size_t numel() const
		{
			return rows_ * cols_;
		}

		/**
		 * @brief get internal vector
		 */
		std::vector<T> vectorized()
		{
			return mvec_;
		}

		/**
		 * @brief get internal vector
		 */
		std::vector<T> vectorized() const
		{
			return mvec_;
		}


		/**
		 * @brief get reference to element at specified position (i,j)
		 * @param row row number (starting from 0)
		 * @param col column number (starting from 0)
		 */
		T& operator()(size_t row, size_t col);

		/**
		 * @brief const version of operator()
		 * @param row row number (starting from 0)
		 * @param col column number (starting from 0)
		 * @sa T& operator()(size_t row, size_t col)
		 */
		T operator()(size_t row, size_t col) const;

		/**
		 * @brief element indexation only for row-oriented matrices [deprecated]
		 *
		 * @param index row number (starting from 0). Column deduced from linear vector arrangement.
		 * @return pointer to element
		 * @throws math::Exception(Exception::Type::IncorrectMatRepForIndexOperator) in case of column matrix representation
		 * @throws math::Exception(Exception::Type::IndexOutOfBounds) in case of index > (rows-1)
		 */
		T* operator[](size_t index);

		/**
		* @brief Fill matrix by value val
		* @param val: Value to fill matrix
		*/
		void fill(T val);

		/**
		* @brief Fill matrix by random values with seed seed
		* @param seed: Seed for random generatoe
		*/
		void rfill(unsigned int seed);

		/**
		 * @brief Print Matrix to std out. Specified by format
		 */
		void print(int prec = 5);

		/**
		* @brief Print matrix to string. Specified by format
		*/
		void print(std::string& img, int prec = 5);

		/**
		 * @brief Get transposed matrix
		 * @return transposed Matrix
		 */
		Matrix<T> getTr() const;

		/**
		 * @brief Change matrix to transposed
		 */
		void tr();

		/**
		* @brief Matrix p-norm (<a href="http://num-anal.srcc.msu.ru/prac_pos/poslist/posobie%206%20zadachi%20normy.pdf">Арушанян, 2-5)</a>)
		*/
		auto pnorm(const int p);

		/**
		 * @brief Check equality of the two matrices of the same type <T>
		 */
		template <class T1>
		friend bool operator==(const Matrix<T1>& m1, Matrix<T1> const& m2);

		/**
		 * @brief Max element of matrix
		 * @return MAX: max element of matrix
		 */
		T maxElement();

		/**
		 * @brief LU decomposition
		 *
		 * Calculation of Lower (L) and Upper (U) matrixes comprising LU decomposition of this matrix: M = L*U
		 * Implemented based on formulation from [Вержбицкий В.М. Основы численных методов. Учебник для вузов. — Высшая школа, 2002. — ISBN 5-06-004020-8]
		 * pp. 63-64.
		 * @param L - reference to Matrix L of the same size
		 * @param U - reference to Matrix U of the same size
		 * @throws math::Exception(Exception::Type::NonSquareMatrixDecomposition)
		 * @throws math::Exception(Exception::Type::DecompositionArgumentIncorrectSize)
		 * TODO: сделать перегрузку для возвращения LU в виде единой матрицы L-E+U (стр. 73 Вержбицкого)
		 */
		void decompLU(Matrix<T>& Matrix_L, Matrix<T>& Matrix_U) const;

		/**
		 * @brief overload of decompLU returning combined matrix L+U-E
		 * @return combined matrix L+U-E (Вержбицкий стр 73 пример 2.2)
		 */
		Matrix<T> decompLU() const;

		/**
		 * @brief Matrix determinant
		 *
		 * Calculate for matrix determinant
		 * @param method. If method :
		 *   = 0 - det() calculated by recursive cofactor algo (default)
		 *   = 1 - det() calculated by LU-decomposition algo
		 *
		 * @throws math::Exception::Type::NonSquareMatrixDeterminant
		 * @return matrix determinant of type <T>
		 * TODO: implement selection of row to make decomposition in cofactor method
		 */
		T det(unsigned int method = 0) const;

		/**
		* @brief multiplication of a matrix by a number
		* @param M matrix
		* @param n number
		* @return multiplied matrix M by number n (M * n)
		*/
		template<typename T1>
		friend Matrix<T1> operator*(const Matrix<T1>& M, T1 n);

		/**
		* @brief permutation overload of operator*(const Matrix<T>&M, T n)
		* @param n number
		* @param M matrix
		* @return multiplied matrix M by number n (n * M)
		*/
		friend Matrix<T> operator*(T n, const Matrix<T>& M)
		{
			return (operator*(M, n));
		}

		/**
		* @brief overload operator*= for multiplication by a number
		* @param n number
		* @return multiplied matrix M by number n (n * M)
		*/
		Matrix<T>& operator*=(T n);

		/**
		* @brief Multiplication of a matrix by a matrix
		* @detailed The type of matrix returned is the same as the type of a first argument.
		* (<a href="https://users.cs.utah.edu/~hari/teaching/paralg/tutorial/05_Cannons.html">Cannon's algorithm)</a>) used for multithreaded matrix multiplication.
		* @throw Exception::Type::IncorrectSizeForMatrixMultiplication
		* @return Multiplication of matrices
		*/
		template<typename T1>
		friend Matrix<T1> operator*(const Matrix<T1>& A, const Matrix<T1>& B);

		/**
		* @brief overload operator*= for multiplication of matrices
		* @param M1 matrix
		* @return multiplied matrix M by M1
		*/
		Matrix<T>& operator*=(const Matrix<T>& M1);

		/**
		* @brief Addition of a matrix and a number
		* @param M matrix
		* @param n number
		* @return sum of matrix M with n (M + n) element by element
		*/
		template<typename T1>
		friend Matrix<T1> operator+(const Matrix<T1>& M, T1 n);

		/**
		* @brief permutation overload of operator+(const Matrix<T>&M, T n)
		* @param n number
		* @param M matrix
		* @return sum of matrix M with n (M + n) element by element
		*/
		friend Matrix<T> operator+(T n, const Matrix<T>& M)
		{
			return (operator+(M, n));
		}

		/**
		* @brief overload operator+= for addition with number
		* @param n number
		* @return sum of matrix M with n (M + n) element by element
		*/
		Matrix<T>& operator+=(T n)
		{
			for (size_t i = 0; i < this->mvec_.size(); ++i)
			{
				this->mvec_[i] += n;
			}
			return *this;
		}

		/**
		* @brief Addition of matrices (element by element)
		* @detailed The type of matrix returned is the same as the type of a first argument
		* @throw Exception::Type::NonEqualMatrixSizes
		* @return Addition of matrices
		*/
		template<typename T1>
		friend Matrix<T1> operator+(const Matrix<T1>& A, const Matrix<T1>& B);

		/**
		* @brief overload operator+= for sum of matrices
		* @param M1 matrix
		* @return sum of matrices M and M1
		*/
		Matrix<T>& operator+=(const Matrix<T>& M1);

		/**
		* @brief Subtraction of a matrix and a number
		* @param M matrix
		* @param n number
		* @return subtraction of matrix M with n (M + n) element by element
		*/
		template<typename T1>
		friend Matrix<T1> operator-(const Matrix<T1>& M, T1 n);

		/**
		* @brief permutation overload of operator-(const Matrix<T>&M, T n)
		* @param n number
		* @param M matrix
		* @return subtraction of matrix M with n (M + n) element by element
		*/
		friend Matrix<T> operator-(T n, const Matrix<T>& M)
		{
			return (operator-(M, n));
		}

		/**
		* @brief overload operator-= for subtraction with number
		* @param n number
		* @return subtraction of matrix M with n (M + n) element by element
		*/
		Matrix<T>& operator-=(T n)
		{
			for (size_t i = 0; i < this->mvec_.size(); ++i)
			{
				this->mvec_[i] -= n;
			}
			return *this;
		}

		/**
		* @brief Subtraction of matrices (element by element)
		* @detailed The type of matrix returned is the same as the type of a first argument
		* @throw Exception::Type::NonEqualMatrixSizes
		* @return Subtraction of matrices
		*/
		template<typename T1>
		friend Matrix<T1> operator-(const Matrix<T1>& A, const Matrix<T1>& B);

		/**
		* @brief overload operator-= for sum of matrices
		* @param M1 matrix
		* @return sum of matrices M and M1
		*/
		Matrix<T>& operator-=(const Matrix<T>& M1);

		/**
		 * @brief calculate inversed matrix
		 * @return inversed matrix
		 */
		Matrix<T> inverse();

		/**
		 * @brief Compare this matrix with another with defined precision
		 *
		 * Comparison doesn't consider representatioins (row or column) of matrices
		 * @param M matrix to compare
		 * @return true if matrices are equal
		 */
		bool compare(const Matrix<T>& M, T eps = math::settings::CurrentSettings.targetTolerance);

	private:
		/**
		 * @brief Utility function to organize cofactor algo for det calculation
		 *
		 * @param rowsExcl
		 * @param colsExcl
		 * @return T
		 */
		T detIterative(unsigned int iteration,
			std::vector<size_t>& rowsExcl,
			std::vector<size_t>& colsExcl) const;

	}; // class Matrix()

	template <class T>
	Matrix<T>::Matrix()
		: rows_{ 0 }, cols_{ 0 }, mvec_{ std::vector<T>() } {};

	template <class T>
	Matrix<T>::Matrix(const Matrix<T>& matrix)
		: rows_{ matrix.rows_ },
		cols_{ matrix.cols_ },
		mvec_{ matrix.mvec_ },
		repr_{ matrix.repr_ } {};

	template <class T>
	Matrix<T>::Matrix(size_t size, MatRep repr)
		: rows_{ size },
		cols_{ size },
		mvec_{ std::vector<T>(size * size) },
		repr_{ repr } {};

	template <class T>
	Matrix<T>::Matrix(size_t rows, size_t cols, MatRep repr)
		: rows_{ rows },
		cols_{ cols },
		mvec_{ std::vector<T>(rows * cols) },
		repr_{ repr } {}

	template <typename T>
	template <typename T1>
	Matrix<T>::Matrix(const std::vector<T1>& vector, bool vertical)
		: mvec_{ vector }
	{
		if (vertical)
		{
			rows_ = vector.size();
			cols_ = 1;
			repr_ = MatRep::Column;
		}
		else
		{
			cols_ = vector.size();
			rows_ = 1;
			repr_ = MatRep::Row;
		}
	};

	template <typename T>
	template <typename T1>
	Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T1>> listMatrix)
		: rows_{ 0 }, cols_{ 0 }, repr_{ MatRep::Row }
	{
		mvec_ = std::vector<T>{};
		for (auto row_itr = listMatrix.begin(); row_itr != listMatrix.end(); ++row_itr)
		{
			size_t cols_check = 0u;
			for (auto col_itr = row_itr->begin(); col_itr != row_itr->end(); ++col_itr)
			{
				mvec_.push_back(static_cast<T>(*col_itr));
				if (rows_ == 0u)
					++cols_;
				++cols_check;
			}
			if (cols_ != cols_check)
			{
				throw(math::ExceptionInvalidValue("Incorrect initializer list for construct math::Matrix"));
			}
			++rows_;
		}
	}

	template <typename T>
	T& Matrix<T>::operator()(size_t row, size_t col)
	{
		if (row >= this->rows_)
		{
			throw(ExceptionIndexOutOfBounds("Matrix<T>::operator(): row index out of bounds!"));
		}
		if (col >= this->cols_)
		{
			throw(ExceptionIndexOutOfBounds("Matrix<T>::operator(): col index out of bounds!"));
		}

		size_t pos{ 0 };
		if (this->repr_ == math::MatRep::Row) // row repr
			pos = row * cols_ + col;
		else if (this->repr_ == math::MatRep::Column) // column repr
			pos = row + rows_ * col;
		return this->mvec_.at(pos);
	}

	template <typename T>
	T Matrix<T>::operator()(size_t row, size_t col) const
	{
		if (row >= this->rows_)
		{
			throw(ExceptionIndexOutOfBounds("Matrix<T>::operator(): row index out of bounds!"));
		}
		if (col >= this->cols_)
		{
			throw(ExceptionIndexOutOfBounds("Matrix<T>::operator(): col index out of bounds!"));
		}

		size_t pos{ 0 };
		if (this->repr_ == math::MatRep::Row) // row repr
			pos = row * cols_ + col;
		else if (this->repr_ == math::MatRep::Column) // column repr
			pos = row + rows_ * col;
		return this->mvec_.at(pos);
	}

	template <typename T>
	T* Matrix<T>::operator[](size_t index)
	{
		if (repr_ != math::MatRep::Row)
		{
			throw(math::ExceptionInvalidValue("Matrix<T>::operator[]: Incorrect matrix represrntation for index operator"));
		}
		if (index > rows_ - 1)
		{
			throw(math::ExceptionIndexOutOfBounds("Matrix<T>::operator[]: Index out of bounds"));
		}
		return &mvec_[index * cols_];
	}

	template <typename T>
	void Matrix<T>::fill(T val)
	{
		std::fill(mvec_.begin(), mvec_.end(), val);
	}

	template <typename T>
	void Matrix<T>::rfill(unsigned int seed)
	{
		std::srand(seed);
		std::generate(mvec_.begin(), mvec_.end(), []() {
			return (rand() % 100) / 100.0;
			});
	}
	template <typename T>
	void Matrix<T>::print(int /*prec*/)
	{
		// std::cout.setf(std::ios_base::left);
		for (size_t row = 0; row < rows_; ++row)
		{
			for (size_t col = 0; col < cols_; ++col)
			{
				std::cout << std::setw(10) << std::left << (*this)(row, col);
			}
			std::cout << std::endl;
		}
		// std::cout.unsetf(std::ios_base::left);
	}

	template <> inline
		void Matrix<double>::print(int prec)
	{
		// std::cout.setf(std::ios_base::left);
		for (size_t row = 0; row < rows_; ++row)
		{
			for (size_t col = 0; col < cols_; ++col)
			{
				std::cout.setf(std::ios_base::fixed | std::ios_base::left);
				std::cout << std::setw(prec + 5u) << std::setprecision(prec) << (*this)(row, col);
			}
			std::cout << std::endl;
		}
		// std::cout.unsetf(std::ios_base::left);
	}

	template <typename T>
	void Matrix<T>::print(std::string& img, int /*prec*/)
	{
		std::stringstream buffer;

		// std::cout.setf(std::ios_base::left);
		for (size_t row = 0; row < rows_; ++row)
		{
			for (size_t col = 0; col < cols_; ++col)
			{
				buffer << std::setw(10) << std::left << (*this)(row, col);
			}
			buffer << std::endl;
		}
		img = buffer.str();
	}

	template <typename T>
	Matrix<T> Matrix<T>::getTr() const
	{
		Matrix<T> M_T(this->cols_, this->rows_);
		//int pos = 0;
		size_t n = this->numel();

		//auto start = std::chrono::steady_clock::now();
		// #pragma omp parallel for shared(M_T, n) schedule(static)
		for (int pos = 0; pos < n; ++pos)
		{
			size_t row = 0;
			size_t col = 0;
			if (this->repr_ == math::MatRep::Row) // row repr
			{
				row = (size_t)std::floor(pos / this->cols_);
				col = pos - row * this->cols_;
			}
			else if (this->repr_ == math::MatRep::Column) // column repr
			{
				col = (size_t)std::floor(pos / this->rows_);
				row = pos - this->rows_ * col;
			}
			M_T(col, row) = this->mvec_.at(pos);
			
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		return M_T;
	}

	template <typename T>
	void Matrix<T>::tr()
	{
		Matrix<T> M_T(this->cols_, this->rows_);
		size_t n = this->numel();

		//auto start = std::chrono::steady_clock::now();
		// #pragma omp parallel for shared(M_T, n) schedule(static)
		for (int pos = 0; pos < n; ++pos)
		{
			size_t row = 0;
			size_t col = 0;
			if (this->repr_ == math::MatRep::Row) // row repr
			{
				row = (size_t)std::floor(pos / this->cols_);
				col = pos - row * this->cols_;
			}
			else if (this->repr_ == math::MatRep::Column) // column repr
			{
				col = (size_t)std::floor(pos / this->rows_);
				row = pos - this->rows_ * col;
			}
			M_T(col, row) = this->mvec_.at(pos);

		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

		this->rows_ = M_T.rows();
		this->cols_ = M_T.cols();
		this->mvec_ = M_T.vectorized();
	}

	template <typename T>
	auto Matrix<T>::pnorm(const int p)
	{
		T norm = static_cast<T>(0.0);
		size_t n = this->numel();

		// #pragma omp parallel for shared(n, p) reduction(+:norm)
		for (int pos = 0; pos < n; ++pos)
		{
			norm += std::pow(std::abs(this->mvec_.at(pos)), p);
		}
		return std::pow(norm, (1.0 / p));
	}
	template <typename T>
	bool operator==(const Matrix<T>& m1, Matrix<T> const& m2)
	{
		return (m1.rows_ == m2.rows_) &&
			(m1.cols_ == m2.cols_) &&
			(m1.mvec_ == m2.mvec_);
	}

	template <typename T>
	T Matrix<T>::maxElement()
	{
		return *std::max_element(mvec_.begin(), mvec_.end());
	}

	template<typename T>
	void Matrix<T>::decompLU(Matrix<T>& Matrix_L, Matrix<T>& Matrix_U) const
	{
		if (cols_ != rows_)
		{
			throw(math::ExceptionNonSquareMatrix("decompLU: matrix must be square!"));
		}
		if (Matrix_L.rows() != Matrix_L.cols() ||
			Matrix_L.rows() != this->rows())
		{
			throw(math::ExceptionInvalidValue("decompLU: Matrix L argument of incorrect size!"));
		}
		if (Matrix_U.rows() != Matrix_U.cols() ||
			Matrix_U.rows() != this->rows())
		{
			throw(math::ExceptionInvalidValue("decompLU: Matrix U argument of incorrect size!"));
		}

		//omp_set_num_threads(std::max(settings::CurrentSettings.numThreads, 1));

		//auto start = std::chrono::steady_clock::now();
		//#pragma omp parallel for shared(Matrix_L) schedule(static)
		//for (int i = 0; i < this->cols_; i++)
		//{
		//	Matrix_L(i, i) = static_cast<T>(1);
		//}
		//
		//size_t n = this->cols_ * this->rows_;
		//
		//#pragma omp parallel for shared(Matrix_L, Matrix_U, n) schedule(static)
		//for (int pos = 0; pos < n; ++pos)
		//{
		//	size_t row = 0;
		//	size_t col = 0;
		//	if (this->repr_ == math::MatRep::Row) // row repr
		//	{
		//		row = (size_t)std::floor(pos / this->cols_);
		//		col = pos - row * this->cols_;
		//	}
		//	else if (this->repr_ == math::MatRep::Column) // column repr
		//	{
		//		col = (size_t)std::floor(pos / this->rows_);
		//		row = pos - this->rows_ * col;
		//	}
		//
		//	if (row <= col)
		//	{
		//		T sum_u = 0.0;
		//		for (size_t k = 0; k < row; k++)
		//		{
		//			sum_u += Matrix_L(row, k) * Matrix_U(k, col);
		//		}
		//		Matrix_U(row, col) = this->mvec_[pos] - sum_u;
		//	}
		//	if (row > col)
		//	{
		//		T sum_l = 0;
		//		for (size_t k = 0; k < col; k++)
		//		{
		//			sum_l += Matrix_L(row, k) * Matrix_U(k, col);
		//		}
		//		if (isEqual(Matrix_U(col, col), 0.0))
		//		{
		//			throw(math::ExceptionInvalidValue("decompLU: Incorrect input matrix for this method, U(j,j) = 0"));
		//		}
		//		Matrix_L(row, col) = (this->mvec_[pos] - sum_l) / Matrix_U(col, col);
		//	} // if (i > j)
		//
		//}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;



		//auto start = std::chrono::steady_clock::now();
		for (size_t i = 0; i < cols_; i++)
		{
			Matrix_L(i, i) = static_cast<T>(1);
		}
		for (size_t i = 0; i < cols_; i++)
		{
			for (size_t j = 0; j < cols_; j++)
			{
				if (i <= j)
				{
					T sum_u = 0;
					for (size_t k = 0; k < i; k++)
					{
						sum_u += Matrix_L(i, k) * Matrix_U(k, j);
					}
					Matrix_U(i, j) = (*this)(i, j) - sum_u;
				}
				if (i > j)
				{
					T sum_l = 0;
					for (size_t k = 0; k < j; k++)
					{
						sum_l += Matrix_L(i, k) * Matrix_U(k, j);
					}
					if (isEqual(Matrix_U(j, j), 0.0))
					{
						throw(math::ExceptionInvalidValue("decompLU: Incorrect input matrix for this method, U(j,j) = 0"));
					}
					Matrix_L(i, j) = ((*this)(i, j) - sum_l) / Matrix_U(j, j);
				} // if (i > j)
			} // for (size_t j = 0; j < cols_; j++)
		} // for (size_t i = 0; i < cols_; i++)
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	} // Matrix<T>::decompLU

	template<typename T>
	Matrix<T> Matrix<T>::decompLU() const
	{
		if (cols_ != rows_)
		{
			throw(math::ExceptionNonSquareMatrix("decompLU: matrix must be square!"));
		}
		Matrix<T> LUE(rows_, cols_);
		for (size_t i = 0; i < rows_; i++)
		{
			LUE(i, i) = static_cast<T>(1);
		}
		for (size_t i = 0; i < cols_; i++)
		{
			for (size_t j = 0; j < cols_; j++)
			{
				if (i <= j)
				{
					T sum_u = 0;
					for (size_t k = 0; k < i; k++)
					{
						sum_u += LUE(i, k) * LUE(k, j);
					}
					LUE(i, j) = (*this)(i, j) - sum_u;
				}
				if (i > j)
				{
					T sum_l = 0;
					for (size_t k = 0; k < j; k++)
					{
						sum_l += LUE(i, k) * LUE(k, j);
					}
					LUE(i, j) = ((*this)(i, j) - sum_l) / LUE(j, j);
				} // if (i > j)
			} // for (size_t j = 0; j < cols_; j++)
		} // for (size_t i = 0; i < cols_; i++)
		return LUE;
	} // Matrix<T>::decompLU


	template <typename T>
	T Matrix<T>::det(unsigned int method) const
	{
		if (this->rows_ != this->cols_)
		{
			throw(math::ExceptionNonSquareMatrix("det: matrix must be square!"));
		}
		if (this->rows_ == 0 || this->cols_ == 0)
		{
			throw(math::ExceptionDegenerateMatrix("det: matrix dimensions is equal to 0!"));
		}
		if (this->rows_ == 1) // trivial case - single element
		{
			return mvec_.at(0);
		}
		if (this->rows_ == 2) // trivial case - 2x2 case
		{
			return (*this)(0, 0) * (*this)(1, 1) -
				(*this)(0, 1) * (*this)(1, 0);
		}
		// for sizes > 2
		if (method == 0) // cofactor algo
		{
			std::vector<size_t> rowsExcl{}, colsExcl{};
			return detIterative(0, rowsExcl, colsExcl);
		}
		if (method == 1) // LU algo
		{
			auto L = Matrix<T>(this->cols_);
			auto U = Matrix<T>(this->cols_);
			T mul_L = static_cast<T>(1.);
			T mul_U = static_cast<T>(1.);
			this->decompLU(L, U);
			for (size_t i = 0; i < this->cols_; ++i)
			{
				mul_L *= L(i, i);
				mul_U *= U(i, i);
			}
			return mul_L * mul_U;
		}
		return T();
	}

	template <typename T>
	T Matrix<T>::detIterative(unsigned int iteration,
		std::vector<size_t>& rowsExcl,
		std::vector<size_t>& colsExcl) const
	{
		if (rowsExcl.size() != colsExcl.size())
		{
			throw(math::ExceptionInvalidValue(""));
		}
		// calculate for determinant of current cofactor
		// define mask vectors
		if (rowsExcl.empty())
		{
			rowsExcl = std::vector<size_t>(this->rows_, 0);
			colsExcl = std::vector<size_t>(this->cols_, 0);
		}
		// define size of cofactor
		size_t numRows{ this->rows_ - iteration };

		if (numRows == 2)
		{
			auto row1Itr = std::find(rowsExcl.begin(), rowsExcl.end(), 0);
			auto row1 = row1Itr - rowsExcl.begin();
			auto row2 = std::find(row1Itr + 1, rowsExcl.end(), 0) - rowsExcl.begin();
			auto col1Itr = std::find(colsExcl.begin(), colsExcl.end(), 0);
			auto col1 = col1Itr - colsExcl.begin();
			auto col2 = std::find(col1Itr + 1, colsExcl.end(), 0) - colsExcl.begin();
			auto a11 = (*this)(row1, col1);
			auto a22 = (*this)(row2, col2);
			auto a21 = (*this)(row2, col1);
			auto a12 = (*this)(row1, col2);
			// auto detrm = ( (*this)(row1, col1) * (*this)(row2, col2) ) -
			//        ( (*this)(row2, col1) * (*this)(row1, col2) );
			auto detrm = a11 * a22 - a21 * a12;
			return detrm;
		}
		T dtrm{ 0. }; // determinant value
		size_t row{ 0 };
		// select row to make decomposition by
		auto rowItr = std::find(rowsExcl.begin(), rowsExcl.end(), 0);
		row = static_cast<size_t>(rowItr - rowsExcl.begin());
		rowsExcl.at(row) = 1;
		// make decomposition by NOT excluded columns
		size_t col{ 0 }, // column number in matrix
			colCofact{ 0 }; // column number in cofactor
		auto colItr = colsExcl.begin();
		while (colItr != colsExcl.end())
		{
			colItr = std::find(colItr, colsExcl.end(), 0);
			if (colItr == colsExcl.end())
				break;
			col = static_cast<size_t>(colItr - colsExcl.begin());
			size_t exp = 0 + colCofact; // by 1st row (always) and colCofact
			++colCofact;
			if (isEqual((*this)(row, col), 0.))
			{
				++colItr;
			}
			else
			{
				colsExcl.at(col) = 1;
				++iteration;
				/// @bug Inf calling?
				auto addDtrm = detIterative(iteration, rowsExcl, colsExcl);
				dtrm += addDtrm * (*this)(row, col) * std::pow(-1., exp);
				colsExcl.at(col) = 0;
				--iteration;
				++colItr;
			}
		} // while( colItrBegin!=colsExcl.cend() )
		rowsExcl.at(row) = 0;
		return dtrm;
	};

	template <typename T>
	Matrix<T> operator*(const Matrix<T>& M, T n)
	{
		Matrix<T> mul_M(M.rows(), M.cols());
		size_t el = M.numel();

		//auto start = std::chrono::steady_clock::now();
		// #pragma omp parallel for shared(mul_M, el) schedule(static)
		for (int pos = 0; pos < el; ++pos)
		{
			mul_M.mvec_.at(pos) = M.mvec_.at(pos) * n;
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		return mul_M;
	};

	template <typename T>
	Matrix<T>& Matrix<T>::operator*=(T n)
	{
		size_t el = this->numel();

		//auto start = std::chrono::steady_clock::now();
		// #pragma omp parallel for shared(el) schedule(static)
		for (int pos = 0; pos < el; ++pos)
		{
			this->mvec_.at(pos) *= n;
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		return *this;
	};

	template<typename T>
	Matrix<T> operator*(const Matrix<T>& A, const Matrix<T>& B)
	{
		if (A.cols() != B.rows())
		{
			throw(math::ExceptionInvalidValue("Matrix<T>::operator*: Matrices can't be multiplied!"));
		}

		Matrix<T> C(A.rows(), B.cols());

		//auto start = std::chrono::steady_clock::now();		
		// #pragma omp parallel for shared(A, B, C) schedule(static)
		for (int pos = 0; pos < C.numel(); ++pos)
		{
			// row representation for matrix C by default
			size_t row = (size_t)std::floor(pos / C.cols_);
			size_t col = pos - row * C.cols_;

			for (int k = 0; k < A.cols_; ++k)
			{
				C.mvec_[pos] += A(row, k) * B(k, col);
			}
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

		return C;
	};

	template <typename T>
	Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& M1)
	{
		Matrix<T> Mm{ operator*((*this),M1) };
		(*this) = Mm;
		return *this;
	}

	template<typename T>
	Matrix<T> operator+(const Matrix<T>& M, T n)
	{
		Matrix<T> sum_M(M.rows(), M.cols());
		size_t el = sum_M.numel();

		//auto start = std::chrono::steady_clock::now();		
		// #pragma omp parallel for shared(sum_M, M, n, el) schedule(static)
		for (int i = 0; i < el; ++i)
		{
			sum_M.mvec_.at(i) = M.mvec_.at(i) + n;
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		return sum_M;
	};

	template<typename T>
	Matrix<T> operator+(const Matrix<T>& A, const Matrix<T>& B)
	{
		if (A.cols() != B.cols() ||
			A.rows() != B.rows())
		{
			throw(math::ExceptionInvalidValue("Matrix<T>::operator+: Matrices can't be added!"));
		}

		Matrix<T> C(A.rows(), A.cols());
		size_t el = C.numel();

		//auto start = std::chrono::steady_clock::now();
		// #pragma omp parallel for shared(A, B, C, el) schedule(static)
		for (int i = 0; i < el; ++i)
		{
			C.mvec_.at(i) = A.mvec_.at(i) + B.mvec_.at(i);
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

		return C;
	};

	template <typename T>
	Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& M1)
	{
		Matrix<T> Mm{ operator+((*this),M1) };
		(*this) = Mm;
		return *this;
	}



	template<typename T>
	Matrix<T> operator-(const Matrix<T>& M, T n)
	{
		Matrix<T> diff_M(M.rows(), M.cols());
		size_t el = diff_M.numel();

		//auto start = std::chrono::steady_clock::now();		
		#pragma omp parallel for shared(diff_M, M, n, el) schedule(static)
		for (int i = 0; i < el; ++i)
		{
			diff_M.mvec_.at(i) = M.mvec_.at(i) - n;
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		return diff_M;
	};

	template<typename T>
	Matrix<T> operator-(const Matrix<T>& A, const Matrix<T>& B)
	{
		if (A.cols() != B.cols() ||
			A.rows() != B.rows())
		{
			throw(math::ExceptionInvalidValue("Matrix<T>::operator-: Matrices can't be subtracted!"));
		}

		Matrix<T> C(A.rows(), A.cols());
		size_t el = C.numel();

		//auto start = std::chrono::steady_clock::now();
		// #pragma omp parallel for shared(A, B, C, el) schedule(static)
		for (int i = 0; i < el; ++i)
		{
			C.mvec_.at(i) = A.mvec_.at(i) - B.mvec_.at(i);
		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		return C;
	};

	template <typename T>
	Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& M1)
	{
		Matrix<T> Mm{ operator-((*this),M1) };
		(*this) = Mm;
		return *this;
	}

	template<typename T>
	Matrix<T> Matrix<T>::inverse()
	{
		if (this->rows_ != this->cols_)
		{
			throw(math::ExceptionNonSquareMatrix("inverse:Inverse of non square matrix!"));
		}
		Matrix<T> X(this->rows(), this->cols()), // inverse matrix
			L(this->rows(), this->cols()),  // L matrix
			U(this->rows(), this->cols());  // U matrix
		this->decompLU(L, U);
		long long n = static_cast<long long>(this->rows_ - 1); // size of matrix
		long long d = n; // from right lower corner
		long long i = d;
		long long j = d;
		while (true) {
			i = d;
			j = d;
			// diagonal element of X calculation
			X(d, d) = static_cast<T>(1.);
			for (long long k = j + 1; k <= n; ++k)
			{
				// formula (2.15) p 71
				X(j, j) -= U(j, k) * X(k, j);
			}
			X(j, j) *= static_cast<T>(1.) / U(j, j);
			// column walk up from jj
			for (i = d - 1; i >= 0; --i)
			{
				// formula (2.16) p 71
				X(i, j) = static_cast<T>(0.);
				for (long long k = i + 1; k <= n; ++k)
				{
					X(i, j) += U(i, k) * X(k, j);
				}
				X(i, j) *= -static_cast<T>(1.) / U(i, i);
			}
			i = d; // return to diagonal
			// row walk left from jj
			for (j = d - 1; j >= 0; --j)
			{
				// formula (2.17) p 71
				X(i, j) = static_cast<T>(0.);
				for (long long k = j + 1; k <= n; ++k)
				{
					X(i, j) -= X(i, k) * L(k, j);
				}
			}
			j = d; // return to diagonal
			--d;
			if (d < 0)
				break;  // exit cycle
		} // while (!end)
		return X;
	} //Matrix<T> Matrix<T>::inverse()

	template<typename T>
	bool Matrix<T>::compare(const Matrix<T>& M, T eps)
	{
		if (this->rows_ != M.rows_ ||
			this->cols_ != M.cols_)
		{
			return false;
		}

		for (size_t i = 0; i < this->rows_; ++i)
		{
			for (size_t j = 0; j < this->cols_; ++j)
			{
				if (!isEqual((*this)(i, j), M(i, j)))
					return false;
			}
		}
		return true;
	}

} // namespace math
