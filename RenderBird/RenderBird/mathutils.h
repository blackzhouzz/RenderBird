#pragma once
#include "mathcore.h"
#include "matrix.h"
#include "quaternion.h"
#include "eulerrotation.h"
#include "vector.h"

namespace MathLib
{
	class MathUtils
	{
		static Matrix4f TranslateMatrix(const Vector3f& trans);
		static Matrix4f ScaleMatrix(const Vector3f& scale);
		static Matrix4f ScaleMatrix(Float scale);
		static Matrix4f InverseRotationMatrix(Float pitch, Float yaw, Float roll);
		static Matrix4f RotationMatrix(const Vector3f& axis, Float theta);
		static Matrix4f ScaleDirectionMatrix(const Vector3f& dir, Float k);
		static Matrix4f BasisMatrix(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ);
		static Matrix4f OrthoMatrixLH(Float width, Float height, Float nearClip, Float farClip);
		static Matrix4f OrthoOffCenterMatrixLH(Float left, Float right, Float bottom, Float top, Float nearClip, Float farClip);
		static Matrix4f PerspectiveFovMatrixLH(Float fovY, Float aspect, Float nearClip, Float farClip);
		static Matrix4f PerspectiveFovInfiniteMatrixLH(Float fovY, Float aspect, Float nearClip);
		static Matrix4f LookAtMatrixLH(const Vector3f& eyePos, const Vector3f& at, const Vector3f& up);
		static Matrix4f LookAtMatrix(const Vector3f& eyePos, const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ);
		static Quaternion Slerp(Float t, const Quaternion& p, const Quaternion& quat, bool shortestPath = false);
		static Quaternion ExtraRotation(const Matrix4f mat);
		static Quaternion RotationMatrixToQuaternion(const Matrix3f& mat);
		static Matrix3f EulerAnglesYXZToRotationMatrix(Float pitch, Float yaw, Float roll);
		static bool RotationMatrixToEulerAnglesYXZ(const Matrix3f& rotate, Float& pitch, Float& yaw, Float& roll);
		static bool ComputeBarycentricCoords(const Vector3f vert[], const Vector3f& position, Vector3f& b);
		static Matrix3f QuaternionToRotationMatrix(const Quaternion& quat);
		static Quaternion AngleAxisToQuaternion(Float angle, const Vector3f& axis);
		static void QuaternionToAngleAxis(const Quaternion& quat, Float& angle, Vector3f& axis);
		static Quaternion AxesToQuaternion(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ);
		static void QuaternionToAxes(const Quaternion& quat, Vector3f& axisX, Vector3f& axisY, Vector3f& axisZ);
		static EulerRotation QuaternionToEulerRotation(const Quaternion& rotate);
		static Quaternion EulerRotationToQuaternion(const EulerRotation& rotate);
		static Matrix4f MakeTransform(const Vector3f& position, const Quaternion& orient, const Vector3f& scale = Vector3f::ONE);
		static Matrix4f MakeTransform(const Vector3f& position, const EulerRotation& pitchYawRoll, const Vector3f& scale = Vector3f::ONE);
		static void CalcBinormalTangent(Vector3f normal, Vector3f& binormal, Vector3f& tangent);
		static void VectorToRotation(const Vector3f& vec, Float& pitch, Float& yaw, Float& roll);
		static Vector3f RotateVector(const Quaternion& q, const Vector3f& vec);

	};
}