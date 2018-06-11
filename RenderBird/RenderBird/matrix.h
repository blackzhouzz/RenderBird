#pragma once
#include "mathcore.h"
#include "vector.h"

namespace MathLib
{
#pragma region
	class Matrix3f
	{
	public:
		Matrix3f();
		Matrix3f(const Float data[3][3]);
		Matrix3f(const Matrix3f& mat);
		Matrix3f(
			Float m00, Float m01, Float m02,
			Float m10, Float m11, Float m12,
			Float m20, Float m21, Float m22
		);
		Float* operator [] (uint rowIndex)const;
		Matrix3f& operator = (const Matrix3f& mat);
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
		Vector3f GetRow(uint axisIndex)const;
		void SetRow(uint axisIndex, const Vector3f& vec);
		Vector3f GetColumn(uint colIndex)const;
		void SetColumn(uint colIndex, const Vector3f& vec);
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
			struct
			{
				Float _11, _12, _13;
				Float _21, _22, _23;
				Float _31, _32, _33;
			};
		};
	};
#pragma endregion Matrix3f

#pragma region
	class Matrix4f
	{
	public:
		Matrix4f(void);
		Matrix4f(const Matrix4f& mat);
		Matrix4f(const Matrix3f& mat, const Vector3f& trans = Vector3f::ZERO);
		Matrix4f(
			Float m00, Float m01, Float m02, Float m03,
			Float m10, Float m11, Float m12, Float m13,
			Float m20, Float m21, Float m22, Float m23,
			Float m30, Float m31, Float m32, Float m33
		);
		Matrix4f& operator = (const Matrix4f& mat);
		Float* operator [] (uint rowIndex);
		const Float *const operator [] (uint rowIndex)const;
		Matrix4f operator * (const Matrix4f& mat)const;
		Matrix4f& operator *= (const Matrix4f& mat);
		Matrix4f operator + (const Matrix4f& mat)const;
		Matrix4f& operator += (const Matrix4f& mat);
		Matrix4f operator - (const Matrix4f& mat)const;
		Matrix4f& operator -= (const Matrix4f& mat);
		bool operator == (const Matrix4f& mat)const;
		bool operator != (const Matrix4f& mat)const;
		Vector3f TransformVector(const Vector3f& vec)const;
		Vector4f TransformVector(const Vector4f& vec)const;
		Matrix4f Transpose(void)const;
		void SetTranslation(const Vector3f& vec, Float w = 1.0);
		Vector3f GetTranslation()const;
		Vector3f GetScale()const;
		void SetScale(const Vector3f& scale);
		void MultiplyScale(const Vector3f& scale);
		Vector4f GetRow(uint rowIndex)const;
		void SetRow(uint rowIndex, const Vector4f& vec);
		Vector4f GetColumn(uint colIndex)const;
		void SetColumn(uint colIndex, const Vector4f& vec);
		Matrix4f Inverse()const;
		Matrix4f InverseSafe()const;
		Matrix3f GetMatrix3()const;
		bool SetAxis(int axisIndex, const Vector3f& newAxis);
	public:
		static const Matrix4f IDENTITY;
		static const Matrix4f ZERO;
		static const Matrix4f TEX;
		union
		{
			Float m[4][4];
			Float m16[16];
			struct
			{
				Float _11, _12, _13, _14;
				Float _21, _22, _23, _24;
				Float _31, _32, _33, _34;
				Float _41, _42, _43, _44;
			};
		};
	};

	inline Vector3f operator * (const Vector3f& vec, const Matrix4f& mat)
	{
		return mat.TransformVector(vec);
	}

	inline Vector4f operator * (const Vector4f& vec, const Matrix4f& mat)
	{
		return mat.TransformVector(vec);
	}
#pragma endregion Matrix4f
}