#pragma once
#include <robo_platform_base.h>
#include <limb.h>
#include <libmath/matrix.h>
#include <vector>

namespace robo
{
    class SteppedRobot :
        public RoboPlatform_base
    {
        protected:
            /// @brief Robot limbs
            std::vector<Limb*> limbs_;

            /// @brief Base coordinates of platform vertices (default)
            math::Matrix<real> vertices_base_coords_;

            /// @brief Current coordinates of platform vertices
            math::Matrix<real> vertices_curr_coords_;

        public:
            virtual ~SteppedRobot() {};

            virtual bool begin();
    };
}