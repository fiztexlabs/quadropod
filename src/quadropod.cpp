#include <quadropod.h>

using namespace robo;

void robo::Quadropod::move(const math::Matrix<real> &coords, const math::Matrix<real> &angles)
{

}

Limb *robo::Quadropod::getLeg(size_t idx)
{
    return limbs_.at(idx);
}
