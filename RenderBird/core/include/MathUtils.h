#pragma once
#include "MathCore.h"
#include "rect2.h"

namespace Core
{
	class MathUtils
	{
	public:
		static Matrix4f TRS(const Vector3f& position, const Quaternion& orient, const Vector3f& scale);
		static Matrix4f ScreenToRasterMatrix(const Rect2f& screenBound, const Vector2i& resolution);
		static Matrix3f MakeNormalTransform(const Vector3f& normal);
		static Matrix3f FromAxes(const Vector3f& axisX, const Vector3f& axisY, const Vector3f& axisZ);
		static Vector3f TransformDirection(const Matrix4f& trans, const Vector3f& dir);
		static Matrix4f ScaleDirectionMatrix(const Vector3f& dir, Float k);
		static Matrix4f OrthoMatrix(Float width, Float height, Float nearClip, Float farClip);
		static Matrix4f OrthoOffCenterMatrix(Float left, Float right, Float bottom, Float top, Float nearClip, Float farClip);
		static Matrix4f PerspectiveFovMatrix(Float fovY, Float aspect, Float nearClip, Float farClip);
		static Vector3f GetForward(const Quaternion& quat);
		//"Fast, Minimum Storage Ray-Triangle Intersection
		static bool RayTriangleIntersect(
			Vector3f ray_P, Vector3f ray_dir, Float ray_t,
			const Vector3f v0, const Vector3f v1, const Vector3f v2,
			Float *isect_u, Float *isect_v, Float *isect_t, bool doubleSide);
	};
}