#include "LNCorePrivate.h"
#include "LNBoundingBox.h"

namespace Luna
{
	const BoundingBox BoundingBox::INVALID(
		Vector3Float(POS_INFINITY_FLOAT, POS_INFINITY_FLOAT, POS_INFINITY_FLOAT),
		Vector3Float(NEG_INFINITY_FLOAT, NEG_INFINITY_FLOAT, NEG_INFINITY_FLOAT)
		);
	const BoundingBox BoundingBox::ZERO(
		Vector3Float(0, 0, 0),
		Vector3Float(0, 0, 0)
		);
	const BoundingBox BoundingBox::infinity(
		Vector3Float(NEG_INFINITY_FLOAT, NEG_INFINITY_FLOAT, NEG_INFINITY_FLOAT),
		Vector3Float(POS_INFINITY_FLOAT * 0.5f, POS_INFINITY_FLOAT * 0.5f, POS_INFINITY_FLOAT * 0.5f)
		);

	BoundingBox::BoundingBox(const Vector3Float& minValue, const Vector3Float& maxValue)
	{
		this->mMin = minValue;
		this->mMax = maxValue;
	}

	//BoundingBox& BoundingBox::operator = (const BoundingBox& box)
	//{
	//	this->mMin = box.mMin;
	//	this->mMax = box.mMax;
	//	return *this;
	//}

	bool BoundingBox::operator == (const BoundingBox& box)const
	{
		return mMin == box.mMin && mMax == box.mMax;
	}

	bool BoundingBox::operator != (const BoundingBox& box)const
	{
		return mMin != box.mMin || mMax != box.mMax;
	}

	BoundingBox BoundingBox::operator + (const BoundingBox& box)const
	{
		BoundingBox ret = *this;
		ret.mMin.Floor(box.mMin);
		ret.mMax.Ceil(box.mMax);
		return ret;
	}

	BoundingBox& BoundingBox::operator += (const BoundingBox& box)
	{
		mMin.Floor(box.mMin);
		mMax.Ceil(box.mMax);
		return *this;
	}

	BoundingBox BoundingBox::operator + (const Vector3Float& vec)const
	{
		BoundingBox ret = *this;
		ret.mMin.Floor(vec);
		ret.mMax.Ceil(vec);
		return ret;
	}

	BoundingBox& BoundingBox::operator += (const Vector3Float& vec)
	{
		mMin.Floor(vec);
		mMax.Ceil(vec);
		return *this;
	}

	void BoundingBox::ClampMin(const Vector3Float& vec)
	{
		mMin.Floor(vec);
	}

	void BoundingBox::ClampMax(const Vector3Float& vec)
	{
		mMax.Ceil(vec);
	}

	bool BoundingBox::IsValid()const
	{
		return (!(mMin.x > mMax.x)) && (!(mMin.y > mMax.y)) && (!(mMin.z > mMax.z));
	}

	void BoundingBox::Set(const Vector3Float& minValue, const Vector3Float& maxOrExt, bool isExt)
	{
		mMin = minValue;
		if (isExt)
		{
			mMax = minValue + maxOrExt;
		}
		else
		{
			mMax = maxOrExt;
		}
	}

	void BoundingBox::Set(const BoundingBox& box)
	{
		*this = box;
	}

	Vector3Float BoundingBox::GetCenter()const
	{
		return (mMax + mMin) / 2;
	}

	Vector3Float BoundingBox::GetDiagonal()const
	{
		return mMax - mMin;
	}

	float BoundingBox::GetDiagonalLength()const
	{
		return (mMax - mMin).Length();
	}

	bool BoundingBox::IsContain(const Vector3Float& point)const 
	{
		return (point.x >= mMin.x) && (point.x <= mMax.x) &&
			(point.y >= mMin.y) && (point.y <= mMax.y) &&
			(point.z >= mMin.z) && (point.z <= mMax.z);
	}

	bool BoundingBox::IsContain(const BoundingBox& box)const
	{
		return !(box.mMin.x < mMin.x || box.mMin.y < mMin.y || box.mMin.z < mMin.z ||
			box.mMax.x > mMax.x || box.mMax.y > mMax.y || box.mMax.z > mMax.z);
	}

	Vector3Float BoundingBox::ClosestPointTo(const Vector3Float& point)const 
	{
		Vector3Float r = point;
		if (point.x < mMin.x) 
		{
			r.x = mMin.x;
		}
		else if (point.x > mMax.x)
		{
			r.x = mMax.x;
		}

		if (point.y < mMin.y)
		{
			r.y = mMin.y;
		}
		else if (point.y > mMax.y)
		{
			r.y = mMax.y;
		}

		if (point.z < mMin.z)
		{
			r.z = mMin.z;
		}
		else if (point.z > mMax.z)
		{
			r.z = mMax.z;
		}

		return r;
	}

	Vector3Float BoundingBox::FurthestPointTo(const Vector3Float& point)const
	{
		Vector3Float r;
		if ( abs(point.x-mMin.x) < abs(point.x-mMax.x) )
		{
			r.x = mMax.x;
		}
		else
		{
			r.x = mMin.x;
		}
		if ( abs(point.y-mMin.y) < abs(point.y-mMax.y) )
		{
			r.y = mMax.y;
		}
		else
		{
			r.y = mMin.y;
		}
		if ( abs(point.z-mMin.z) < abs(point.z-mMax.z) )
		{
			r.z = mMax.z;
		}
		else
		{
			r.z = mMin.z;
		}
		return r;
	}

	float BoundingBox::GetArea()const
	{
		if (!IsValid())
		{
			return -1.0f;
		}
		return 2.0f * (mMax.z - mMin.z) * (mMax.y - mMin.y) * (mMax.x - mMin.x);
	}

	float BoundingBox::GetWidth()const
	{
		return mMax.x - mMin.x;
	}

	float BoundingBox::GetHeight()const
	{
		return mMax.y - mMin.y;
	}

	float BoundingBox::GetDepth()const
	{
		return mMax.z - mMin.z;
	}

	EAxis BoundingBox::GetLongestAxis(float* length)const
	{
		float fHeight = GetHeight();
		float fWidth = GetWidth();
		float fDepth = GetDepth();
		if (fWidth > fHeight)
		{
			if (fWidth > fDepth)
			{
				if (length != NULL)
				{
					*length = fWidth;
				}
				return AXIS_X;
			}
			else
			{
				if (length != NULL)
				{
					*length = fDepth;
				}
				return AXIS_Z;
			}
		}
		else
		{
			if (fHeight > fDepth)
			{
				if (length != NULL)
				{
					*length = fHeight;
				}
				return AXIS_Y;
			}
			else
			{
				if (length != NULL)
				{
					*length = fDepth;
				}
				return AXIS_Z;
			}
		}
	}

	EAxis BoundingBox::GetShortestAxis(float* length)const
	{
		float fHeight = GetHeight();
		float fWidth = GetWidth();
		float fDepth = GetDepth();
		if (fWidth < fHeight)
		{
			if (fWidth < fDepth)
			{
				if (length != NULL)
				{
					*length = fWidth;
				}
				return AXIS_X;
			}
			else
			{
				if (length != NULL)
				{
					*length = fDepth;
				}
				return AXIS_Z;
			}
		}
		else
		{
			if (fHeight < fDepth)
			{
				if (length != NULL)
				{
					*length = fHeight;
				}
				return AXIS_Y;
			}
			else
			{
				if (length != NULL)
				{
					*length = fDepth;
				}
				return AXIS_Z;
			}
		}
	}

	void BoundingBox::DeformExtent(float size)
	{
		mMin -= Vector3Float(0.5f * size, 0.5f * size, 0.5f * size);
		mMax += Vector3Float(0.5f * size, 0.5f * size, 0.5f * size);
	}

	void BoundingBox::MakeInside(const BoundingBox& box)
	{
		Vector3Float dia = GetDiagonal();
		if (mMin.x < box.mMin.x)
		{
			mMin.x = box.mMin.x;
		}
		if (mMin.y < box.mMin.y)
		{
			mMin.y = box.mMin.y;
		}
		if (mMin.z < box.mMin.z)
		{
			mMin.z = box.mMin.z;
		}
		mMax = mMin + dia;

		if (mMax.x > box.mMax.x)
		{
			mMax.x = box.mMax.x;
		}
		if (mMax.y > box.mMax.y)
		{
			mMax.y = box.mMax.y;
		}
		if (mMax.z > box.mMax.z)
		{
			mMax.z = box.mMax.z;
		}
		mMin = mMax - dia;
	}

	void BoundingBox::MakeVecInside(Vector3Float& vec)
	{
		vec.Ceil(mMin);
		vec.Floor(mMax);
	}

	void BoundingBox::MoveDelta(const Vector3Float& vec)
	{
		Vector3Float dia = GetDiagonal();
		mMin += vec;
		mMax = mMin + dia;
	}

	void BoundingBox::MoveAbs(const Vector3Float& abs)
	{
		Vector3Float dia = GetDiagonal();
		mMin = abs - dia / 2;
		mMax = abs + dia / 2;
	}

	float BoundingBox::DistanceFrom(const Vector3Float& point)const
	{
		return GetCenter().DistanceSQ(point);
	}

	float BoundingBox::DistanceFromSQ(const Vector3Float& point)const
	{
		return GetCenter().Distance(point);
	}

	BoundingBox BoundingBox::TransformBy(const Matrix4f& mat)const
	{
		if (!IsValid())
		{
			return BoundingBox::INVALID;
		}
		BoundingBox ret;
		ret.mMin = ret.mMax = mat.GetTranslation();
		if (mat[0][0] > 0.0f)
		{
			ret.mMin.x += mat[0][0] * mMin.x; 
			ret.mMax.x += mat[0][0] * mMax.x;
		}
		else
		{
			ret.mMin.x += mat[0][0] * mMax.x; 
			ret.mMax.x += mat[0][0] * mMin.x;
		}
		if (mat[0][1] > 0.0f)
		{
			ret.mMin.y += mat[0][1] * mMin.x; 
			ret.mMax.y += mat[0][1] * mMax.x;
		}
		else
		{
			ret.mMin.y += mat[0][1] * mMax.x; 
			ret.mMax.y += mat[0][1] * mMin.x;
		}
		if (mat[0][2] > 0.0f)
		{
			ret.mMin.z += mat[0][2] * mMin.x; 
			ret.mMax.z += mat[0][2] * mMax.x;
		}
		else
		{
			ret.mMin.z += mat[0][2] * mMax.x; 
			ret.mMax.z += mat[0][2] * mMin.x;
		}

		if (mat[1][0] > 0.0f)
		{
			ret.mMin.x += mat[1][0] * mMin.y; 
			ret.mMax.x += mat[1][0] * mMax.y;
		}
		else
		{
			ret.mMin.x += mat[1][0] * mMax.y; 
			ret.mMax.x += mat[1][0] * mMin.y;
		}
		if (mat[1][1] > 0.0f)
		{
			ret.mMin.y += mat[1][1] * mMin.y; 
			ret.mMax.y += mat[1][1] * mMax.y;
		}
		else
		{
			ret.mMin.y += mat[1][1] * mMax.y; 
			ret.mMax.y += mat[1][1] * mMin.y;
		}
		if (mat[1][2] > 0.0f)
		{
			ret.mMin.z += mat[1][2] * mMin.y; 
			ret.mMax.z += mat[1][2] * mMax.y;
		}
		else
		{
			ret.mMin.z += mat[1][2] * mMax.y; 
			ret.mMax.z += mat[1][2] * mMin.y;
		}
		if (mat[2][0] > 0.0f)
		{
			ret.mMin.x += mat[2][0] * mMin.z; 
			ret.mMax.x += mat[2][0] * mMax.z;
		}
		else
		{
			ret.mMin.x += mat[2][0] * mMax.z; 
			ret.mMax.x += mat[2][0] * mMin.z;
		}
		if (mat[2][1] > 0.0f)
		{
			ret.mMin.y += mat[2][1] * mMin.z; 
			ret.mMax.y += mat[2][1] * mMax.z;
		}
		else
		{
			ret.mMin.y += mat[2][1] * mMax.z; 
			ret.mMax.y += mat[2][1] * mMin.z;
		}
		if (mat[2][2] > 0.0f)
		{
			ret.mMin.z += mat[2][2] * mMin.z; 
			ret.mMax.z += mat[2][2] * mMax.z;
		}
		else
		{
			ret.mMin.z += mat[2][2] * mMax.z; 
			ret.mMax.z += mat[2][2] * mMin.z;
		}
		return ret;
	}
}