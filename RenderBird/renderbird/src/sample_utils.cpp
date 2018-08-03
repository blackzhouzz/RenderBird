#include "sample_utils.h"
#include "entitymanager.h"
#include "disk.h"

namespace RenderBird
{
	Vector2f SampleUtils::ConcentricSampleDisk(const Vector2f &u)
	{
		// Map uniform random numbers to $[-1,1]^2$
		Vector2f offset = u * 2.0 - Vector2f(1, 1);

		// Handle degeneracy at the origin
		if (offset.x == 0 && offset.y == 0) return Vector2f(0, 0);

		// Apply concentric mapping to point
		Float theta, r;
		if (std::abs(offset.x) > std::abs(offset.y)) 
		{
			r = offset.x;
			theta = C_QUARTER_PI * (offset.y / offset.x);
		}
		else 
		{
			r = offset.y;
			theta = C_QUARTER_PI - C_QUARTER_PI * (offset.x / offset.y);
		}
		return Vector2f(std::cos(theta), std::sin(theta)) * r;
	}

	void SampleUtils::SampleDiskLight(Disk* disk, const Matrix4f& objToWorld, const Vector2f& randuv, LightSample& lightSample)
	{
		Vector2f pd = ConcentricSampleDisk(randuv);
		Vector3f objPos(pd.x * disk->m_radius, pd.y * disk->m_radius, disk->m_height);
		lightSample.m_normal = objToWorld.TransformDirection(Vector3f::UNIT_Z).GetNormalized();
		lightSample.m_position = objToWorld.TransformPoint(objPos);
		lightSample.m_pdf = 1 / DiskUtils::GetArea(disk);
	}
}