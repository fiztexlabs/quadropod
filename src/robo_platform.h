#pragma once
#include <libmath/matrix.h>
#include <defines.h>

namespace robo
{
    /**
     * @brief Interface class for robot movement algorithms
     * @author Ilya Konovalov
    */
    class RoboPlarform
    {
        public:
            virtual ~RoboPlarform() {};

            /// @brief Move robot platform with target coordinates and angles
            /// @param coords: Movement coordinates, [m]
            /// @param angles: platform angles, [deg]
            virtual void move(const math::Matrix<real>& coords, const math::Matrix<real>& angles) = 0;
    };
}