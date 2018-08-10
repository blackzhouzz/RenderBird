#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	class SampleUtils
	{
	public:
		static Vector2f ToUnitDisk(const Vector2f &u);
		static void CosHemisphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf);
		static void UniformHemisphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf);
		static void UniformSphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf);
		static void UniformCone(const Vector3f& normal, const Vector2f& rand2d, Float angle, Vector3f* wi, Float* pdf);
		static Float PowerHeuristic(Float pdf1, Float pdf2);
		static Float BalanceHeuristic(Float pdf1, Float pdf2);
	};
}