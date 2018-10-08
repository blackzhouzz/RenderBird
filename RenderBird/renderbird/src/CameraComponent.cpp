#include "CameraComponent.h"

IMPLEMENT_TYPE(RenderBird, CameraComponent)

namespace RenderBird
{
	void CameraComponentUtils::PerspectiveFovMatrix(CameraComponent* comp, Float fov, Float aspect, Float nearClip, Float farClip)
	{
		comp->m_fov = fov;
		comp->m_aspect = aspect;
		comp->m_nearClip = nearClip;
		comp->m_aspect = farClip;
		comp->m_proj = MathUtils::PerspectiveFovMatrix(DegToRad(fov), aspect, nearClip, farClip);
	}
}