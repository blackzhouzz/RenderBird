#pragma once
#include "renderbird_private.h"
#include "trianglemesh.h"

namespace RenderBird
{
	class GeometryGenerator
	{
	public:
		static TriangleMesh* GenerateBox(Float size);
		static TriangleMesh* GeneratePlane(Vector2f size);
	};
}