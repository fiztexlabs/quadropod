#pragma once
#include <type_traits>
#include <cmath>
#include <limits>
#include <libmath/math_settings.h>

namespace math
{
	/**
	* @defgroup Boolean Boolean operations
	* @{
	* @brief Define boolean methods
	*/
	/**
	* @brief Comparsion of two numbers with different types
	* @details To compare A and B calculate diff = fabs(A-B). If diff is smaller than n% of max(abs(A),abs(B)) then A and B can be considered equal.
	* n defined as largest machine epsilon of types T and T1.
	* 
	* Example of using in C++:
	* @code
	* #include <includes/matlib.h>
	* #include <iostream>
	* 
	* int main()
	* {
	*     float A = 1.e5;
	*     double B = 1.e5;
	*     std::cout << math::isEqual(A, B) << std::endl;
	* }
	* @endcode
	* @param A: First number
	* @param B: Second number
	* @return flag: True if A is equal to B
	*/
	template<typename T, typename T1>
	bool isEqual(T A, T1 B)
	{
		typedef typename std::conditional<((std::is_floating_point<T>::value&&
			std::is_floating_point<T1>::value) ? sizeof(T) <= sizeof(T1) :
			std::is_floating_point<T1>::value),
			T1, T>::type RudeType;

		RudeType diff = std::abs(static_cast<RudeType>(A) - static_cast<RudeType>(B));

		//RudeType largest = (abs(B) > abs(A)) ? abs(B) : abs(A);

		if (diff <= static_cast<RudeType>(math::settings::CurrentSettings.targetTolerance))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/// @brief check T for numeric types
	template<typename T>
	constexpr bool isNumeric =
		std::is_same<T, int>::value ||
		std::is_same<T, int const>::value ||
		std::is_same<T, long int>::value ||
		std::is_same<T, long int const>::value ||
		std::is_same<T, unsigned int>::value ||
		std::is_same<T, unsigned int const>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, float const>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, double const>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, long double const>::value;

	/**
	* @}
	*/
}
