#pragma once
#include "RenderbirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class Material : public Object
	{
	public:
		RGBA32 m_diffuseColor;
		RGBA32 m_emissiveColor;
		RGBA32 m_specularColor;
		Float m_glossiness;
		Float m_opacity;
		uint32 m_flags;
		BSDF* m_bsdf;
	};
}

DECLEAR_TYPE(RenderBird, Material, Core::Object);
