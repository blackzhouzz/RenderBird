#pragma once
#include "ecs/system.h"
#include "math/ray.h"
#include "raytracing/rayhitinfo.h"

namespace RenderBird
{
	class RaycastSys : System
	{
	public:
		bool Raycast(const Ray& ray, RayHitInfo& hitInfo);
	};
}