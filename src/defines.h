#pragma once

// #define ROBO_DOUBLE_PREC

namespace robo
{
    /// @brief Precision
#define ROBO_DOUBLE_PREC
#ifdef ROBO_DOUBLE_PREC
    typedef double real;
#else
    typedef float real;
#endif
}