#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	enum class LightType
	{
		Directional,
		Spot,
		Point
	};


	class Light : public Object
	{
	public:
		Light();
	private:
		LightType m_type;
	};
}

DECLEAR_TYPE(RenderBird, Light, Runtime::Object);