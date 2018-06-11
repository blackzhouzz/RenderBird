#pragma once
#include "LNVector.h"
#include "LNBoundingBox.h"
#include "LNSphere.h"

namespace Luna
{
	enum EPlaneSide
	{
		PS_Front=0,		//��ƽ��ǰ��
		PS_In,			//��ƽ����
		PS_Back,		//��ƽ�����
		PS_Cross,		//���ƽ��
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