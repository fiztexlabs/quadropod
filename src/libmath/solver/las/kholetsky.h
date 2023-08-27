#pragma once

#include <libmath/solver/las/lassolver.h>
#include <libmath/matrix.h>

namespace math
{
	/**
	* @brief Class for solving LAS with Kholetsky method (via LU-decomposition)
	*/
	template <typename T>
	class Kholetsky :
		public LASsolver<T>
	{
	public:
		Kholetsky()
		{
			this->method_ = "Kholetsky";
		}

		/// @brief LASsolver::solve
		virtual void solve(const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x) override
		{
			// check inputs
			checkInputs(A, b, x);

			// working arrays
			Matrix<T> LUE = A.decompLU();
			Matrix<T> Y(A.rows(), 1);
			// first run (eq 2.11 ����������, p 68)
			for (int i = 0; i < A.rows(); ++i)
			{
				Y(i, 0) = b(i, 0);
				for (int k = 0; k <= i - 1; ++k)
				{
					Y(i, 0) -= LUE(i, k) * Y(k, 0);
				}
			}
			// second run (eq 2.13 ����������, p 68)
			// size_t i{0};
			for (int j = static_cast<int>(A.rows() - 1); j >= 0; --j)
			{
				size_t i = static_cast<size_t>(j);
				x(i, 0) = Y(i, 0);
				for (size_t k = i + 1; k < A.rows(); ++k)
				{
					x(i, 0) = x(i, 0) - LUE(i, k) * x(k, 0);
				}
				x(i, 0) = x(i, 0) / LUE(i, i);
			}
		}
	};
}