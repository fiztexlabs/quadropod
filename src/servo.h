#pragma once
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>
#include <cmath>
#include <defines.h>

namespace robo
{
    /**
    * @brief Control servo with PCA9685
    * @details Class allows to smooth move of servos
    * @author Ilya Konovalov
    */
    class Servo
    {
        private:
            Adafruit_PWMServoDriver* pwm_ = nullptr;

            /// @brief PWM bit depth
            int bit_depth_ = 12;

            /// @brief PWM frequency
            int pwm_freq_;

            /// @brief Low servo impulse width (ms)
            real low_imp_;

            /// @brief Hight servo impulse width (ms)
            real hight_imp_;

            /// @brief PWM gradations
            int pwm_grad_ = std::pow(2,bit_depth_);

            /// @brief Digital value for low servo impulse width
            int servo_low_;

            /// @brief Digital value for hiht servo impulse width
            int servo_hight_;

            /// @brief Maximum servo angle
            real max_angle_;

            /// @brief Servo pin
            int servo_pin_ = 0;

            /// @brief Accelaration [deg/s2]
            real acceleration_ = 200.0;

            /// @brief Current speed [deg/s]
            real speed_ = 0.0;

            /// @brief Maximum speed [deg/s]
            real max_speed_ = 425.0;

            /// @brief Current time pos 
            unsigned long t_c_ = 0.0;

            /// @brief Last time pos 
            unsigned long t_l_ = 0.0;

            /// @brief Target servo position
            real target_pos_ = 0.0;

            /// @brief Current servo position
            real current_pos_ = 0.0;

        public:
            /**
             * @brief Construct a new Servo object
             * 
             * @param servo_pin: PCA9685 pin, to be atach with servo
             * @param pwm: PWM object
             */
            Servo(
                const int servo_pin,
                Adafruit_PWMServoDriver* pwm
            ):
            Servo()
            {
                pwm_ = pwm;
                servo_pin_ = servo_pin;
            }

            /**
             * @brief Construct a new Servo object
             * 
             * @param max_angle: Maximum servo angle [deg]
             * @param pwm_freq: Frequency of servo PWM [Hz]
             * @param low_imp: Impulse width, correspond to low servo position [ms]
             * @param hight_imp: Impulse width, correspond to hight servo position [ms]
             */
            Servo(
                const real max_angle = 180,
                const int pwm_freq = 50,
                const real low_imp = 0.5,
                const real hight_imp = 2.4
            ):
            max_angle_(max_angle),
            pwm_freq_(pwm_freq),
            low_imp_(low_imp),
            hight_imp_(hight_imp),
            servo_low_(pwm_grad_*(low_imp_/(1000/pwm_freq_))),
            servo_hight_(pwm_grad_*(hight_imp_/(1000/pwm_freq_)))
            {
            };

            ~Servo()
            {
            }

            /// @brief Function start pwm and wire objets
            void begin();

            /// @brief Set target position of servo in [deg]
            void setTargetPosition(const real target_pos);

            /**
             * @brief Move servo to target position at each controller's loop iteration
             * 
             * @return true if current position is equal to the target position
             * @return false if current position isn't equal to target position
             */
            bool move();
    };
}