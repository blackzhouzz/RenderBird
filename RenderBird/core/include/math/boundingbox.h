#pragma once
#include "vector.h"

namespace Core
{
	class BoundingBox
	{
	public:
		BoundingBox() = default;
		BoundingBox(const Vector3f& minValue, const Vector3f& maxValue);
		bool operator == (const BoundingBox& box)const;
		bool operator != (const BoundingBox& box)const;
		BoundingBox operator + (const BoundingBox& box)const;
		BoundingBox& operator += (const BoundingBox& box);
		BoundingBox operator + (const Vector3f& vec)const;
		BoundingBox& operator += (const Vector3f& vec);
	public:
		bool IsValid()const;
		void Set(const Vector3f& minValue, const Vector3f& maxOrExt, bool isExt = false);
		Vector3f GetCenter()const;
		Vector3f GetDiagonal()const;
		Float GetDiagonalLength()const;
		Float DistanceFrom(const Vector3f& point)const;
		Float DistanceFromSQ(const Vector3f& point)const;
		bool IsContain(const Vector3f& point)const;
		bool IsContain(const BoundingBox& box)const;
		Vector3f ClosestPointTo(const Vector3f& point) const;
		Vector3f FurthestPointTo(const Vector3f& point) const;
		Float GetArea()const;
		Float GetWidth()const;
		Float GetHeight()const;
		Float GetDepth()const;
		EAxis GetLongestAxis(Float* length = nullptr)const;
		EAxis GetShortestAxis(Float* length = nullptr)const;
		void ExpandSize(Float size);
		void MakeInside(const BoundingBox& box);
		void MoveDelta(const Vector3f& vec);
		void MoveAbs(const Vector3f& abs);
	public:
		Vector3f min;
		Vector3f max;
		static const BoundingBox INVALID;
		static const BoundingBox ZERO;
		static const BoundingBox MAX;
	};
}