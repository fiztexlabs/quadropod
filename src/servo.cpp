#include <servo.h>
#include <Arduino.h>
#include <libmath/boolean.h>

using namespace robo;

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

void robo::Servo::begin()
{
    pwm_->setPWMFreq(pwm_freq_);
}

void robo::Servo::setZeroPosition(const real zero_pos)
{
    zero_pos_ = zero_pos;
}

void robo::Servo::move(const real target_pos)
{
    target_pos_ = target_pos;
}

bool robo::Servo::tick()
{
    t_c_ = micros();
    
    real dt = t_c_ - t_l_;
    
    // calculate current position
    real err = target_pos_ - current_pos_;
    if (abs(err) > 0.1) 
    {
      bool thisDir = (speed_ * speed_ / acceleration_ / 2.0 >= abs(err));  // пора тормозить
      speed_ += acceleration_ * dt * (thisDir ? -sign(speed_) : sign(err));
      speed_ = constrain(speed_, -max_speed_, speed_);
      current_pos_ += speed_ * dt * 1.e-6;
    }

    // move servo
    pwm_->setPWM(servo_pin_, 0, map(current_pos_,0,max_angle_,servo_low_,servo_hight_));
    // pwm_->setPWM(servo_pin_, 0, map(target_pos_,0,max_angle_,servo_low_,servo_hight_));

    t_l_ = t_c_;

    
    if (math::isEqual(err,0.0))
    {
        return true;
    }
    else
    {
        return false;
    }

    // return true;
}
