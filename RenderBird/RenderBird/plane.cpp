#include "LNCorePrivate.h"
#include "LNPlane.h"

namespace Luna
{
	const Plane Plane::ZERO = Plane(Vector3Float(0,0,0), 0);

	Plane::Plane(const Vector3Float& normal, float d) 
		: mNormal(normal)
		, mD(-d)
	{
	}

	Plane::Plane(Vector3Float a, Vector3Float b, Vector3Float c)
	{
		mNormal = Cross((b - a), (c - a)).SafeNormalize();
		mD = a.Dot(mNormal);
	}

	Plane::Plane(const Plane& plane)
	{
		*this = plane;
	}

	Plane& Plane::operator = (const Plane& plane)
	{
		mNormal = plane.mNormal;
		mD = plane.mD;
		return *this;
	}

	bool Plane::operator == (const Plane& plane)const
	{
		return mNormal == plane.mNormal && mD == plane.mD;
	}

	bool Plane::operator != (const Plane& plane)const
	{
		return mNormal != plane.mNormal || mD != plane.mD;
	}

	void Plane::Set(const Vector3Float& v1, const Vector3Float& v2, const Vector3Float& v3)
	{
		Vector3Float e1 = v2 - v1;
		Vector3Float e2 = v3 - v2;
		mNormal = Cross(e1, e2).Normalize();
		mD = -mNormal.Dot(v1);
	}

	void Plane::Set(const Vector3Float& normal, float d)
	{
		mNormal = normal;
		mD = -d;
	}

	void Plane::Normalize()
	{
		float len = mNormal.Length();
		if (len > FLOAT_EPSILON)
		{
			float invLen = 1.0f / len;
			mNormal *= invLen;
			mD *= invLen;
		}
	}

	float Plane::Distance(const Vector3Float& point)const
	{
		return mNormal.Dot(point) + mD;
	}

	EPlaneSide Plane::GetPlaneSide(const Vector3Float& point)const
	{
		float dis = Distance(point);
		if (fabs(dis) < SMALL_FLOAT_EPSILON)
		{
			return PS_In;
		}
		else if (dis > SMALL_FLOAT_EPSILON)
		{
			return PS_Front;
		}
		else 
		{
			return PS_Back;
		}
	}

	EPlaneSide Plane::GetPlaneSide(const BoundingBox& box)const
	{
		float minD, maxD;

		if (mNormal.x > 0.0f)
		{
			minD = mNormal.x*box.mMin.x;
			maxD = mNormal.x*box.mMax.x;
		}
		else
		{
			minD = mNormal.x*box.mMax.x; 
			maxD = mNormal.x*box.mMin.x;
		}

		if (mNormal.y > 0.0f)
		{
			minD += mNormal.y*box.mMin.y; 
			maxD += mNormal.y*box.mMax.y;
		}
		else
		{
			minD += mNormal.y*box.mMax.y; 
			maxD += mNormal.y*box.mMin.y;
		}

		if (mNormal.z > 0.0f)
		{
			minD += mNormal.z*box.mMin.z;
			maxD += mNormal.z*box.mMax.z;
		}
		else
		{
			minD += mNormal.z*box.mMax.z; 
			maxD += mNormal.z*box.mMin.z;
		}

		//box at front side of plane
		if (minD > -mD)
		{
			return PS_Front;
		}

		//box at back side of plane
		if (maxD < -mD)
		{
			return PS_Back;
		}

		//box cross plane
		return PS_Cross;
	}

	EPlaneSide Plane::GetPlaneSide(const Sphere& sphere)const
	{
		float d = Distance(sphere.mCenter);
		if (d > sphere.mRadius)
		{
			return PS_Front;
		}
		if (d < -sphere.mRadius)
		{
			return PS_Back;
		}
		return PS_Cross;
	}
}