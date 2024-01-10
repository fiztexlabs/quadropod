#pragma once
#include <robo_platform.h>
#include <Arduino.h>
#include <libmath/matrix.h>
#include <defines.h>
#include <string>

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

            /// @brief Build movement matrix (<a href="https://ru.wikipedia.org/wiki/%D0%9C%D0%B0%D1%82%D1%80%D0%B8%D1%86%D0%B0_%D0%BF%D0%B5%D1%80%D0%B5%D1%85%D0%BE%D0%B4%D0%B0">Transformation matrix)
            /// @param ax: Axis for which vector will be turned
            /// @param angle: Rotation angle [rad]
            /// @param mov: Vector of movements (dx, dy and dz) [m]
            /// @param M_oA[out]: Rotation matrix for "ax" axis
            void buildMovementMatrix(
                const std::string &ax,
                const real angle,
                math::Matrix<real>& M_oA,
                const math::Matrix<real> &mov =
                {
                    {0.0},
                    {0.0},
                    {0.0}
                });

        public:
            virtual ~RoboPlatform_base() {};
    };

}