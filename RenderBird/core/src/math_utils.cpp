#include "math_utils.h"

namespace Core
{
	Matrix4f MathUtils::TRS(const Vector3f& position, const Quaternion& orient, const Vector3f& scale)
	{
		return Matrix4f::Transform(position, orient.ToMatrix(), scale);
	}

	Matrix4f MathUtils::ScreenToRasterMatrix(const Rect2f& screenBound, const Vector2i& resolution)
	{
		return
			Matrix4f::FromScaleVector(Vector3f((Float)resolution.x, (Float)resolution.y, 1.0f))
			* Matrix4f::FromScaleVector(Vector3f(1.0f / (screenBound.m_max.x - screenBound.m_min.x), 1.0f / (screenBound.m_min.y - screenBound.m_max.y), 1.0f))
			* Matrix4f::FromTranslationVector(Vector3f(-screenBound.m_min.x, -screenBound.m_max.y, 0.0f));
	}

	Matrix3f MathUtils::MakeNormalTransform(const Vector3f& normal)
	{
		Vector3f axisX, axisY;
		if (normal.x != normal.y || normal.x != normal.z)
			axisX = -Vector3f(normal.z - normal.y, normal.x - normal.z, normal.y - normal.x);
		else
			axisX = -Vector3f(normal.z - normal.y, normal.x + normal.z, -normal.y - normal.x);

		axisX = axisX.Normalized();
		axisY = Vector3f::CrossProduct(normal, axisX);
		return FromAxes(axisX, axisY, normal);
	}

	Matrix3f MathUtils::FromAxes(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ)
	{
		return Matrix3f
		(
			axisX.x, axisY.x, axisZ.x,
			axisX.y, axisY.y, axisZ.y,
			axisX.z, axisY.z, axisZ.z
		);
	}

	Vector3f MathUtils::TransformDirection(const Matrix4f& trans, const Vector3f& dir)
	{
		return Matrix4f::ToRotationMatrix(trans) * dir;
	}

	Matrix4f MathUtils::ScaleDirectionMatrix(const Vector3f& dir, Float k)
	{
		return Matrix4f(
			1.0f + (k - 1.0f) * dir.x * dir.x, (k - 1.0f) * dir.x * dir.y, (k - 1.0f) * dir.x * dir.z, 0,
			(k - 1.0f) * dir.x * dir.y, 1.0f + (k - 1.0f) * dir.y * dir.y, (k - 1.0f) * dir.y * dir.z, 0,
			(k - 1.0f) * dir.x * dir.z, (k - 1.0f) * dir.z * dir.y, 1.0f + (k - 1.0f) * dir.z * dir.z, 0,
			0, 0, 0, 1.0f);
	}

	Matrix4f MathUtils::OrthoMatrix(Float width, Float height, Float nearClip, Float farClip)
	{
		return Matrix4f(
			2.0f / width, 0, 0, 0,
			0, 2.0f / height, 0, 0,
			0, 0, 1.0f / (farClip - nearClip), 0,
			0, 0, nearClip / (nearClip - farClip), 1.0f
		);
	}

	Matrix4f MathUtils::OrthoOffCenterMatrix(Float left, Float right, Float bottom, Float top, Float nearClip, Float farClip)
	{
		return Matrix4f(
			2.0f / (right - left), 0, 0, 0,
			0, 2.0f / (top - bottom), 0, 0,
			0, 0, 1.0f / (farClip - nearClip), 0,
			(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f
		);
	}

	Matrix4f MathUtils::PerspectiveFovMatrix(Float fovY, Float aspect, Float nearClip, Float farClip)
	{
		Float scaleY = 1.0f / std::tan(fovY * 0.5f);
		Float scaleX = scaleY / aspect;
		return Matrix4f(
			scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, farClip / (farClip - nearClip), 1.0f,
			0, 0, -nearClip * farClip / (farClip - nearClip), 0
		);
	}

	Vector3f MathUtils::GetForward(const Quaternion& quat)
	{
		auto mat = quat.ToMatrix();
		return Vector3f(mat(2, 0), mat(2, 1), mat(2, 2));
	}

}