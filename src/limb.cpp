#include <limb.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <libmath/matrix.h>

using namespace robo;

void Limb::calcServoPos(const math::Matrix<real> &coord)
{
    // calc target servos angles

    math::Matrix<real> target_coords_2d_ =
        {
            {std::sqrt(std::pow(coord(0, 0), static_cast<real>(2.0)) + std::pow(coord(1, 0), static_cast<real>(2.0)))},
            {coord(2, 0)}
        };

    servo_target_pos_(0, 0) =
        std::acos(std::max(static_cast<real>(-1.0), std::min(static_cast<real>(1.0), coord(0, 0) / std::sqrt(std::pow(coord(0, 0), static_cast<real>(2.0)) + std::pow(coord(1, 0), static_cast<real>(2.0)))))) * (180.0/M_PI);

    real L03 = std::sqrt(
        std::pow(target_coords_2d_(0, 0), static_cast<real>(2.0)) + std::pow(target_coords_2d_(1, 0), static_cast<real>(2.0))
    );

    real L13 =
        std::sqrt(
            std::pow(target_coords_2d_(0, 0) - L_(0, 0), static_cast<real>(2.0)) +
            std::pow(target_coords_2d_(1, 0), static_cast<real>(2.0)));

    real f03 = std::acos(
        std::max(static_cast<real>(-1.0), std::min(static_cast<real>(1.0), (L03 * L03 - L_(0, 0) * L_(0, 0) - L13 * L13) / (static_cast<real>(-2.0) * L_(0, 0) * L13)))
    ) * (180.0/M_PI);

    real f13 = std::acos(
        std::max(static_cast<real>(-1.0), std::min(static_cast<real>(1.0), (L_(2, 0) * L_(2, 0) - L_(1, 0) * L_(1, 0) - L13 * L13) / (static_cast<real>(-2.0) * L_(1, 0) * L13)))
    ) * (180.0/M_PI);

    // invert, if Z > 0
    servo_target_pos_(1, 0) = coord(2,0) > static_cast<real>(0.0) ? f03 - f13 : 360.0 - f03 - f13;

    servo_target_pos_(2, 0) = std::acos(
        std::max(static_cast<real>(-1.0), std::min(static_cast<real>(1.0), (L13 * L13 - L_(1, 0) * L_(1, 0) - L_(2, 0) * L_(2, 0)) / (static_cast<real>(-2.0) * L_(1, 0) * L_(2, 0))))
    ) * (180.0/M_PI);

    // re-calc positions with zero servo positions

    for (size_t i = 0; i<servo_target_pos_.rows(); ++i)
    {
        servo_target_pos_(i,0) = std::abs(servo_zero_pos_(i,0) - servo_target_pos_(i,0));
    }

    // set target servos angles

    auto pos = servo_target_pos_.vectorized().cbegin();
    for(auto& servo : servo_)
    {
        servo.setTargetPosition(*pos);
        ++pos;
    }
    
    // Serial.println(String(servo_zero_pos_(1,0)));
    // Serial.println("");
}

Servo *robo::Limb::getServo(const size_t idx)
{
    return &servo_.at(idx);
}

void robo::Limb::begin()
{
    for(auto& servo : servo_)
    {
        servo.begin();
    }
}

bool robo::Limb::move()
{
    bool success = true;

    for(auto& servo : servo_)
    {
        success = std::min(success, servo.move());
    }

    return success;
}
