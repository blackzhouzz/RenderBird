#include "sample_utils.h"
#include "entitymanager.h"
#include "disk.h"

namespace RenderBird
{
	//concentric mapping see [Shirley and Chiu 97]
	Vector2f SampleUtils::ToUnitDisk(const Vector2f &randuv)
	{
		Float phi, r;
		Float a = 2 * randuv.x - 1;
		Float b = 2 * randuv.y - 1;
		if (a == 0 && b == 0)
		{
			return Vector2f::ZERO;
		}
		if (a*a > b*b) 
		{
			r = a;
			phi = (C_QUARTER_PI)*(b / a);
		}
		else 
		{
			r = b;
			phi = (C_QUARTER_PI)*(a / b) + C_HALF_PI;
		}
		return Vector2f(r*cos(phi), r*sin(phi));
	}

	void SampleUtils::SampleDiskLight(Disk* disk, const Matrix4f& objToWorld, const Vector2f& randuv, LightSample& lightSample)
	{
		Vector2f pd = ToUnitDisk(randuv);
		Vector3f objPos(pd.x * disk->m_radius, pd.y * disk->m_radius, 0);
		lightSample.m_normal = objToWorld.TransformDirection(Vector3f::UNIT_Z).GetNormalized();
		lightSample.m_position = objToWorld.TransformPoint(objPos);
		lightSample.m_pdf = 1 / DiskUtils::GetArea(disk);
	}
}