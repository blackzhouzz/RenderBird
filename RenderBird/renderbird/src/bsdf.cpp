#include "BSDF.h"

namespace RenderBird
{
	BSDF::BSDF()
		: m_color(RGB32::WHITE * 0.5)
		, m_doubleSide(true)
		, m_flags(0)
	{
	}

	Vector3f BSDF::LocalToWorld(const Vector3f &v) const
	{
		return m_frame.toGlobal(v);
	}

	Vector3f BSDF::WorldToLocal(const Vector3f &v) const
	{
		return m_frame.toLocal(v);
	}
}