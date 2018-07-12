#pragma once
#include "runtime_private.h"
#include "math/ray.h"
#include "rayhitinfo.h"
#include "renderbird_register.h"
using namespace Runtime;

namespace RenderBird
{
	enum GeometryType
	{
		GT_Mesh,
		GT_Box,
		GT_Sphere,
		GT_Plane,
		GT_TriangleMesh,
	};

	class Geometry : public Object
	{
		//DECLARE_TYPEINFO(Geometry, Object)
	public:
		virtual GeometryType GetGeometryType()const = 0;
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo) = 0;
	};


}

DECLEAR_TYPE_ABSTRACT(RenderBird, Geometry, Object);