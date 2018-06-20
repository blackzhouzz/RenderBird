#pragma once
#include "system.h"
#include "ray.h"
#include "rayhitinfo.h"

namespace RenderBird
{
	class RaycastSys : System
	{
	public:
		bool Raycast(const Ray& ray, RayHitInfo& hitInfo);
	};
}