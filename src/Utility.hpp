#pragma once
#include <math.h>

namespace Util
{
	inline double ToDeg(double rad)
	{
		return ((rad*180.0f)/M_PI);
	}
}