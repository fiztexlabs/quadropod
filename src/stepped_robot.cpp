#include <stepped_robot.h>

bool robo::SteppedRobot::begin()
{
    for (const auto& limb : limbs_)
    {
        limb->begin();
    }

    return true;
}