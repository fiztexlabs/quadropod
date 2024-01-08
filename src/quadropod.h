#pragma once
#include <stepped_robot.h>
#include <limb.h>
#include <vector>
#include <libmath/matrix.h>
#include <Arduino.h>

namespace robo
{
    /**
     * @brief Quadruped robot class
     * @author Ilya Konovalov
    */
    class Quadropod :
        public SteppedRobot
    {
        private:

            /// @brief Number of legs
            size_t legs_num_ = 4;

            /// @brief Check inputs and raise exceptions
            void checkInputs(
                const std::vector<Limb*>& limbs,
                const math::Matrix<real>& vertices_base_coords,
                const math::Matrix<real>& platform_base_coords
            );
        public:
            /**
             * @brief Qaudruped robot constructor
             * @param limbs: Vector of robot limbs, for quadruped robots length must be equal to 4
             * @param vertices_base_coords: 4x3 Matrix, rows values represents x y ad z coordinates,
             * cols - robot legs. Coordinates defined relative to robot platfor median center
             * @param platform_base_coords: Initial coordinates of robot position: 4x6 Matrix,
             * rows represents coordinates x, y, z and angles p, f, a4 cols - represents legs
             * @param serial: Pointer to serial port, which robot will be use for output diagnostic 
             * information
            */
            Quadropod(
                const std::vector<Limb*>& limbs,
                const math::Matrix<real>& vertices_base_coords,
                const String& name = "",
                const math::Matrix<real>& platform_base_coords = 
                {
                    {0.0, 0.0, 0.0, 0.0},
                    {0.0, 0.0, 0.0, 0.0},
                    {0.0, 0.0, 0.0, 0.0},
                    {0.0, 0.0, 0.0, 0.0},
                    {0.0, 0.0, 0.0, 0.0},
                    {0.0, 0.0, 0.0, 0.0}
                },
                HardwareSerial* serial = &Serial
            )
            {
                checkInputs(
                    limbs,
                    vertices_base_coords,
                    platform_base_coords
                );

                limbs_ = limbs;
                vertices_base_coords_ = vertices_base_coords;
                vertices_curr_coords_ = vertices_base_coords_;
                name_ = name;
                platform_base_coords_ = platform_base_coords;
                serial_ = serial;

                vertices_diff_coords_ = math::Matrix<real>(legs_num_, 3);

                /// @brief Created robots counter
                static int cnt_;

                if (name_ == "")
                {
                    name_ = "Quadruped_robot_"+String(cnt_);
                }
                ++cnt_;

            };

            virtual void calcPositions(const math::Matrix<real>& movement, const math::Matrix<real>& angles) override;

            /// @brief Smooth motion
            virtual void move() override;
            
            /// @brief Get robot limb by idx
            /// @param idx: Index of the robot limb
            /// @return Pointer to the limb object
            Limb* getLeg(const size_t idx);
    };
}