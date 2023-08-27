#pragma once

#include <libmath/solver/las/lassolver.h>
#include <libmath/math_settings.h>
#include <libmath/math_exception.h>
#include <libmath/boolean.h>
#include <vector>
#include <string>
#include <chrono>

namespace math
{
	/**
	* @brief Class for solving LAS with biconjugate gradient stabilized method
	*/
	template <typename T>
	class BicGStab :
		public LASsolver<T>
	{
	public:
		/// @brief Default constructor
		BicGStab() 
		{
			this->method_ = "BicGStab";
		};

		/**
		* @brief BicGStab solver constructor.
		* @param setup: Solver settings
		*/
		BicGStab(const LASsetup& setup)
		{
			this->method_ = "BicGStab";

			this->checkInputs(setup);

			this->currentSetup_ = setup;
		}

		/**
		* @brief LASsolver::solve
		*/
		virtual void solve(const Matrix<T>& A, const Matrix<T>& b, Matrix<T>& x) override
		{
			// check inputs
			this->checkInputs(A, b, x);

			Matrix<T> x_l = x;

			Matrix<T> r = b - A * x;
			Matrix<T> r1 = r;

			Matrix<T> p(b.rows(), 1);
			p.fill(static_cast<T>(0.0));

			Matrix<T> v(b.rows(), 1);
			v.fill(static_cast<T>(0.0));

			Matrix<T> h(b.rows(), 1);
			h.fill(static_cast<T>(0.0));

			Matrix<T> s(b.rows(), 1);

			Matrix<T> t(b.rows(), 1);

			T rho = static_cast<T>(1.0);
			T rho_l = static_cast<T>(1.0);
			T alpha = static_cast<T>(1.0);
			T omega = static_cast<T>(1.0);

			T betta = static_cast<T>(0.0);

			T tmp1 = static_cast<T>(0.0);
			T tmp2 = static_cast<T>(0.0);

			std::vector<T> tmp_E(b.rows(), static_cast<T>(1.0));
			T E = static_cast<T>(1.0);
			T e = static_cast<T>(math::settings::CurrentSettings.targetTolerance);

			size_t iter_cnt = 0;

			// stopping criteria
			bool stop = 0;

			//auto start = std::chrono::steady_clock::now();
			while (!stop)
			{
				rho_l = rho;
				rho = (r1.getTr() * r)(0, 0);
				betta = (rho / rho_l) * (alpha / omega);
				p = r + betta * (p - omega * v);
				v = A * p;
				alpha = rho / (r1.getTr() * v)(0, 0);
				h = x + alpha * p;
				s = r - alpha * v;
				t = A * s;
				omega = (t.getTr() * s)(0, 0) / (t.getTr() * t)(0, 0);
				x = h + omega * s;
				r = s - omega * t;

				++iter_cnt;

				if (this->currentSetup_.criteria == LASStoppingCriteriaType::tolerance)
				{
					E = (b - A * x).pnorm(2);
					if (E <= static_cast<T>(this->currentSetup_.targetTolerance))
					{
						stop = 1;
					}
					else
					{
						if (iter_cnt > this->currentSetup_.abort_iter)
						{
							throw(math::ExceptionTooManyIterations("BicGStab.solve: Solver didn't converge with choosen tolerance. Too many iterations!"));
						}
					}
				}
				if (this->currentSetup_.criteria == LASStoppingCriteriaType::iterations)
				{
					if (iter_cnt > this->currentSetup_.max_iter)
					{
						stop = 1;
					}
				}
			}
			//auto end = std::chrono::steady_clock::now();
			//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
		}
	};
}