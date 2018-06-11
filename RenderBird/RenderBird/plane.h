#pragma once
#include "LNVector.h"
#include "LNBoundingBox.h"
#include "LNSphere.h"

namespace Luna
{
	enum EPlaneSide
	{
		PS_Front=0,		//在平面前面
		PS_In,			//在平面中
		PS_Back,		//在平面后面
		PS_Cross,		//横跨平面
	};

	class Plane
	{
	public:
		Plane(void) {}
		Plane(const Vector3Float& normal, float d);
		Plane(const Plane& plane);
		Plane(Vector3Float a, Vector3Float b, Vector3Float c);
		~Plane(void) {}
		Plane& operator = (const Plane& plane);
		bool operator == (const Plane& plane)const;
		bool operator != (const Plane& plane)const;
		void Set(const Vector3Float& v1, const Vector3Float& v2, const Vector3Float& v3);
		void Set(const Vector3Float& normal, float d);
		void Normalize();
		float Distance(const Vector3Float& point)const;
		EPlaneSide GetPlaneSide(const Vector3Float& point)const;
		EPlaneSide GetPlaneSide(const BoundingBox& box)const;
		EPlaneSide GetPlaneSide(const Sphere& sphere)const;
	public:
		Vector3Float mNormal;
		float mD;
		static const Plane ZERO;
	};
}