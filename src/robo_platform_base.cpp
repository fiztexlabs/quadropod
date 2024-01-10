#include <robo_platform_base.h>
#include <cmath>
#include <algorithm>

using namespace robo;

void robo::RoboPlatform_base::buildMovementMatrix(const std::string &ax, const real angle, math::Matrix<real>& M_oA, const math::Matrix<real> &mov)
{
    if (ax == "OX")
    {
        M_oA =
            {
                {static_cast<real>(1.0), static_cast<real>(0.0), static_cast<real>(0.0), mov(0, 0)},
                {static_cast<real>(0.0), std::cos(angle), -std::sin(angle), mov(1, 0)},
                {static_cast<real>(0.0), std::sin(angle), std::cos(angle), mov(2, 0)},
                {static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)}};
        return void();
    }
    if (ax == "OY")
    {
        M_oA =
            {
                {std::cos(angle), static_cast<real>(0.0), std::sin(angle), mov(0, 0)},
                {static_cast<real>(0.0), static_cast<real>(1.0), static_cast<real>(0.0), mov(1, 0)},
                {-std::sin(angle), static_cast<real>(0.0), std::cos(angle), mov(2, 0)},
                {static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)}};
        return void();
    }
    if (ax == "OZ")
    {
        M_oA =
            {
                {std::cos(angle), -std::sin(angle), static_cast<real>(0.0), mov(0, 0)},
                {std::sin(angle), std::cos(angle), static_cast<real>(0.0), mov(1, 0)},
                {static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0), mov(2, 0)},
                {static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(0.0), static_cast<real>(1.0)}};
        return void();
    }
}
