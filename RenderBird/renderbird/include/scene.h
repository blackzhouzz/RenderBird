#pragma once
#include "object.h"
#include "math/ray.h"
#include "rayhitinfo.h"

namespace RenderBird
{
	class Scene : public Object
	{
	public:
		void Intersect(const Ray& ray, RayHitInfo* hitInfo);
	};
}