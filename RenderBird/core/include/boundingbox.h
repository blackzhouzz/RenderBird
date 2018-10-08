#pragma once
#include "MathCore.h"

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
		bool IsContain(const Vector3f& point)const;
		bool IsContain(const BoundingBox& box)const;
		Vector3f ClosestPointTo(const Vector3f& point) const;
		Vector3f FurthestPointTo(const Vector3f& point) const;
		Float GetArea()const;
		Float GetWidth()const;
		Float GetHeight()const;
		Float GetDepth()const;
		Axis GetLongestAxis(Float* length = nullptr)const;
		Axis GetShortestAxis(Float* length = nullptr)const;
		void ExpandSize(Float size);
		void MakeInside(const BoundingBox& box);
		void MoveDelta(const Vector3f& vec);
		void MoveAbs(const Vector3f& abs);
		Vector3f GetVertex(uint32 index)const;
	public:
		Vector3f m_min;
		Vector3f m_max;
		static const BoundingBox INVALID;
		static const BoundingBox ZERO;
		static const BoundingBox MAX;
	};
}