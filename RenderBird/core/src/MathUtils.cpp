#include "MathUtils.h"

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

	bool MathUtils::RayTriangleIntersect(
		Vector3f ray_P, Vector3f ray_dir, Float ray_t,
		const Vector3f v0, const Vector3f v1, const Vector3f v2,
		Float *isect_u, Float *isect_v, Float *isect_t, bool doubleSide)
	{
		Vector3f edge1 = v1 - v0;
		Vector3f edge2 = v2 - v0;

		// Begin calculating determinant; also used to calculate U parameter

		Vector3f pvec = Vector3f::CrossProduct(ray_dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle

		const Float det = Vector3f::DotProduct(edge1, pvec);
		Float u, v, t;
		if (doubleSide)
		{
			// No backface culling in this experiment, determinant within "epsilon" as
			// defined in M&T paper is considered 0

			if (det > -C_FLOAT_EPSILON && det < C_FLOAT_EPSILON)
				return false;

			const Float inv_det = 1.0f / det;

			// Calculate vector from vertex to ray origin

			Vector3f tvec = ray_P - v0;

			// Calculate U parameter and test bounds
			u = Vector3f::DotProduct(tvec, pvec) * inv_det;
			if (u < 0.0f || u > 1.0f)
				return false;

			// Prepare to test V parameter

			Vector3f qvec = Vector3f::CrossProduct(tvec, edge1);

			// Calculate V parameter and test bounds

			v = Vector3f::DotProduct(ray_dir, qvec) * inv_det;
			//sub epsilon solve z fighting
			if (v < 0.0f || u + v >= 1.0f - C_FLOAT_EPSILON)
				return false;

			// Calculate t, final check to see if ray intersects triangle. Test to
			// see if t > tFar added for consistency with other algorithms in experiment.

			t = Vector3f::DotProduct(edge2, qvec) * inv_det;
			if (t <= C_FLOAT_EPSILON || t >= ray_t)
				return false;
		}
		else
		{
			if (det < C_FLOAT_EPSILON)
				return false;
			Vector3f tvec = ray_P - v0;

			u = Vector3f::DotProduct(tvec, pvec);
			if (u < 0.0f || u > det)
				return false;

			Vector3f qvec = Vector3f::CrossProduct(tvec, edge1);

			v = Vector3f::DotProduct(ray_dir, qvec);
			if (v < 0.0f || u + v > det)
				return false;

			t = Vector3f::DotProduct(edge2, qvec);
			const Float inv_det = 1.0f / det;

			t *= inv_det;
			u *= inv_det;
			v *= inv_det;
		}
		*isect_u = u;
		*isect_v = v;
		*isect_t = t;

		return true;
	}
}