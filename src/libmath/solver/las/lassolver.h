#pragma once

#include <libmath/matrix.h>
#include <libmath/math_settings.h>
#include <libmath/boolean.h>
#include <string>

namespace math
{
	/**
	* @brief Types of stopping criteria.
	* - iterations: Stopping solver by target iterations
	* - tolerance: Stopping solver by target tolerance
	*/
	enum class LASStoppingCriteriaType
	{
		iterations,
		tolerance
	};

	/**
	* @brief Solver settings.
	*/
	struct LASsetup
	{
		/// @brief Stopping criteria
		LASStoppingCriteriaType criteria = LASStoppingCriteriaType::tolerance;

		/// @brief Maximum number of internal iterations for iterations stopping criteria
		size_t max_iter = 100;

		/// @brief Maximum iterations, befor method will halted. Need to avoid infinity iterations for
		/// tolerance criteria, if oscillations occurs.
		/// @details abort_iter will be ignored if iterations stopping criteria choosen
		size_t abort_iter = 10 * max_iter;

		/// @brief Target tolerance for numerical method for tolerance stopping criteria
		real targetTolerance = math::settings::DefaultSettings.targetTolerance;
	};

	/**
	* @brief Interfacial class for solving linear algebraic systems (LAS) of view 
	* of view @f$ \mathbf{A}\mathbf{x} = \mathbf{b} @f$.
	* @details Usage on the BicGStab example:
	* @code
	* #include <libmath/solver/las/bicgstab.h>
	* #include <libmath/matrix.h>
	* #include <omp.h>
	* 
	* int main()
	* {
	*	  // set threads number for parallelization
	*     omp_set_num_threads(1);
	*	
	*	  // set LAS dimension
	*     size_t dim = 10;
	* 
	*	  // generate matrix of LAS coefficients
	*     math::Matrix<double> A(dim);
	*     A.rfill(1);
	* 
	* 	  // generate column-vector of right-parts
	*     math::Matrix<double> b(dim, 1);
	*     b.rfill(2);
	* 
	* 	  // set initial guess to x column-vector
	*     math::Matrix<double> x(dim, 1);
	*     x.fill(0.0);
	* 
	* 	  // create solver with default setup
	*     math::BicGStab<double> bicgstab_solver;
	* 
	* 	  // solve system
	*     bicgstab_solver.solve(A, b, x);
	* 
	*     (A*x - b).print();
	* }
	* @endcode
	*/
	template <typename T, typename = typename std::enable_if<isNumeric<T>>::type>
	class LASsolver
	{
	protected:
		/// @brief Current solver settings
		LASsetup currentSetup_;

		/// @brief Method's name
		std::string method_ = "";

		/**
		* @brief Service function for checking input settings
		*/
		void checkInputs(const LASsetup& setup)
		{
			if (setup.criteria == LASStoppingCriteriaType::tolerance)
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

		/**
		* @brief Check input linear system
		*/
		void checkInputs(const Matrix<T>& A, const Matrix<T>& b, const Matrix<T>& x)
		{
			if (A.cols() != A.rows())
			{
				throw(math::ExceptionNonSquareMatrix(method_ + ": Inconsistent linear system. Matrix A argument must be square!"));
			}
			if (b.cols() > 1)
			{
				throw(ExceptionIncorrectMatrix(method_ + ": Matrix b argument must be column matrix!"));
			}
			if (b.rows() != A.rows())
			{
				throw(ExceptionIncorrectMatrix(method_ + ": dimensions of arguments A and b didn't agree!"));
			}
			if (x.cols() > 1)
			{
				throw(ExceptionIncorrectMatrix(method_ + ": Matrix x argument must be column matrix!"));
			}
			if (x.rows() != A.rows())
			{
				throw(ExceptionIncorrectMatrix(method_ + ": dimensions of input argument A and output x didn't agree!"));
			}
		}
	public:

		/**
		* @brief Solve LAS
		* @param A[in]: Coefficients matrix
		* @param b[in]: Column-vector of equations right-hands
		* @param x[out]: Column vector of solution. Initial value of x used
		* as initial guess for methods, that requires initial gues values
		*/
		virtual void solve(const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x) = 0;

		/**
		* @brief Set solver settings
		* @param setup: Solver settings
		*/
		void setupSolver(const LASsetup& setup)
		{
			checkInputs(setup);

			currentSetup_ = setup;
		};

		/**
		* @brief Get solver settings
		* @param setup[out]: Solver settings
		*/
		void getSolverSetup(LASsetup& setup) const
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