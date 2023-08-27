#pragma once

#include <libmath/matrix.h>
#include <libmath/math_settings.h>
#include <libmath/math_exception.h>
#include <libmath/boolean.h>
#include <vector>
#include <functional>
#include <chrono>


namespace math
{

	/**
	* @brief Partial derivate of function @f$ f @f$
	* @details Calculate @f$ \frac{\partial f}{\partial x} @f$.
	*
	* Example of using in C++:
	* @code
	* #include <libmath/differential.h>
	*
	* int main()
	* {
	*	// define function
	*	std::function<double(const math::Matrix<double>&)> f1(
	*		[](const  math::Matrix<double>& x)
	*		{
	*			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
	*		}
	*	);
	*	
	*	// scheme 1
	*	double df1dx1_sch1 = math::partialDerivate(
	*		f1,
	*		{
	*			{3.0},
	*			{2.0},
	*			{4.0}
	*		},
	*		0
	*	);
	* }
	* @endcode
	* @param scheme: Scheme of differentiation (1 by default)
	*	- 1: Scheme of first order, calculates as @f$ \frac{\partial f}{\partial x} = \frac{f(x_i) - f(x_{i-1})}{x_i-x_{i-1}} @f$
	*	- 2: Scheme of second order, calculates as
	* @f$ \frac{\partial f}{\partial x} = \frac{\frac{3}{2} f(x_{i+1} - 2 f(x_i) + \frac{1}{2} f(x_{i-1}}{x_i-x_{i-1}} @f$
	* @param stepX: Step of derivate calculation, @f$ \Delta x = x_i-x_{i-1} @f$ (0.1*math::settings::Settings.targetTolerance by default)
	* @param F: Function
	* @param x: Column-vector of aruments
	* @param xId: index of derivated variable
	* @return Partial derivate of f with x variable @f$ \frac{\partial f}{\partial x} @f$
	*/
	template<typename T, typename T1, class = std::enable_if<isNumeric<T>&& isNumeric<T1>>>
	T partialDerivate(const std::function<T(const Matrix<T1>&)>& F_, 
		const math::Matrix<T1>& x, 
		const size_t xId, 
		const int scheme = 1, 
		T1 stepX = static_cast<T1>(0.1*math::settings::CurrentSettings.targetTolerance)
	)
	{
		// check inputs
		if (x.cols() > 1)
		{
			throw(math::ExceptionIncorrectMatrix("partialDerivate: Matrix x argument must be column matrix!"));
		}
		if (xId >= x.rows())
		{
			throw(math::ExceptionIndexOutOfBounds("partialDerivate: Incorrect xId argument!"));
		}

		T dFdX = static_cast<T>(0.0);

		math::Matrix<T1> previous_x = x;
		previous_x(xId, 0) = x(xId, 0) - stepX;
		math::Matrix<T1> next_x = x;
		next_x(xId, 0) = x(xId, 0) + stepX;

		switch (scheme)
		{
		case 1:
			dFdX = (F_(x) - F_(previous_x)) / static_cast<T>(stepX);
			break;
		case 2:
			dFdX = ((3.0 / 2.0) * F_(next_x) - 2.0 * F_(x) + 0.5 * F_(previous_x)) / static_cast<T>(stepX);
			break;
		default:
			throw(math::ExceptionInvalidValue("partialDerivate: Incorrect scheme argument!"));
		}

		return dFdX;

	}

	/**
	* @brief Derivate of function @f$ f(x) @f$
	* @details Call partialDerivate and calculate full derivate as partial derivate of function with single argument x by x.
	*
	* Example of using in C++:
	* @code
	* #include <libmath/differential.h>
	* #include <iostream>
	* 
	* int main()
	* {
	*	// define function
	*	std::function<double(double)> f(
	*		[](const double x)
	*		{
	*			return (pow(x, 2.0) - 9.0);
	*		}
	*	);
	*	
	*	double dfdx = math::diff(f, 3.0);
	* }
	* @endcode
	* @param scheme: Scheme of differentiation (1 by default)
	*	- 1: Scheme of first order, calculates as @f$ \frac{df}{dx} = \frac{f(x_i) - f(x_{i-1})}{x_i-x_{i-1}} @f$
	*	- 2: Scheme of second order, calculates as
	* @f$ \frac{df}{dx} = \frac{\frac{3}{2} f(x_{i+1} - 2 f(x_i) + \frac{1}{2} f(x_{i-1}}{x_i-x_{i-1}} @f$.
	* @param stepX: Step of derivate calculation, @f$ \Delta x = x_i-x_{i-1} @f$ (0.1*math::settings::Settings.targetTolerance by default)
	* @param F: Function
	* @param x: Argument
	* @return Derivate of f with x argument, @f$ \frac{df}{dx} @f$
	*/
	template<typename T, typename T1, class = std::enable_if<isNumeric<T>&& isNumeric<T1>>>
	T diff(
		const std::function<T(const T1)>& F,
		const T1 x,
		const int scheme = 1,
		T1 stepX = static_cast<T1>(0.1*math::settings::CurrentSettings.targetTolerance)
	)
	{
		Matrix<T1> args(std::vector<T1>{ x }, 1);
		std::function<T(const Matrix<T1>&)> f
		(
			[F](const Matrix<T1>& args)
			{
				return F(args(0, 0));
			}
		);

		return math::partialDerivate<T, T1>(f, args, 0, scheme, stepX);
	}

	/**
	* @brief Jacobi matrix of vector function @f$ \mathbf{u} @f$ with arguments @f$ \mathbf{x} @f$
	* @details Calculate Matrix of size MxN, where M - number of functions F, N - number of functions arguments x.
	* @f$ \mathbf{J} =
	* \begin{pmatrix}
	*  \frac{\partial u_1}{\partial x_1} & \frac{\partial u_1}{\partial x_2} & \cdots & \frac{\partial u_1}{\partial x_n} \\
	*  \frac{\partial u_2}{\partial x_1} & \frac{\partial u_2}{\partial x_2} & \cdots & \frac{\partial u_2}{\partial x_n} \\
	*  \vdots  & \vdots  & \ddots & \vdots  \\
	*  \frac{\partial u_m}{\partial x_1} & \frac{\partial u_m}{\partial x_2} & \cdots & \frac{\partial u_m}{\partial x_n} \\
	* \end{pmatrix} @f$.
	*
	* Example of using in C++:
	* @code
	* #include <libmath/differential.h>
	* #include <libmath/matrix.h>
	* #include <iostream>
	* #include <omp.h>
	*
	* int main()
	* {
	*	omp_set_num_threads(4);
	*	
	*	// vector function F
	*	std::vector<std::function<double(const math::Matrix<double>&)>> F;
	*	
	*	F.push_back
	*	(
	*		[](const math::Matrix<double>& x)
	*		{
	*			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
	*		}
	*	);
	*	F.push_back
	*	(
	*		[](const math::Matrix<double>& x)
	*		{
	*			return (x(0, 0) + x(1, 0) * x(2, 0) - 2.0);
	*		}
	*	);
	*	F.push_back
	*	(
	*		[](const math::Matrix<double>& x)
	*		{
	*			return (x(0, 0) + x(1, 0) + x(2, 0) - 3.0);
	*		}
	*	);
	*	
	*	math::Matrix<double> x0 =
	*	{
	*		{1.0},
	*		{1.0},
	*		{1.0}
	*	};
	*	
	*	// define J matrix for results of jakobian
	*	math::Matrix<double> J(3);
	*	
	*	// calculate Jakobi matrix
	*	math::jacobi(F, x0, J);
	*
	* }
	* @endcode
	* @param[in] scheme: Scheme of differentiation (1 by default)
	*	- 1: Scheme of first order, calculates as @f$ \frac{\partial f}{\partial x} = \frac{f(x_i) - f(x_{i-1})}{x_i-x_{i-1}} @f$
	*	- 2: Scheme of second order, calculates as
	* @f$ \frac{\partial f}{\partial x} = \frac{\frac{3}{2} f(x_{i+1} - 2 f(x_i) + \frac{1}{2} f(x_{i-1}}{x_i-x_{i-1}} @f$
	* @param stepX: Step of derivate calculation, @f$ \Delta x = x_i-x_{i-1} @f$ (0.001*math::settings::Settings.targetTolerance by default)
	* @param[in] F: Vector of functions (F = vector function @f$ \mathbf{u} @f$)
	* @param[in] x: Column matrix of arguments of F, for which Jakobian calculates
	* @param[out] J: Jakobi's matrix of size MxN
	*/
	template<typename T, typename T1, class = std::enable_if<isNumeric<T>&& isNumeric<T1>>>
	void jacobi(
		const std::vector<std::function<T(const Matrix<T1>&)>>& F, 
		const math::Matrix<T1>& x, 
		math::Matrix<T>& J,
		const int scheme = 1,
		T1 stepX = static_cast<T1>(0.001 * math::settings::CurrentSettings.targetTolerance)
	)
	{
		// check inputs
		if (x.cols() > 1)
		{
			std::cerr << "jakobian: Matrix x argument must be column matrix!\n";
			//Exception exc(Exception::Type::IncorrectMatrixSize);
			//throw exc;
		}
		if (x.rows() != F.size())
		{
			std::cerr << "jakobian: dimensions of input argument F and output x didn't agree!\n";
			//Exception exc(Exception::Type::IncorrectMatrixSize);
			//throw exc;
		}

		math::Matrix<T1> previous_x = x;
		math::Matrix<T1> next_x = x;

		size_t m = F.size();
		size_t n = x.rows();

		// check inputs
		if ((J.rows() != m) || (J.cols() != n))
		{
			std::cerr << "jakobian: Matrix J argument of incorrect size!\n";
			//Exception exc(Exception::Type::JakobianArgumentIncorrectSize);
			//throw exc;
		}

		size_t n_els = J.numel();

		//auto start = std::chrono::steady_clock::now();
		#pragma omp parallel for shared(J, n_els) schedule(static)
		for (int pos = 0; pos < n_els; ++pos)
		{
			size_t row = 0;
			size_t col = 0;
			if (J.representation() == math::MatRep::Row) // row repr
			{
				row = (size_t)std::floor(pos / J.cols());
				col = pos - row * J.cols();
			}
			else if (J.representation() == math::MatRep::Column) // column repr
			{
				col = (size_t)std::floor(pos / J.rows());
				row = pos - J.rows() * col;
			}
			J(col, row) = math::partialDerivate<T, T1>(F[col], x, row, scheme, stepX);

		}
		//auto end = std::chrono::steady_clock::now();
		//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

	}
}