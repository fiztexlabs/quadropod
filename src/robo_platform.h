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

            /// @brief Calc robot platform coordinates with target coordinates and angles
            /// @param mov: Movement coordinates, [m]
            /// @param angles: platform angles, [deg]
            virtual void calcPositions(const math::Matrix<real>& mov, const math::Matrix<real>& angles) = 0;

            /// @brief Move platform
            /// @details Used to smooth movement
            virtual void move() = 0;
    };
}