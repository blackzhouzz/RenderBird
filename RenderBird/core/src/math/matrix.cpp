#include "math/matrix.h"

namespace Core
{
#pragma region
	const Matrix3f Matrix3f::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0);
	const Matrix3f Matrix3f::IDENTITY(1.0, 0, 0, 0, 1.0, 0, 0, 0, 1.0);

	Matrix3f::Matrix3f(const Float data[3][3])
	{
		memcpy(m, data, 9 * sizeof(Float));
	}

	Matrix3f::Matrix3f(
		Float m00, Float m01, Float m02,
		Float m10, Float m11, Float m12,
		Float m20, Float m21, Float m22)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
	}

	Float* Matrix3f::operator [] (uint rowIndex)const
	{
		return (Float*)m[rowIndex];
	}

	bool Matrix3f::operator == (const Matrix3f& mat)const
	{
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				if (m[rowIndex][colIndex] != mat.m[rowIndex][colIndex])
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Matrix3f::operator != (const Matrix3f& mat)const
	{
		return !operator == (mat);
	}

	Matrix3f Matrix3f::operator + (const Matrix3f& mat)const
	{
		Matrix3f sum;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				sum.m[rowIndex][colIndex] = m[rowIndex][colIndex] + mat.m[rowIndex][colIndex];
			}
		}
		return sum;
	}

	Matrix3f Matrix3f::operator - (const Matrix3f& mat)const
	{
		Matrix3f diff;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				diff.m[rowIndex][colIndex] = m[rowIndex][colIndex] - mat.m[rowIndex][colIndex];
			}
		}
		return diff;
	}

	Matrix3f Matrix3f::operator * (const Matrix3f& mat)const
	{
		Matrix3f prod;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				prod.m[rowIndex][colIndex] = m[rowIndex][0] * mat.m[0][colIndex] + m[rowIndex][1] * mat.m[1][colIndex] + m[rowIndex][2] * mat.m[2][colIndex];
			}
		}
		return prod;
	}

	Matrix3f Matrix3f::operator - ()const
	{
		Matrix3f neg;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				neg[rowIndex][colIndex] = -m[rowIndex][colIndex];
			}
		}
		return neg;
	}

	Vector3f Matrix3f::operator * (const Vector3f& pt)const
	{
		Vector3f prod;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			prod[rowIndex] = m[rowIndex][0] * pt[0] + m[rowIndex][1] * pt[1] + m[rowIndex][2] * pt[2];
		}
		return prod;
	}

	Vector3f operator * (const Vector3f& pt, const Matrix3f& mat)
	{
		Vector3f prod;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			prod[rowIndex] =
				pt[0] * mat.m[0][rowIndex] +
				pt[1] * mat.m[1][rowIndex] +
				pt[2] * mat.m[2][rowIndex];
		}
		return prod;
	}

	Matrix3f Matrix3f::operator * (Float scalar)const
	{
		Matrix3f prod;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				prod[rowIndex][colIndex] = scalar * m[rowIndex][colIndex];
			}
		}
		return prod;
	}

	Matrix3f operator * (Float scalar, const Matrix3f& mat)
	{
		Matrix3f prod;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				prod[rowIndex][colIndex] = scalar * mat.m[rowIndex][colIndex];
			}
		}
		return prod;
	}

	Vector3f Matrix3f::GetRow(uint axisIndex)const
	{
		assert(0 <= axisIndex && axisIndex < 3);
		return Vector3f(m[axisIndex][0], m[axisIndex][1], m[axisIndex][2]);
	}

	void Matrix3f::SetRow(uint axisIndex, const Vector3f& vec)
	{
		assert(0 <= axisIndex && axisIndex < 3);
		m[axisIndex][0] = vec.x;
		m[axisIndex][1] = vec.y;
		m[axisIndex][2] = vec.z;
	}

	Vector3f Matrix3f::GetColumn(uint colIndex)const
	{
		assert(0 <= colIndex && colIndex < 3);
		return Vector3f(m[0][colIndex], m[1][colIndex], m[2][colIndex]);
	}

	void Matrix3f::SetColumn(uint colIndex, const Vector3f& vec)
	{
		assert(0 <= colIndex && colIndex < 3);
		m[0][colIndex] = vec.x;
		m[1][colIndex] = vec.y;
		m[2][colIndex] = vec.z;
	}

	void Matrix3f::FromAxes(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ)
	{
		SetRow(0, axisX);
		SetRow(1, axisY);
		SetRow(2, axisZ);
	}

	void Matrix3f::ToAxes(Vector3f& axisX, Vector3f& axisY, Vector3f& axisZ)const
	{
		axisX = GetRow(0);
		axisY = GetRow(1);
		axisZ = GetRow(2);
	}

	Matrix3f Matrix3f::Transpose()const
	{
		Matrix3f trans;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
			{
				trans[rowIndex][colIndex] = m[colIndex][rowIndex];
			}
		}
		return trans;
	}

	bool Matrix3f::Inverse(Matrix3f& invMat, Float tolerance)const
	{
		// Invert a 3x3 using cofactors.  This is about 8 times faster than
		// the Numerical Recipes code which uses Gaussian elimination.

		invMat[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
		invMat[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
		invMat[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];
		invMat[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
		invMat[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
		invMat[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];
		invMat[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
		invMat[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
		invMat[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

		Float det =
			m[0][0] * invMat[0][0] +
			m[0][1] * invMat[1][0] +
			m[0][2] * invMat[2][0];

		if (fabs(det) <= tolerance)
			return false;

		Float invDet = 1.0 / det;
		for (uint rowIndex = 0; rowIndex < 3; rowIndex++)
		{
			for (uint colIndex = 0; colIndex < 3; colIndex++)
				invMat[rowIndex][colIndex] *= invDet;
		}
		return true;
	}

	Matrix3f Matrix3f::Inverse(Float tolerance)const
	{
		Matrix3f inverse = Matrix3f::ZERO;
		Inverse(inverse, tolerance);
		return inverse;
	}

	Float Matrix3f::Determinant()const
	{
		Float cofactor00 = m[1][1] * m[2][2] -
			m[1][2] * m[2][1];
		Float cofactor10 = m[1][2] * m[2][0] -
			m[1][0] * m[2][2];
		Float cofactor20 = m[1][0] * m[2][1] -
			m[1][1] * m[2][0];

		Float det =
			m[0][0] * cofactor00 +
			m[0][1] * cofactor10 +
			m[0][2] * cofactor20;

		return det;
	}

#pragma endregion Matrix3f

	const Matrix4f Matrix4f::IDENTITY(
		1.0, 0, 0, 0,
		0, 1.0, 0, 0,
		0, 0, 1.0, 0,
		0, 0, 0, 1.0
	);
	const Matrix4f Matrix4f::ZERO(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	);
	const Matrix4f Matrix4f::TEX(
		0.5, 0, 0, 0,
		0, -0.5, 0, 0,
		0, 0, 1.0, 0,
		0.5, 0.5, 0, 1.0
	);

	Matrix4f::Matrix4f(const Matrix3f& mat, const Vector3f& trans)
	{
		m[0][0] = mat.m[0][0]; m[0][1] = mat.m[0][1]; m[0][2] = mat.m[0][2]; m[0][3] = 0;
		m[1][0] = mat.m[1][0]; m[1][1] = mat.m[1][1]; m[1][2] = mat.m[1][2]; m[1][3] = 0;
		m[2][0] = mat.m[2][0]; m[2][1] = mat.m[2][1]; m[2][2] = mat.m[2][2]; m[2][3] = 0;
		m[3][0] = trans.x;	m[3][1] = trans.y;	m[3][2] = trans.z; ; m[3][3] = 1.0;
	}

	Matrix4f::Matrix4f(
		Float m00, Float m01, Float m02, Float m03,
		Float m10, Float m11, Float m12, Float m13,
		Float m20, Float m21, Float m22, Float m23,
		Float m30, Float m31, Float m32, Float m33
	)
	{
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	Float* Matrix4f::operator [] (uint rowIndex)
	{
		assert(rowIndex < 4);
		return m[rowIndex];
	}

	const Float *const Matrix4f::operator [] (uint rowIndex)const
	{
		assert(rowIndex < 4);
		return m[rowIndex];
	}

	Matrix4f Matrix4f::operator * (const Matrix4f &mat)const
	{
		Matrix4f r;
		r.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
		r.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
		r.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
		r.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

		r.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
		r.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
		r.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
		r.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

		r.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
		r.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
		r.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
		r.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

		r.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
		r.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
		r.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
		r.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

		return r;
	}

	Matrix4f& Matrix4f::operator *= (const Matrix4f &mat)
	{
		m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
		m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
		m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
		m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

		m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
		m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
		m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
		m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

		m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
		m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
		m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
		m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

		m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
		m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
		m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
		m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];

		return *this;
	}

	Matrix4f Matrix4f::operator + (const Matrix4f &mat)const
	{
		Matrix4f r;

		r.m[0][0] = m[0][0] + mat.m[0][0];
		r.m[0][1] = m[0][1] + mat.m[0][1];
		r.m[0][2] = m[0][2] + mat.m[0][2];
		r.m[0][3] = m[0][3] + mat.m[0][3];

		r.m[1][0] = m[1][0] + mat.m[1][0];
		r.m[1][1] = m[1][1] + mat.m[1][1];
		r.m[1][2] = m[1][2] + mat.m[1][2];
		r.m[1][3] = m[1][3] + mat.m[1][3];

		r.m[2][0] = m[2][0] + mat.m[2][0];
		r.m[2][1] = m[2][1] + mat.m[2][1];
		r.m[2][2] = m[2][2] + mat.m[2][2];
		r.m[2][3] = m[2][3] + mat.m[2][3];

		r.m[3][0] = m[3][0] + mat.m[3][0];
		r.m[3][1] = m[3][1] + mat.m[3][1];
		r.m[3][2] = m[3][2] + mat.m[3][2];
		r.m[3][3] = m[3][3] + mat.m[3][3];

		return r;
	}

	Matrix4f& Matrix4f::operator += (const Matrix4f &mat)
	{
		m[0][0] = m[0][0] + mat.m[0][0];
		m[0][1] = m[0][1] + mat.m[0][1];
		m[0][2] = m[0][2] + mat.m[0][2];
		m[0][3] = m[0][3] + mat.m[0][3];

		m[1][0] = m[1][0] + mat.m[1][0];
		m[1][1] = m[1][1] + mat.m[1][1];
		m[1][2] = m[1][2] + mat.m[1][2];
		m[1][3] = m[1][3] + mat.m[1][3];

		m[2][0] = m[2][0] + mat.m[2][0];
		m[2][1] = m[2][1] + mat.m[2][1];
		m[2][2] = m[2][2] + mat.m[2][2];
		m[2][3] = m[2][3] + mat.m[2][3];

		m[3][0] = m[3][0] + mat.m[3][0];
		m[3][1] = m[3][1] + mat.m[3][1];
		m[3][2] = m[3][2] + mat.m[3][2];
		m[3][3] = m[3][3] + mat.m[3][3];
		return *this;
	}

	Matrix4f Matrix4f::operator - (const Matrix4f &mat)const
	{
		Matrix4f r;
		r.m[0][0] = m[0][0] - mat.m[0][0];
		r.m[0][1] = m[0][1] - mat.m[0][1];
		r.m[0][2] = m[0][2] - mat.m[0][2];
		r.m[0][3] = m[0][3] - mat.m[0][3];

		r.m[1][0] = m[1][0] - mat.m[1][0];
		r.m[1][1] = m[1][1] - mat.m[1][1];
		r.m[1][2] = m[1][2] - mat.m[1][2];
		r.m[1][3] = m[1][3] - mat.m[1][3];

		r.m[2][0] = m[2][0] - mat.m[2][0];
		r.m[2][1] = m[2][1] - mat.m[2][1];
		r.m[2][2] = m[2][2] - mat.m[2][2];
		r.m[2][3] = m[2][3] - mat.m[2][3];

		r.m[3][0] = m[3][0] - mat.m[3][0];
		r.m[3][1] = m[3][1] - mat.m[3][1];
		r.m[3][2] = m[3][2] - mat.m[3][2];
		r.m[3][3] = m[3][3] - mat.m[3][3];

		return r;
	}

	Matrix4f& Matrix4f::operator -= (const Matrix4f &mat)
	{
		m[0][0] = m[0][0] - mat.m[0][0];
		m[0][1] = m[0][1] - mat.m[0][1];
		m[0][2] = m[0][2] - mat.m[0][2];
		m[0][3] = m[0][3] - mat.m[0][3];

		m[1][0] = m[1][0] - mat.m[1][0];
		m[1][1] = m[1][1] - mat.m[1][1];
		m[1][2] = m[1][2] - mat.m[1][2];
		m[1][3] = m[1][3] - mat.m[1][3];

		m[2][0] = m[2][0] - mat.m[2][0];
		m[2][1] = m[2][1] - mat.m[2][1];
		m[2][2] = m[2][2] - mat.m[2][2];
		m[2][3] = m[2][3] - mat.m[2][3];

		m[3][0] = m[3][0] - mat.m[3][0];
		m[3][1] = m[3][1] - mat.m[3][1];
		m[3][2] = m[3][2] - mat.m[3][2];
		m[3][3] = m[3][3] - mat.m[3][3];

		return *this;
	}

	bool Matrix4f::operator == (const Matrix4f& mat) const
	{
		if (
			m[0][0] != mat.m[0][0] || m[0][1] != mat.m[0][1] || m[0][2] != mat.m[0][2] || m[0][3] != mat.m[0][3] ||
			m[1][0] != mat.m[1][0] || m[1][1] != mat.m[1][1] || m[1][2] != mat.m[1][2] || m[1][3] != mat.m[1][3] ||
			m[2][0] != mat.m[2][0] || m[2][1] != mat.m[2][1] || m[2][2] != mat.m[2][2] || m[2][3] != mat.m[2][3] ||
			m[3][0] != mat.m[3][0] || m[3][1] != mat.m[3][1] || m[3][2] != mat.m[3][2] || m[3][3] != mat.m[3][3])
			return false;
		return true;
	}

	bool Matrix4f::operator != (const Matrix4f& mat)const
	{
		if (m[0][0] != mat.m[0][0] || m[0][1] != mat.m[0][1] || m[0][2] != mat.m[0][2] || m[0][3] != mat.m[0][3] ||
			m[1][0] != mat.m[1][0] || m[1][1] != mat.m[1][1] || m[1][2] != mat.m[1][2] || m[1][3] != mat.m[1][3] ||
			m[2][0] != mat.m[2][0] || m[2][1] != mat.m[2][1] || m[2][2] != mat.m[2][2] || m[2][3] != mat.m[2][3] ||
			m[3][0] != mat.m[3][0] || m[3][1] != mat.m[3][1] || m[3][2] != mat.m[3][2] || m[3][3] != mat.m[3][3])
			return true;
		return false;
	}

	Vector3f Matrix4f::TransformVector(const Vector3f& vec)const
	{
		Vector3f r;
		Float fInvW = 1.0 / (m[0][3] * vec.x + m[1][3] * vec.y + m[2][3] * vec.z + m[3][3]);
		r.x = (m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0]) * fInvW;
		r.y = (m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1]) * fInvW;
		r.z = (m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2]) * fInvW;
		return r;
	}

	Vector4f Matrix4f::TransformVector(const Vector4f& vec)const
	{
		Vector4f r;
		r.x = (m[0][0] * vec.x + m[1][0] * vec.y + m[2][0] * vec.z + m[3][0] * vec.w);
		r.y = (m[0][1] * vec.x + m[1][1] * vec.y + m[2][1] * vec.z + m[3][1] * vec.w);
		r.z = (m[0][2] * vec.x + m[1][2] * vec.y + m[2][2] * vec.z + m[3][2] * vec.w);
		r.w = (m[0][3] * vec.x + m[1][3] * vec.y + m[2][3] * vec.z + m[3][3] * vec.w);
		return r;
	}

	Matrix4f Matrix4f::Transpose(void)const
	{
		return Matrix4f(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	void Matrix4f::SetTranslation(const Vector3f& vec, Float w)
	{
		m[3][0] = vec.x;
		m[3][1] = vec.y;
		m[3][2] = vec.z;
		m[3][3] = w;
	}

	Vector3f Matrix4f::GetTranslation()const
	{
		return Vector3f(m[3][0], m[3][1], m[3][2]);
	}

	Vector3f Matrix4f::GetScale()const
	{
		return Vector3f(m[0][0], m[1][1], m[2][2]);
	}

	void Matrix4f::SetScale(const Vector3f& scale)
	{
		m[0][0] = scale.x;
		m[1][1] = scale.y;
		m[2][2] = scale.z;
	}

	void Matrix4f::MultiplyScale(const Vector3f& scale)
	{
		m[0][0] *= scale.x;
		m[1][1] *= scale.y;
		m[2][2] *= scale.z;
	}

	Vector4f Matrix4f::GetRow(uint axis)const
	{
		assert(0 <= axis && axis < 4);
		return Vector4f(m[axis][0], m[axis][1], m[axis][2], m[axis][3]);
	}

	void Matrix4f::SetRow(uint axis, const Vector4f& vec)
	{
		assert(0 <= axis && axis < 4);
		m[axis][0] = vec.x;
		m[axis][1] = vec.y;
		m[axis][2] = vec.z;
		m[axis][3] = vec.w;
	}

	Vector4f Matrix4f::GetColumn(uint colIndex)const
	{
		assert(0 <= colIndex && colIndex < 4);
		return Vector4f(m[0][colIndex], m[1][colIndex], m[2][colIndex], m[3][colIndex]);
	}

	void Matrix4f::SetColumn(uint colIndex, const Vector4f& vec)
	{
		assert(0 <= colIndex && colIndex < 4);
		m[0][colIndex] = vec.x;
		m[1][colIndex] = vec.y;
		m[2][colIndex] = vec.z;
		m[3][colIndex] = vec.w;
	}

	Matrix4f Matrix4f::InverseSafe()const
	{
		if (IsEqual(GetRow(0), Vector4f::ZERO) &&
			IsEqual(GetRow(1), Vector4f::ZERO) &&
			IsEqual(GetRow(2), Vector4f::ZERO))
		{
			return Matrix4f(0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 1);
		}
		else
		{
			return Inverse();
		}
	}

	//matrix求逆，见3d数学基础-图形与游戏开发p112,invM = adjM / |M|
	Matrix4f Matrix4f::Inverse()const
	{
		Float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
		Float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
		Float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
		Float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		Float v0 = m20 * m31 - m21 * m30;
		Float v1 = m20 * m32 - m22 * m30;
		Float v2 = m20 * m33 - m23 * m30;
		Float v3 = m21 * m32 - m22 * m31;
		Float v4 = m21 * m33 - m23 * m31;
		Float v5 = m22 * m33 - m23 * m32;

		Float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
		Float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
		Float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
		Float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

		//求矩阵行列式
		Float det = (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
		if (det == 0.0)
		{
			return Matrix4f::IDENTITY;
		}
		Float invDet = 1.0 / det;

		Float d00 = t00 * invDet;
		Float d10 = t10 * invDet;
		Float d20 = t20 * invDet;
		Float d30 = t30 * invDet;

		Float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		Float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		Float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		Float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		Float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		Float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		Float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		Float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		Float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		Float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		Float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		Float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return Matrix4f(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33
		);
	}

	Matrix3f Matrix4f::GetMatrix3()const
	{
		return Matrix3f(
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]
		);
	}

	bool Matrix4f::SetAxis(int axisIndex, const Vector3f& newAxis)
	{
		Vector3f axis[3];
		Vector3f oldAxis = Vector3f(m[axisIndex][0], m[axisIndex][1], m[axisIndex][2]);

		axis[0] = newAxis;
		axis[0].Normalize();
		axis[1] = Cross(axis[0], oldAxis).Normalize();
		axis[2] = Cross(axis[0], axis[1]).Normalize();

		for (int i = 0; i < 3; ++i)
		{
			if (IsEqual(axis[i], Vector3f::ZERO))
			{
				return false;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			int newAxisIndex = (i + axisIndex) % 3;
			m[newAxisIndex][0] = axis[i].x;
			m[newAxisIndex][1] = axis[i].y;
			m[newAxisIndex][2] = axis[i].z;
		}
		return true;
	}


}