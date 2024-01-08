#pragma once

namespace math
{
	/// @brief Precision
// #define MATH_DOUBLE_PRECISION_DEFINE
#ifdef MATH_DOUBLE_PRECISION_DEFINE
	typedef double real;
#else
	typedef float real;
#endif
}

namespace math::settings
{
	/// @brief Global librsp settings
	struct Settings
	{
		/// @brief Target tolerance for numerical methods
		real targetTolerance = 1.e-3;

		/// @brief Number of threads for parallel executions
		/// @details If threads = 0 all available cores are used
		int numThreads = 4;
	};

	/// @brief Default properties
	inline Settings DefaultSettings;

	/// @brief Current global rsplib properties
	inline Settings CurrentSettings = DefaultSettings;

	/**
	* @brief Set target tolerance for numerical methods
	* @param tol: Target tolerance
	*/
	void setTargetTolerance(const real tol);

	/**
	* @brief Get target tolerance of numerical methods
	*/
	real getTargetTolerance();
}