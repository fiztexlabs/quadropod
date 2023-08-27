#pragma once

#include <libmath/matrix.h>
#include <libmath/boolean.h>
#include <libmath/math_settings.h>
#include <libmath/solver/las/lassolver.h>
#include <libmath/solver/las/kholetsky.h>
#include <libmath/solver/las/bicgstab.h>
#include <functional>
#include <vector>

namespace math
{
	/**
	* @brief Types of stopping criteria.
	* - iterations: Stopping solver by target iterations
	* - tolerance: Stopping solver by target tolerance
	*/
	enum class USStoppingCriteriaType
	{
		iterations,
		tolerance
	};

	/**
	* @brief Solver settings.
	*/
	struct USsetup
	{
		~USsetup()
		{
			delete linearSolver;
		}
		/// @brief Stopping criteria
		USStoppingCriteriaType criteria = USStoppingCriteriaType::tolerance;

		/// @brief Maximum number of internal iterations
		size_t max_iter = 100;

		/// @brief Maximum iterations, befor method will halted. Need to avoid infinity iterations for
		/// tolerance criteria, if oscillations occurs.
		/// @details abort_iter will be ignored if iterations stopping criteria choosen
		size_t abort_iter = 10 * max_iter;

		/// @brief Target tolerance for numerical method
		real targetTolerance = math::settings::DefaultSettings.targetTolerance;

		/// @brief Differential step, @f$ \Delta x = x_i-x_{i-1} @f$, @f$ \Delta x = x_i-x_{i-1} @f$
		real diff_step = 0.001 * math::settings::CurrentSettings.targetTolerance;

		/// @brief Differential scheme
		/// @see math::partialDerivate
		int diff_scheme = 1;

		/// @brief Internal linear system solver
		/// @see LASsolver
		LASsolver<real>* linearSolver = new BicGStab<real>();

	};

	/**
	* @brief Base class for unlinear solvers
	* @details Example of usage Secant solver for solving system of unlinear
	* equations and single unlinear equation (see us.example.cpp)
    * @code
	* #include <libmath/solver/us/secant.h>
	* #include <libmath/matrix.h>
	* #include <iostream>
	* #include <omp.h>
	* 
	* int main()
	* {
	* 	// solve system of unlinear equations
	* 
	* 	omp_set_num_threads(4);
	* 
	* 	// vector function F
	* 	std::vector<std::function<double(const math::Matrix<double>&)>> F;
	* 
	* 	F.push_back
	* 	(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (pow(x(0, 0), 2.0) + pow(x(1, 0), 2.0) - x(2, 0) - 6.0);
	* 		}
	* 	);
	* 	F.push_back
	* 	(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (x(0, 0) + x(1, 0) * x(2, 0) - 2.0);
	* 		}
	* 	);
	* 	F.push_back
	* 	(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (x(0, 0) + x(1, 0) + x(2, 0) - 3.0);
	* 		}
	* 	);
	* 
	* 	// define initial values
	* 	math::Matrix<double> x =
	* 	{
	* 		{1.0},
	* 		{1.0},
	* 		{1.0}
	* 	};
	* 
	* 	// create Secant solver with default setup
	* 	math::Secant<double> secant_solver;
	* 
	* 	secant_solver.solve(F, x);
	* 
	* 	std::cout <<"Residual of function F[0] fot solving system of unlinear equations " << F[0](x) << std::endl;
	* 	std::cout <<"Residual of function F[1] fot solving system of unlinear equations " << F[1](x) << std::endl;
	* 	std::cout <<"Residual of function F[2] fot solving system of unlinear equations " << F[2](x) << std::endl;
	* 
	* 	// using US solver for solving single unlinear eqution
	* 
	* 	// single function f
	* 	std::function<double(const math::Matrix<double>&)> f(
	* 		[](const math::Matrix<double>& x)
	* 		{
	* 			return (pow(x(0, 0), 2.0) + pow(x(0, 0), 2.0) - x(0, 0) - 6.0);
	* 		}
	* 	);
	* 
	* 	// define x
	* 	math::Matrix<double> x = { {1.0} };
	* 
	* 	secant_solver.solve({ f }, x);
	* 
	* 	std::cout << "Residual of function f fot solving single unlinear equation " << f(x) << std::endl;
	* 
	* }
    * @endcode
	*/
	template <typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class UnlinearSolver
	{
	protected:
		/// @brief Current solver settings
		USsetup currentSetup_;

		/// @brief Method's name
		std::string method_ = "";

		/**
		* @brief Service function for checking input settings
		*/
		void checkInputs(const USsetup& setup)
		{
			if (setup.criteria == USStoppingCriteriaType::tolerance)
			{
				if (setup.targetTolerance < 0.0)
				{
					throw(math::ExceptionInvalidValue(method_ + ": Invalid target tolerance. Tolerance must be positive number!"));
				}
				if (setup.targetTolerance == 0.0)
				{
					throw(math::Exception(method_ + ": Invalid target tolerance. Tolerance must be greater than 0!"));
				}
			}
		};
	public:

		/**
		* @brief Find roots of system @f$ F(x) = 0 @f$
		* @details For solving system of unlinear equations F must be vector of functions with
		* size, greater, than 1. If size of vector F and matrix x equal to 1 - single equation 
		* @f$ F[0](x)=0 @f$ will be solved.
		* @param[in] F: Vector of functions, defines system of unlinear equations of type @f$ f(\mathbf{x}) = 0 @f$
		* @param[out] x: Column matrix of result roots. Initial value of x used as initial guess for numerical method
		* 
		*/
		virtual void solve(const std::vector<std::function<T(const Matrix<T>&)>>& F, Matrix<T>& x) = 0;

		/**
		* @brief Set solver settings
		* @param setup: Solver settings
		*/
		void setupSolver(const USsetup& setup)
		{
			checkInputs(setup);

			currentSetup_ = setup;
		};

		/**
		* @brief Get solver settings
		* @param setup[out]: Solver settings
		*/
		void getSolverSetup(USsetup& setup) const
		{
			setup = currentSetup_;
		};

		/**
		* @brief Get method name
		* @param mathod[out]: Solving method
		*/
		void getMethod(std::string& method) const
		{
			method = method_;
		}
	};
}