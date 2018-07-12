#pragma once
#include "ecs/system.h"
#include "math/ray.h"
#include "rayhitinfo.h"
using namespace Runtime;
using namespace Core;

namespace RenderBird
{
	class RaycastSys : System
	{
	public:
		bool Raycast(const Ray& ray, RayHitInfo& hitInfo);
	};
}