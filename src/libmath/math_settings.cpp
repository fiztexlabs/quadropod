#include <libmath/math_settings.h>
#include <libmath/math_exception.h>

void math::settings::setTargetTolerance(const math::real tol)
{
	if (tol <= 0.0)
	{
		throw(math::ExceptionInvalidValue("Target tolerance for numerical methods must be greater than 0.0"));
	}
	else
	{
		CurrentSettings.targetTolerance = tol;
	}
}

math::real math::settings::getTargetTolerance()
{
	return CurrentSettings.targetTolerance;
}
