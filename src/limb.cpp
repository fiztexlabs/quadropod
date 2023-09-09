#include <limb.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <libmath/matrix.h>

using namespace robo;

void Limb::calcServoPos(const math::Matrix<real> &coord)
{
    math::Matrix<real> target_coords_2d_ =
        {
            {std::sqrt(std::pow(coord(0, 0), 2.0) + std::pow(coord(1, 0), 2.0))},
            {coord(2, 0)}
        };

    servo_target_pos_(0, 0) =
        std::acos(coord(0, 0) / std::sqrt(std::pow(coord(0, 0), 2.0) + std::pow(coord(0, 1), 2.0))) * (180.0/M_PI);

    real L03 = std::sqrt(
        std::pow(target_coords_2d_(0, 0), 2.0) + std::pow(target_coords_2d_(0, 1), 2.0)
    );

    real L13 =
        std::sqrt(
            std::pow(target_coords_2d_(0, 0) - L_(0, 0), 2.0) +
            std::pow(target_coords_2d_(0, 1), 2.0));

    real f03 = std::acos(
        (L03 * L03 - L_(0, 0) * L_(0, 0) - L13 * L13) / (-2.0 * L_(0, 0) * L13)
    ) * (180.0/M_PI);

    real f13 = std::acos(
        (L_(2, 0) * L_(2, 0) - L_(1, 0) * L_(1, 0) - L13 * L13) / (-2.0 * L_(1, 0) * L13)
    ) * (180.0/M_PI);

    servo_target_pos_(1, 0) = 360.0 - f03 - f13;

    servo_target_pos_(2, 0) = std::acos(
        (L13 * L13 - L_(1, 0) * L_(1, 0) - L_(2, 0) * L_(2, 0)) / (-2.0 * L_(1, 0) * L_(2, 0))
    ) * (180.0/M_PI);
}