#pragma once
#include "renderbird_private.h"
#include "disk.h"

namespace RenderBird
{
	struct LightSample
	{
		Vector3f m_position;
		Vector3f m_normal;
		Vector3f m_direction;
		Float m_distance;
		Vector2f m_uv;
		Float m_pdf;
	};

	class SampleUtils
	{
	public:
		static Vector2f ConcentricSampleDisk(const Vector2f &u);
		static void SampleDiskLight(Disk* disk, const Matrix4f& objToWorld, const Vector2f& randuv, LightSample& lightSample);

	};
}