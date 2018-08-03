#pragma once
#include "renderbird_private.h"

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
	};
}

DECLEAR_TYPE(RenderBird, Material, Core::Object);
