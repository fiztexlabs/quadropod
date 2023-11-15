#pragma once
#include <robo_platform.h>
#include <limb.h>
#include <vector>
#include <libmath/matrix.h>

namespace robo
{
    /**
     * @brief Quadruped robot class
     * @author Ilya Konovalov
    */
    class Quadropod :
        public RoboPlarform
    {
        private:
            /// @brief Robot limbs
            std::vector<Limb*> limbs_;

            /// @brief Base coordinates of platform vertices (default)
            math::Matrix<real> vertices_base_coords_;

            /// @brief Current coordinates of platform vertices
            math::Matrix<real> vertices_curr_coords_;
        public:
            Quadropod(
                const std::vector<Limb*>& limbs,
                const math::Matrix<real>& vertices_base_coords
            ) :
            limbs_(limbs),
            vertices_base_coords_(vertices_base_coords)
            {

            };

            void move(const math::Matrix<real>& coords, const math::Matrix<real>& angles) override;
            
            /// @brief Get robot limb by idx
            /// @param idx: Index of the robot limb
            /// @return Pointer to the limb object
            Limb* getLeg(size_t idx);
    };
}