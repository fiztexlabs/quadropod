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

            /// @brief Rotation angle between global roboplatform coordinate system and
            /// local limb coordinate system
            std::vector<real> limb_local_coord_system_rot_;

            /// @brief Direction of local limb coordinate system axis relate to
            /// platform coordinate system (1 if axis has the same direction, -1 otherwise)
            math::Matrix<real> limb_local_axis_direction_;

            /// @brief Base coordinates of platform vertices (default)
            math::Matrix<real> vertices_base_coords_;

            /// @brief Current coordinates of platform vertices
            math::Matrix<real> vertices_curr_coords_;

            /// @brief Change of vertices coordinates
            math::Matrix<real> vertices_diff_coords_;

            /// @brief Limbs rotation matrices
            std::vector<math::Matrix<real>> M_rot_;

        public:
            virtual ~SteppedRobot() {};

            virtual bool begin();
    };
}