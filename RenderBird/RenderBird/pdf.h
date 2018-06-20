#pragma once
#include "mathcore.h"
using namespace MathLib;

namespace RenderBird
{
	class PDF
	{
	public:
		static inline Float CosHemispherePDF(Float cosTheta) 
		{
			return cosTheta * C_INV_PI;
		}
	};
}