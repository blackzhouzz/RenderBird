#include "light.h"

IMPLEMENT_TYPE(RenderBird, Light)

namespace RenderBird
{
	Light::Light()
		: m_type(LightType::Directional)
	{

	}
}