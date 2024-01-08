#include <quadropod.h>
#include <robo_exception.h>
#include <string>
#include <cmath>

using namespace robo;

void robo::Quadropod::checkInputs(
    const std::vector<Limb *> &limbs, 
    const math::Matrix<real> &vertices_base_coords, 
    const math::Matrix<real> &platform_base_coords
    )
{
    if (limbs.size() != 4)
    {
        if(serial_ != nullptr)
        {
            serial_->println(name_+": incorrect number of legs");
        }
        throw(ExceptionInvalidDimension(std::string(name_.c_str())+": incorrect number of legs"));
    }
    
    if (vertices_base_coords.cols() != 4 || vertices_base_coords.rows() != 3)
    {
        if(serial_ != nullptr)
        {
            serial_->println(name_+": incorrect dimension of vertices coordinates");
        }
        throw(ExceptionInvalidDimension(std::string(name_.c_str())+": incorrect dimension of vertices coordinates"));
    }
    
    if (platform_base_coords.cols() != 4 || platform_base_coords.rows() != 6)
    {
        if(serial_ != nullptr)
        {
            serial_->println(name_+": incorrect dimension of platform coordinates");
        }
        throw(ExceptionInvalidDimension(std::string(name_.c_str())+": incorrect dimension of platform coordinates"));
    }
}

void robo::Quadropod::calcPositions(const math::Matrix<real> &coords, const math::Matrix<real> &angles)
{
    // convert angles to radians
    math::Matrix<real> angles_r = angles;
    size_t i = 0;
    for (size_t i = 0; i < 3; ++i)
    {
        angles_r(i, 0) = angles(i, 0) * (PI / 180.0);
    }

    // compute movement matrices

    M_ox_ = 
        {
            {static_cast<real>(1.0),      static_cast<real>(0.0),      static_cast<real>(0.0),       platform_curr_coords_(0, 0) - coords(0, 0)},
            {static_cast<real>(0.0),      std::cos(angles_r(0, 0)),    -std::sin(angles_r(0, 0)),    platform_curr_coords_(0, 1) - coords(0, 1)},
            {static_cast<real>(0.0),      std::sin(angles_r(0, 0)),    std::cos(angles_r(0, 0)),     platform_curr_coords_(0, 2) - coords(0, 2)},
            {static_cast<real>(0.0),      static_cast<real>(0.0),      static_cast<real>(0.0),       static_cast<real>(1.0)                    }
        };

    M_oy_ = 
        {
            {std::cos(angles_r(1, 0)),    static_cast<real>(0.0),      std::sin(angles_r(1, 0)),    platform_curr_coords_(0, 0) - coords(0, 0)},
            {static_cast<real>(0.0),      static_cast<real>(1.0),      static_cast<real>(0.0),      platform_curr_coords_(0, 1) - coords(0, 1)},
            {-std::sin(angles_r(1, 0)),   static_cast<real>(0.0),      std::cos(angles_r(1, 0)),    platform_curr_coords_(0, 2) - coords(0, 2)},
            {static_cast<real>(0.0),      static_cast<real>(0.0),      static_cast<real>(0.0),      static_cast<real>(1.0)                    }
        };

    M_oz_ = 
        {
            {std::cos(angles_r(2, 0)),    -std::sin(angles_r(2, 0)),   static_cast<real>(0.0),      platform_curr_coords_(0, 0) - coords(0, 0)},
            {std::sin(angles_r(2, 0)),    std::cos(angles_r(2, 0)),    static_cast<real>(0.0),      platform_curr_coords_(0, 1) - coords(0, 1)},
            {static_cast<real>(0.0),      static_cast<real>(0.0),      static_cast<real>(1.0),      platform_curr_coords_(0, 2) - coords(0, 2)},
            {static_cast<real>(0.0),      static_cast<real>(0.0),      static_cast<real>(0.0),      static_cast<real>(1.0)                    }
        };
    
    // found new vertices coordinates

    math::Matrix<real> vertices_new_coords = vertices_curr_coords_;
    for(size_t i = 0; i < legs_num_; ++i)
    {
        math::Matrix<real> v = 
        {
            {vertices_curr_coords_(i, 0)},
            {vertices_curr_coords_(i, 1)},
            {vertices_curr_coords_(i, 2)},
            {static_cast<real>(1.0)}
        };

        math::Matrix<real> v1 = M_ox_*M_oy_*M_oz_*v;
        vertices_new_coords(i, 0) = v1(0, 0);
        vertices_new_coords(i, 1) = v1(0, 1);
        vertices_new_coords(i, 2) = v1(0, 2);
        vertices_new_coords(i, 3) = v1(0, 3);
    }

    vertices_diff_coords_ = vertices_new_coords - vertices_curr_coords_;
    vertices_curr_coords_ = vertices_new_coords;

    // calc new coordinates of limbs foots
    math::Matrix<real> curr_foot_coords;
    for (auto &limb : limbs_)
    {
        limb->getCurrentTargetCoords(curr_foot_coords);
        limb->calcServoPos(
            curr_foot_coords - vertices_diff_coords_);
    }
}

void robo::Quadropod::move()
{
    for (auto& limb : limbs_)
    {
        limb->move();
    }
}

Limb *robo::Quadropod::getLeg(const size_t idx)
{
    return limbs_.at(idx);
}
