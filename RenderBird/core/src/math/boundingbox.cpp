#include "math/boundingbox.h"
#include <limits>

namespace Core
{
	const BoundingBox BoundingBox::INVALID(
		Vector3f(std::numeric_limits<Float>::max(), std::numeric_limits<Float>::max(), std::numeric_limits<Float>::max()),
		Vector3f(std::numeric_limits<Float>::lowest(), std::numeric_limits<Float>::lowest(), std::numeric_limits<Float>::lowest())
	);
	const BoundingBox BoundingBox::ZERO(
		Vector3f(0, 0, 0),
		Vector3f(0, 0, 0)
	);
	const BoundingBox BoundingBox::MAX(
		Vector3f(std::numeric_limits<Float>::lowest(), std::numeric_limits<Float>::lowest(), std::numeric_limits<Float>::lowest()),
		Vector3f(std::numeric_limits<Float>::max() * 0.5f, std::numeric_limits<Float>::max() * 0.5f, std::numeric_limits<Float>::max() * 0.5f)
	);

	BoundingBox::BoundingBox(const Vector3f& minValue, const Vector3f& maxValue)
	{
		min = minValue;
		max = maxValue;
	}

	bool BoundingBox::operator == (const BoundingBox& box)const
	{
		return min == box.min && max == box.max;
	}

	bool BoundingBox::operator != (const BoundingBox& box)const
	{
		return min != box.min || max != box.max;
	}

	BoundingBox BoundingBox::operator + (const BoundingBox& box)const
	{
		BoundingBox ret = *this;
		ret.min.Floor(box.min);
		ret.max.Ceil(box.max);
		return ret;
	}

	BoundingBox& BoundingBox::operator += (const BoundingBox& box)
	{
		min.Floor(box.min);
		max.Ceil(box.max);
		return *this;
	}

	BoundingBox BoundingBox::operator + (const Vector3f& vec)const
	{
		BoundingBox ret = *this;
		ret.min.Floor(vec);
		ret.max.Ceil(vec);
		return ret;
	}

	BoundingBox& BoundingBox::operator += (const Vector3f& vec)
	{
		min.Floor(vec);
		max.Ceil(vec);
		return *this;
	}

	bool BoundingBox::IsValid()const
	{
		return (!(min.x > max.x)) && (!(min.y > max.y)) && (!(min.z > max.z));
	}

	void BoundingBox::Set(const Vector3f& minValue, const Vector3f& maxOrExt, bool isExt)
	{
		min = minValue;
		if (isExt)
		{
			max = minValue + maxOrExt;
		}
		else
		{
			max = maxOrExt;
		}
	}

	Vector3f BoundingBox::GetCenter()const
	{
		return (max + min) * 0.5;
	}

	Vector3f BoundingBox::GetDiagonal()const
	{
		return max - min;
	}

	Float BoundingBox::GetDiagonalLength()const
	{
		return (max - min).Length();
	}

	bool BoundingBox::IsContain(const Vector3f& point)const
	{
		return (point.x >= min.x) && (point.x <= max.x) &&
			(point.y >= min.y) && (point.y <= max.y) &&
			(point.z >= min.z) && (point.z <= max.z);
	}

	bool BoundingBox::IsContain(const BoundingBox& box)const
	{
		return !(box.min.x < min.x || box.min.y < min.y || box.min.z < min.z ||
			box.max.x > max.x || box.max.y > max.y || box.max.z > max.z);
	}

	Vector3f BoundingBox::ClosestPointTo(const Vector3f& point)const
	{
		Vector3f r = point;
		if (point.x < min.x)
		{
			r.x = min.x;
		}
		else if (point.x > max.x)
		{
			r.x = max.x;
		}

		if (point.y < min.y)
		{
			r.y = min.y;
		}
		else if (point.y > max.y)
		{
			r.y = max.y;
		}

		if (point.z < min.z)
		{
			r.z = min.z;
		}
		else if (point.z > max.z)
		{
			r.z = max.z;
		}

		return r;
	}

	Vector3f BoundingBox::FurthestPointTo(const Vector3f& point)const
	{
		Vector3f r;
		if (abs(point.x - min.x) < abs(point.x - max.x))
		{
			r.x = max.x;
		}
		else
		{
			r.x = min.x;
		}
		if (abs(point.y - min.y) < abs(point.y - max.y))
		{
			r.y = max.y;
		}
		else
		{
			r.y = min.y;
		}
		if (abs(point.z - min.z) < abs(point.z - max.z))
		{
			r.z = max.z;
		}
		else
		{
			r.z = min.z;
		}
		return r;
	}

	Float BoundingBox::GetArea()const
	{
		if (!IsValid())
		{
			return -1.0;
		}
		return 2.0 * (max.z - min.z) * (max.y - min.y) * (max.x - min.x);
	}

	Float BoundingBox::GetWidth()const
	{
		return max.x - min.x;
	}

	Float BoundingBox::GetHeight()const
	{
		return max.y - min.y;
	}

	Float BoundingBox::GetDepth()const
	{
		return max.z - min.z;
	}

	Axis BoundingBox::GetLongestAxis(Float* length)const
	{
		Float height = GetHeight();
		Float width = GetWidth();
		Float depth = GetDepth();
		if (width > height)
		{
			if (width > depth)
			{
				if (length != nullptr)
				{
					*length = width;
				}
				return Axis::X;
			}
			else
			{
				if (length != nullptr)
				{
					*length = depth;
				}
				return Axis::Z;
			}
		}
		else
		{
			if (height > depth)
			{
				if (length != nullptr)
				{
					*length = height;
				}
				return Axis::Y;
			}
			else
			{
				if (length != nullptr)
				{
					*length = depth;
				}
				return Axis::Z;
			}
		}
	}

	Axis BoundingBox::GetShortestAxis(Float* length)const
	{
		Float height = GetHeight();
		Float width = GetWidth();
		Float depth = GetDepth();
		if (width < height)
		{
			if (width < depth)
			{
				if (length != nullptr)
				{
					*length = width;
				}
				return Axis::X;
			}
			else
			{
				if (length != nullptr)
				{
					*length = depth;
				}
				return Axis::Z;
			}
		}
		else
		{
			if (height < depth)
			{
				if (length != nullptr)
				{
					*length = height;
				}
				return Axis::Y;
			}
			else
			{
				if (length != nullptr)
				{
					*length = depth;
				}
				return Axis::Z;
			}
		}
	}

	void BoundingBox::ExpandSize(Float size)
	{
		min -= Vector3f(0.5f * size, 0.5f * size, 0.5f * size);
		max += Vector3f(0.5f * size, 0.5f * size, 0.5f * size);
	}

	void BoundingBox::MakeInside(const BoundingBox& box)
	{
		Vector3f dia = GetDiagonal();
		if (min.x < box.min.x)
		{
			min.x = box.min.x;
		}
		if (min.y < box.min.y)
		{
			min.y = box.min.y;
		}
		if (min.z < box.min.z)
		{
			min.z = box.min.z;
		}
		max = min + dia;

		if (max.x > box.max.x)
		{
			max.x = box.max.x;
		}
		if (max.y > box.max.y)
		{
			max.y = box.max.y;
		}
		if (max.z > box.max.z)
		{
			max.z = box.max.z;
		}
		min = max - dia;
	}

	void BoundingBox::MoveDelta(const Vector3f& vec)
	{
		Vector3f dia = GetDiagonal();
		min += vec;
		max = min + dia;
	}

	void BoundingBox::MoveAbs(const Vector3f& abs)
	{
		Vector3f dia = GetDiagonal();
		min = abs - dia * 0.5;
		max = abs + dia * 0.5;
	}

	Float BoundingBox::DistanceFrom(const Vector3f& point)const
	{
		return DistanceSQ(GetCenter(), point);
	}

	Float BoundingBox::DistanceFromSQ(const Vector3f& point)const
	{
		return Distance(GetCenter(), point);
	}
}