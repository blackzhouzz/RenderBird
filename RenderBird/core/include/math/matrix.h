#pragma once
#include "core_private.h"
#include "mathcore.h"
#include "vector.h"

namespace Core
{
#pragma region
	class Matrix3f
	{
	public:
		Matrix3f() = default;
		Matrix3f(const Float data[3][3]);
		Matrix3f(
			Float m00, Float m01, Float m02,
			Float m10, Float m11, Float m12,
			Float m20, Float m21, Float m22
		);
		Float* operator [] (uint32 rowIndex)const;
		bool operator == (const Matrix3f& mat)const;
		bool operator != (const Matrix3f& mat)const;
		Matrix3f operator + (const Matrix3f& mat)const;
		Matrix3f operator - (const Matrix3f& mat)const;
		Matrix3f operator * (const Matrix3f& mat)const;
		Matrix3f operator - () const;
		Vector3f operator * (const Vector3f& vec)const;
		friend Vector3f operator * (const Vector3f& vec, const Matrix3f& mat);
		Matrix3f operator * (Float scalar)const;
		friend Vector3f operator * (const Vector3f& vec, const Matrix3f& mat);
		Vector3f GetRow(uint32 axisIndex)const;
		void SetRow(uint32 axisIndex, const Vector3f& vec);
		Vector3f GetColumn(uint32 colIndex)const;
		void SetColumn(uint32 colIndex, const Vector3f& vec);
		void FromAxes(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ);
		void ToAxes(Vector3f& axisX, Vector3f& axisY, Vector3f& axisZ)const;
		Matrix3f Transpose()const;
		bool Inverse(Matrix3f& invMat, Float tolerance = C_FLOAT_EPSILON)const;
		Matrix3f Inverse(Float tolerance = C_FLOAT_EPSILON)const;
		Float Determinant()const;
		static const Matrix3f ZERO;
		static const Matrix3f IDENTITY;
	public:
		union
		{
			Float m[3][3];
			Float m9[9];
		};
	};
#pragma endregion Matrix3f

#pragma region
	class Matrix4f
	{
	public:
		Matrix4f(void) = default;
		Matrix4f(const Matrix3f& mat, const Vector3f& trans = Vector3f::ZERO);
		Matrix4f(
			Float m00, Float m01, Float m02, Float m03,
			Float m10, Float m11, Float m12, Float m13,
			Float m20, Float m21, Float m22, Float m23,
			Float m30, Float m31, Float m32, Float m33
		);
		Float* operator [] (uint32 rowIndex);
		const Float *const operator [] (uint32 rowIndex)const;
		Matrix4f operator * (const Matrix4f& mat)const;
		Matrix4f& operator *= (const Matrix4f& mat);
		Matrix4f operator + (const Matrix4f& mat)const;
		Matrix4f& operator += (const Matrix4f& mat);
		Matrix4f operator - (const Matrix4f& mat)const;
		Matrix4f& operator -= (const Matrix4f& mat);
		bool operator == (const Matrix4f& mat)const;
		bool operator != (const Matrix4f& mat)const;
		Vector3f TransformPerspective(const Vector3f& vec)const;
		Vector3f TransformPoint(const Vector3f& vec)const;
		Vector4f TransformVector(const Vector4f& vec)const;
		Vector3f TransformDirection(const Vector3f& vec)const;
		Matrix4f Transpose(void)const;
		void SetTranslation(const Vector3f& vec, Float w = 1.0f);
		Vector3f GetTranslation()const;
		Vector3f GetScale()const;
		void SetScale(const Vector3f& scale);
		void MultiplyScale(const Vector3f& scale);
		Vector4f GetRow(uint32 rowIndex)const;
		void SetRow(uint32 rowIndex, const Vector4f& vec);
		Vector4f GetColumn(uint32 colIndex)const;
		void SetColumn(uint32 colIndex, const Vector4f& vec);
		Matrix4f Inverse()const;
		Matrix4f InverseSafe()const;
		Matrix3f GetMatrix3()const;
		bool SetAxis(int axisIndex, const Vector3f& newAxis);
	public:
		static const Matrix4f IDENTITY;
		static const Matrix4f ZERO;
		static const Matrix4f TEX;
		static const Matrix4f YtoZ;
		union
		{
			Float m[4][4];
			Float m16[16];
		};
	};
#pragma endregion Matrix4f
}