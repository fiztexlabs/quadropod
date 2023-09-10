#pragma once
#include <libmath/matrix.h>
#include <vector>
#include <defines.h>
#include <string>
#include <robo_exception.h>
#include <cmath>
#include <servo.h>
#include <Adafruit_PWMServoDriver.h>

namespace robo
{
    class Limb
    {
    private:
        Adafruit_PWMServoDriver* pwm_ = nullptr;

        /// @brief Length of limb parts
        math::Matrix<real> L_;

        /// @brief Number of vertexes
        size_t vertex_num_;

        /// @brief Limbs counter
        inline static size_t cnt_;

        /// @brief Limb name
        std::string name_;

        /// @brief Target servo positions [deg]
        math::Matrix<real> servo_target_pos_ = math::Matrix<real>(3, 1);

        /// @brief Zero servo positions [deg]
        math::Matrix<real> servo_zero_pos_ = math::Matrix<real>(3, 1);

        /// @brief Servos pins
        std::vector<int> servo_pins_;

        /// @brief Servos
        std::vector<Servo> servo_;

    public:
        /// @brief Default constructor
        Limb(){

        };

        /**
         * @brief Constructor for robot limb
         * @details Links number in limb is 3
         * @param L: Column vector of length of limb parts
         * @param servo_pins: Pins for limb servos on PCA9685
         * @param pwm: PWM object for PCA9685
         * @param name: Limb name. Default name is "Limb_[idx]""
         */
        Limb(
            const math::Matrix<real> &L,
            const std::vector<int> &servo_pins,
            Adafruit_PWMServoDriver* pwm, 
            const math::Matrix<real> &servo_zero_pos =
            {
                {0.0},
                {0.0},
                {0.0}
            },
            const std::string &name = "") : 
            L_(L), 
            servo_pins_(servo_pins),
            pwm_(pwm), 
            vertex_num_(L.rows()),
            servo_zero_pos_(servo_zero_pos),
            name_(name)
        {
            // check inputs
            if (L_.rows() != 3)
            {
                throw(ExceptionInvalidValue("Limb: links amount must be 3!"));
            }
            if (servo_pins_.size() != 3)
            {
                throw(ExceptionInvalidValue("Limb: servo pins amount must be 3!"));
            }
            if (L_.cols() != 1)
            {
                throw(ExceptionInvalidValue("Limb: L must be column vector!"));
            }

            if (name_ == "")
            {
                name_ = "Limb_" + std::to_string(cnt_);
            }
            ++cnt_;

            // construct servo objects
            for(const auto& pin : servo_pins_)
            {
                servo_.push_back(Servo(pin, pwm_));
            }


        };

        /// @brief Destructor
        ~Limb(){};

        void begin();

        /// @brief Set target coordinates of the limb end (foot) and compute
        /// target servos angles
        void calcServoPos(const math::Matrix<real> &coords);

        /**
         * @brief Move limb servos to target positions at each controller's loop iteration
         * 
         * @return true if current positions of all servos is equal to the target positions
         * @return false if current positions of all servos isn't equal to target positions
         */
        bool move();
    };
}