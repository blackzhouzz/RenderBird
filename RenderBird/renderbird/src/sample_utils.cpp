#include "sample_utils.h"
#include "entitymanager.h"
#include "disk.h"

namespace RenderBird
{
	//concentric mapping see [Shirley and Chiu 97]
	Vector2f SampleUtils::ToUnitDisk(const Vector2f &rand2d)
	{
		Float phi, r;
		Float a = 2 * rand2d.x - 1;
		Float b = 2 * rand2d.y - 1;
		if (a == 0 && b == 0)
		{
			return Vector2f::ZERO;
		}
		else if (a*a > b*b) 
		{
			r = a;
			phi = (C_QUARTER_PI)*(b / a);
		}
		else 
		{
			r = b;
			phi = (C_QUARTER_PI)*(a / b) + C_HALF_PI;
		}
		return Vector2f(r*cos(phi), r*sin(phi));
	}

	void SampleUtils::CosHemisphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf)
	{
		Vector2f newRand2d = ToUnitDisk(rand2d);
		float costheta = sqrtf(std::max(1.0f - newRand2d.x * newRand2d.x - newRand2d.y * newRand2d.y, 0.0f));
		Matrix3f rot = MathUtils::MakeNormalTransform(normal);
		*wi = rot * Vector3f(newRand2d.x, newRand2d.y, costheta);
		*pdf = costheta * C_1_INV_PI;
	}

	void SampleUtils::UniformHemisphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf)
	{
		float z = rand2d.x;
		float r = sqrtf(std::max(0.0f, 1.0f - z * z));
		float phi = C_2_PI * rand2d.y;
		float x = r * cosf(phi);
		float y = r * sinf(phi);

		Matrix3f rot = MathUtils::MakeNormalTransform(normal);
		*wi = rot * Vector3f(x, y, z);
		*pdf = 0.5f * C_1_INV_PI;
	}

	void SampleUtils::UniformSphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf)
	{
		float z = 1.0f - 2.0f*rand2d.x;
		float r = sqrtf(fmaxf(0.0f, 1.0f - z * z));
		float phi = C_2_PI * rand2d.y;
		float x = r * cosf(phi);
		float y = r * sinf(phi);

		*wi = Vector3f(x, y, z);
		*pdf = 0.25f * C_1_INV_PI;
	}

	void SampleUtils::UniformCone(const Vector3f& normal, const Vector2f& rand2d, Float angle, Vector3f* wi, Float* pdf)
	{
		float z = cosf(angle*rand2d.x);
		float r = sqrtf(std::max(0.0f, 1.0f - z * z));
		float phi = C_2_PI * rand2d.y;
		float x = r * cosf(phi);
		float y = r * sinf(phi);

		*wi = Vector3f(x, y, z);
		*pdf = 0.5f * C_1_INV_PI / (1.0f - cosf(angle));
	}

	Float SampleUtils::PowerHeuristic(Float pdf1, Float pdf2)
	{
		return (pdf1 * pdf1) / (pdf1 * pdf1 + pdf2 * pdf2);
	}

	Float SampleUtils::BalanceHeuristic(Float pdf1, Float pdf2)
	{
		return (pdf1) / (pdf1 + pdf2);
	}
}