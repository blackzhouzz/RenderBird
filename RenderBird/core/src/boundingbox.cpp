#include "boundingbox.h"
#include <limits>

namespace Core
{
	const BoundingBox BoundingBox::INVALID(
		Vector3f(C_FLOAT_MAX, C_FLOAT_MAX, C_FLOAT_MAX) * (Float)0.5f,
		Vector3f(C_FLOAT_LOWEST, C_FLOAT_LOWEST, C_FLOAT_LOWEST) * (Float)0.5f
	);
	const BoundingBox BoundingBox::ZERO(
		Vector3f(0, 0, 0),
		Vector3f(0, 0, 0)
	);
	const BoundingBox BoundingBox::MAX(
		Vector3f(C_FLOAT_LOWEST, C_FLOAT_LOWEST, C_FLOAT_LOWEST),
		Vector3f(C_FLOAT_MAX * (Float)0.5f, C_FLOAT_MAX * (Float)0.5f, C_FLOAT_MAX * (Float)0.5f)
	);

	BoundingBox::BoundingBox(const Vector3f& minValue, const Vector3f& maxValue)
	{
		m_min = minValue;
		m_max = maxValue;
	}

	bool BoundingBox::operator == (const BoundingBox& box)const
	{
		return m_min == box.m_min && m_max == box.m_max;
	}

	bool BoundingBox::operator != (const BoundingBox& box)const
	{
		return m_min != box.m_min || m_max != box.m_max;
	}

	BoundingBox BoundingBox::operator + (const BoundingBox& box)const
	{
		BoundingBox ret = *this;
		ret.m_min = Vector3f::Min(ret.m_min, box.m_min);
		ret.m_max = Vector3f::Max(ret.m_max, box.m_max);
		return ret;
	}

	BoundingBox& BoundingBox::operator += (const BoundingBox& box)
	{
		m_min = Vector3f::Min(m_min, box.m_min);
		m_max = Vector3f::Max(m_max, box.m_max);
		return *this;
	}

	BoundingBox BoundingBox::operator + (const Vector3f& vec)const
	{
		BoundingBox ret = *this;
		ret.m_min = Vector3f::Min(ret.m_min, m_min);
		ret.m_max = Vector3f::Max(ret.m_max, m_max);
		return ret;
	}

	BoundingBox& BoundingBox::operator += (const Vector3f& vec)
	{
		m_min = Vector3f::Min(vec, m_min);
		m_max = Vector3f::Max(vec, m_max);
		return *this;
	}

	bool BoundingBox::IsValid()const
	{
		return (!(m_min.x > m_max.x)) && (!(m_min.y > m_max.y)) && (!(m_min.z > m_max.z));
	}

	void BoundingBox::Set(const Vector3f& minValue, const Vector3f& maxOrExt, bool isExt)
	{
		m_min = minValue;
		if (isExt)
		{
			m_max = minValue + maxOrExt;
		}
		else
		{
			m_max = maxOrExt;
		}
	}

	Vector3f BoundingBox::GetCenter()const
	{
		return (m_max + m_min) * (Float)0.5f;
	}

	Vector3f BoundingBox::GetDiagonal()const
	{
		return m_max - m_min;
	}

	Float BoundingBox::GetDiagonalLength()const
	{
		return (m_max - m_min).Length();
	}

	bool BoundingBox::IsContain(const Vector3f& point)const
	{
		return (point.x >= m_min.x) && (point.x <= m_max.x) &&
			(point.y >= m_min.y) && (point.y <= m_max.y) &&
			(point.z >= m_min.z) && (point.z <= m_max.z);
	}

	bool BoundingBox::IsContain(const BoundingBox& box)const
	{
		return !(box.m_min.x < m_min.x || box.m_min.y < m_min.y || box.m_min.z < m_min.z ||
			box.m_max.x > m_max.x || box.m_max.y > m_max.y || box.m_max.z > m_max.z);
	}

	Vector3f BoundingBox::ClosestPointTo(const Vector3f& point)const
	{
		Vector3f r = point;
		if (point.x < m_min.x)
		{
			r.x = m_min.x;
		}
		else if (point.x > m_max.x)
		{
			r.x = m_max.x;
		}

		if (point.y < m_min.y)
		{
			r.y = m_min.y;
		}
		else if (point.y > m_max.y)
		{
			r.y = m_max.y;
		}

		if (point.z < m_min.z)
		{
			r.z = m_min.z;
		}
		else if (point.z > m_max.z)
		{
			r.z = m_max.z;
		}

		return r;
	}

	Vector3f BoundingBox::FurthestPointTo(const Vector3f& point)const
	{
		Vector3f r;
		if (abs(point.x - m_min.x) < abs(point.x - m_max.x))
		{
			r.x = m_max.x;
		}
		else
		{
			r.x = m_min.x;
		}
		if (abs(point.y - m_min.y) < abs(point.y - m_max.y))
		{
			r.y = m_max.y;
		}
		else
		{
			r.y = m_min.y;
		}
		if (abs(point.z - m_min.z) < abs(point.z - m_max.z))
		{
			r.z = m_max.z;
		}
		else
		{
			r.z = m_min.z;
		}
		return r;
	}

	Float BoundingBox::GetArea()const
	{
		if (!IsValid())
		{
			return -1.0f;
		}
		return 2.0f * (m_max.z - m_min.z) * (m_max.y - m_min.y) * (m_max.x - m_min.x);
	}

	Float BoundingBox::GetWidth()const
	{
		return m_max.x - m_min.x;
	}

	Float BoundingBox::GetHeight()const
	{
		return m_max.y - m_min.y;
	}

	Float BoundingBox::GetDepth()const
	{
		return m_max.z - m_min.z;
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
		m_min -= Vector3f(0.5f * size, 0.5f * size, 0.5f * size);
		m_max += Vector3f(0.5f * size, 0.5f * size, 0.5f * size);
	}

	void BoundingBox::MakeInside(const BoundingBox& box)
	{
		Vector3f dia = GetDiagonal();
		if (m_min.x < box.m_min.x)
		{
			m_min.x = box.m_min.x;
		}
		if (m_min.y < box.m_min.y)
		{
			m_min.y = box.m_min.y;
		}
		if (m_min.z < box.m_min.z)
		{
			m_min.z = box.m_min.z;
		}
		m_max = m_min + dia;

		if (m_max.x > box.m_max.x)
		{
			m_max.x = box.m_max.x;
		}
		if (m_max.y > box.m_max.y)
		{
			m_max.y = box.m_max.y;
		}
		if (m_max.z > box.m_max.z)
		{
			m_max.z = box.m_max.z;
		}
		m_min = m_max - dia;
	}

	void BoundingBox::MoveDelta(const Vector3f& vec)
	{
		Vector3f dia = GetDiagonal();
		m_min += vec;
		m_max = m_min + dia;
	}

	void BoundingBox::MoveAbs(const Vector3f& abs)
	{
		Vector3f dia = GetDiagonal();
		m_min = abs - dia * (Float)0.5f;
		m_max = abs + dia * (Float)0.5f;
	}

	Float BoundingBox::DistanceFrom(const Vector3f& point)const
	{
		return (GetCenter() - point).Length();
	}
}