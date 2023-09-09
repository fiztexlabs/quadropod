#pragma once
#include <libmath/matrix.h>
#include <vector>
#include <defines.h>
#include <string>
#include <robo_exception.h>
#include <cmath>

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

namespace robo
{
    class Limb
    {
    private:
        /// @brief Length of limb parts
        math::Matrix<real> L_;

        /// @brief Number of vertexes
        size_t vertex_num_;

        /// @brief Limbs counter
        static size_t cnt_;

        /// @brief Limb name
        std::string name_;

        /// @brief Target foot coordinates
        math::Matrix<real> target_coords_ = math::Matrix<real>(3, 1);

        /// @brief Current foot coordinates
        math::Matrix<real> current_coords_ = math::Matrix<real>(3, 1);

        /// @brief Target servo positions
        math::Matrix<real> servo_target_pos_ = math::Matrix<real>(3, 1);

        /// @brief Current servo positions
        math::Matrix<real> servo_current_pos_ = math::Matrix<real>(3, 1);

    public:
        /// @brief Default constructor
        Limb(){

        };

        /**
         * @brief Constructor for robot limb
         * @details Links number in limb is 3
         * @param L: Column vector of length of limb parts
         * @param name: Limb name. Default name is "Limb_[idx]""
         */
        Limb(
            const math::Matrix<real> &L,
            const std::string &name = "") : L_(L), vertex_num_(L.rows()), name_(name)
        {
            if (L_.rows() != 3)
            {
                throw(ExceptionInvalidValue("Limb: links amount must be 3!"));
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
        };

        /// @brief Destructor
        ~Limb(){};

        /// @brief Set target coordinates of the limb end (foot) and compute
        /// target servos angles
        void calcServoPos(const math::Matrix<real> &coords);
    };
}