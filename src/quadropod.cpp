#include <quadropod.h>
#include <robo_exception.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <libmath/boolean.h>

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

void robo::Quadropod::calcPositions(const math::Matrix<real> &movement, const math::Matrix<real> &angles)
{
    math::Matrix<real> angles_r;
    if (platform_angles_.compare(angles))
    {
        angles_r = math::Matrix<real>(3, 1);
    }
    else
    {
        math::Matrix<real> diff_a = angles - platform_angles_;
        platform_angles_ = angles;

        // convert angles to radians
        angles_r = (M_PI / 180.0) * diff_a;
    }

    // compute movement matrices

    buildMovementMatrix("OX", angles_r(0, 0), M_ox_, movement);
    buildMovementMatrix("OY", angles_r(1, 0), M_oy_, movement);
    buildMovementMatrix("OZ", angles_r(2, 0), M_oz_, movement);

    // found new vertices coordinates

    // math::Matrix<real> vertices_new_coords = vertices_curr_coords_;
    // for(size_t i = 0; i < legs_num_; ++i)
    // {
    //     math::Matrix<real> v = 
    //     {
    //         {vertices_curr_coords_(0, i)},
    //         {vertices_curr_coords_(1, i)},
    //         {vertices_curr_coords_(2, i)},
    //         {static_cast<real>(1.0)}
    //     };

    //     math::Matrix<real> v1 = M_ox_ * M_oy_ * M_oz_ * v;
    //     vertices_new_coords(0, i) = v1(0, 0);
    //     vertices_new_coords(1, i) = v1(1, 0);
    //     vertices_new_coords(2, i) = v1(2, 0);
    //     // vertices_new_coords(3, i) = v1(3, 0);
    // }

    // vertices_diff_coords_ = vertices_new_coords - vertices_curr_coords_;
    // vertices_curr_coords_ = vertices_new_coords;

    // // calc new coordinates of limbs foots

    // math::Matrix<real> curr_foot_coords;
    // size_t j = 0;
    // for (auto &limb : limbs_)
    // {
    //     limb->getCurrentTargetCoords(curr_foot_coords);
    //     limb->calcServoPos(
    //         curr_foot_coords -
    //         math::Matrix<real>{
    //             {vertices_diff_coords_(0, j)},
    //             {vertices_diff_coords_(1, j)},
    //             {vertices_diff_coords_(2, j)}});
    //     ++j;
    // }

    math::Matrix<real> vertices_new_coords = vertices_curr_coords_;
    for(size_t i = 0; i < legs_num_; ++i)
    {
        math::Matrix<real> v = 
        {
            {vertices_curr_coords_(0, i)},
            {vertices_curr_coords_(1, i)},
            {vertices_curr_coords_(2, i)},
            {static_cast<real>(1.0)}
        };

        math::Matrix<real> v1 = M_ox_ * M_oy_ * M_oz_ * v;
        vertices_new_coords(0, i) = v1(0, 0);
        vertices_new_coords(1, i) = v1(1, 0);
        vertices_new_coords(2, i) = v1(2, 0);
        // vertices_new_coords(3, i) = v1(3, 0);

        // translate to limbs local coordinate system

        math::Matrix<real> v2 =
            {
                {vertices_new_coords(0, i) - vertices_curr_coords_(0, i)},
                {vertices_new_coords(1, i) - vertices_curr_coords_(1, i)},
                {vertices_new_coords(2, i) - vertices_curr_coords_(2, i)},
                {static_cast<real>(1.0)}};
        math::Matrix<real> v3 = M_rot_.at(i) * v2;
        vertices_diff_coords_(0, i) = v3(0, 0);
        vertices_diff_coords_(1, i) = v3(1, 0);
        vertices_diff_coords_(2, i) = v3(2, 0);
    }

    // vertices_diff_coords_ = vertices_new_coords - vertices_curr_coords_;
    vertices_curr_coords_ = vertices_new_coords;

    // calc new coordinates of limbs foots

    math::Matrix<real> curr_foot_coords;
    math::Matrix<real> new_foot_coords;
    // math::Matrix<real> M_rot;
    size_t j = 0;
    for (auto &limb : limbs_)
    {
        limb->getCurrentTargetCoords(curr_foot_coords);

        new_foot_coords = curr_foot_coords -
                          math::Matrix<real>{
                              {vertices_diff_coords_(0, j)},
                              {vertices_diff_coords_(1, j)},
                              {vertices_diff_coords_(2, j)}};
        // new_foot_coords = math::Matrix<real>{
        //     {new_foot_coords(0, 0)},
        //     {new_foot_coords(1, 0)},
        //     {new_foot_coords(2, 0)},
        //     {static_cast<real>(1.0)}};

        // rotate coords

        // buildMovementMatrix("OZ", limb_local_coord_system_rot_.at(j), M_rot);

        // new_foot_coords = M_rot_.at(j) * new_foot_coords;

        // reflect axis
        for (size_t i = 0; i < limb_local_axis_direction_.rows(); ++i)
        {
            if (!math::isEqual(new_foot_coords(i, 0), curr_foot_coords(i, 0)))
            {
                new_foot_coords(i, 0) = new_foot_coords(i, 0) * limb_local_axis_direction_(i, j);
            }
        }

        limb->calcServoPos(
            math::Matrix<real>{
                {new_foot_coords(0, 0)},
                {new_foot_coords(1, 0)},
                {new_foot_coords(2, 0)}});
        ++j;
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
