#pragma once
#include <robo_platform.h>
#include <Arduino.h>
#include <libmath/matrix.h>
#include <defines.h>

namespace robo
{
    class RoboPlatform_base :
        public RoboPlarform
    {
        protected:
            /// @brief Robot name
            String name_ = "";

            /// @brief Base platform coordinates
            math::Matrix<real> platform_base_coords_;

            /// @brief Current platform coordinates
            math::Matrix<real> platform_curr_coords_;

            /// @brief Platform angles
            math::Matrix<real> platform_angles_ = math::Matrix<real>(3, 1);

            /// @brief Pointer to the serial object for output giagnostic information
            HardwareSerial* serial_ = nullptr;

            /// @brief Movement matrix with rotation around OX
            math::Matrix<real> M_ox_ = math::Matrix<real>(4);

            /// @brief Movement matrix with rotation around OY
            math::Matrix<real> M_oy_ = math::Matrix<real>(4);

            /// @brief Movement matrix with rotation around OZ
            math::Matrix<real> M_oz_ = math::Matrix<real>(4);

        public:
            virtual ~RoboPlatform_base() {};
    };

}