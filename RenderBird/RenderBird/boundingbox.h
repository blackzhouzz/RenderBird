#pragma once
#include "LNVector.h"
#include "LNMatrix.h"

namespace Luna
{
	class BoundingBox
	{
	public:
		BoundingBox() {}
		BoundingBox(const Vector3Float& minValue, const Vector3Float& maxValue);
		~BoundingBox() {}
		bool operator == (const BoundingBox& box)const;
		bool operator != (const BoundingBox& box)const;
		BoundingBox operator + (const BoundingBox& box)const;
		BoundingBox& operator += (const BoundingBox& box);
		BoundingBox operator + (const Vector3Float& vec)const;
		BoundingBox& operator += (const Vector3Float& vec);

	public:
		void ClampMin(const Vector3Float& vec);
		void ClampMax(const Vector3Float& vec);

		bool IsValid()const;
		void Set(const Vector3Float& minValue, const Vector3Float& maxOrExt, bool isExt=false);
		void Set(const BoundingBox& box);

		Vector3Float GetCenter()const;
		Vector3Float GetDiagonal()const;
		float GetDiagonalLength()const;

		float DistanceFrom(const Vector3Float& point)const;
		float DistanceFromSQ(const Vector3Float& point)const;

		bool IsContain(const Vector3Float& point)const;
		bool IsContain(const BoundingBox& box)const;
		Vector3Float ClosestPointTo(const Vector3Float& point) const;
		Vector3Float FurthestPointTo(const Vector3Float& point) const;
		float GetArea()const;
		float GetWidth()const;
		float GetHeight()const;
		float GetDepth()const;
		EAxis GetLongestAxis(float* length=NULL)const;
		EAxis GetShortestAxis(float* length=NULL)const;
		void DeformExtent(float size);

		void MakeInside(const BoundingBox& box);
		void MakeVecInside(Vector3Float& vec);
		void MoveDelta(const Vector3Float& vec);
		void MoveAbs(const Vector3Float& abs);
		BoundingBox TransformBy(const Matrix4f& mat)const;
	public:
		Vector3Float mMin;
		Vector3Float mMax;
		static const BoundingBox INVALID;
		static const BoundingBox ZERO;
		static const BoundingBox infinity;
	};
}