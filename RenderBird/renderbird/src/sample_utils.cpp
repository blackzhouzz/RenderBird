#include "sample_utils.h"
#include "entitymanager.h"
#include "disk_component.h"

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
			return C_Zero_v2f;
		}
		else if (a*a > b*b)
		{
			r = a;
			phi = C_QUARTER_PI * (b / a);
		}
		else
		{
			r = b;
			phi = C_HALF_PI - C_QUARTER_PI * (a / b);
		}
		return Vector2f(r * cos(phi), r * sin(phi));
	}

	void SampleUtils::CosHemisphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf)
	{
		Vector2f newRand2d = ToUnitDisk(rand2d);
		Float costheta = std::sqrt(std::max((Float)1.0f - newRand2d.x * newRand2d.x - newRand2d.y * newRand2d.y, (Float)0.0f));
		//Matrix3f rot = MathUtils::MakeNormalTransform(normal);
		//*wi = rot * Vector3f(newRand2d.x, newRand2d.y, costheta);
		*wi = Vector3f(newRand2d.x, newRand2d.y, costheta);
		*pdf = costheta * C_1_INV_PI;
	}

	void SampleUtils::UniformHemisphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf)
	{
		Float z = rand2d.x;
		Float r = std::sqrt(std::max((Float)0.0f, (Float)1.0f - z * z));
		Float phi = C_2_PI * rand2d.y;
		Float x = r * std::cos(phi);
		Float y = r * std::sin(phi);

		//Matrix3f rot = MathUtils::MakeNormalTransform(normal);
		//*wi = rot * Vector3f(x, y, z);
		*pdf = 0.5f * C_1_INV_PI;
	}

	void SampleUtils::UniformSphere(const Vector3f& normal, const Vector2f& rand2d, Vector3f* wi, Float* pdf)
	{
		Float z = 1.0 - 2.0 * rand2d.x;
		Float r = std::sqrt(std::max(0.0, 1.0 - z * z));
		Float phi = C_2_PI * rand2d.y;
		Float x = r * std::cos(phi);
		Float y = r * std::sin(phi);

		*wi = Vector3f(x, y, z);
		*pdf = 0.25f * C_1_INV_PI;
	}

	void SampleUtils::UniformCone(const Vector3f& normal, const Vector2f& rand2d, Float angle, Vector3f* wi, Float* pdf)
	{
		Float z = std::cos(angle*rand2d.x);
		Float r = std::sqrt(std::max(0.0, 1.0 - z * z));
		Float phi = C_2_PI * rand2d.y;
		Float x = r * std::cos(phi);
		Float y = r * std::sin(phi);

		*wi = Vector3f(x, y, z);
		*pdf = 0.5f * C_1_INV_PI / (1.0f - std::cos(angle));
	}

	Float SampleUtils::PowerHeuristic(Float pdf1, Float pdf2)
	{
		const Float sqrPdf1 = pdf1 * pdf1;
		const Float sqrPdf2 = pdf2 * pdf2;
		return sqrPdf1 / (sqrPdf1 + sqrPdf2);
	}

	Float SampleUtils::BalanceHeuristic(Float pdf1, Float pdf2)
	{
		return (pdf1) / (pdf1 + pdf2);
	}
}