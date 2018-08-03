#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	struct CameraComponent
	{
		Float m_fov;
		Float m_aspect;
		Float m_nearClip;
		Float m_farClip;
		Matrix4f m_proj;
	};

	struct CameraComponentUtils
	{
		static void PerspectiveFovMatrix(CameraComponent* comp, Float fov, Float aspect, Float nearClip, Float farClip);
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, CameraComponent);
DEFAULT_BEGIN(RenderBird, CameraComponent)
DEFAULT_DATA(60.0)
DEFAULT_DATA(1.0)
DEFAULT_DATA(1e-2)
DEFAULT_DATA(1000)
DEFAULT_DATA(Matrix4f::IDENTITY)
DEFAULT_END()
