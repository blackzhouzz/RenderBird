#pragma once

namespace RenderBird
{
	enum GeometryType
	{
		GT_Mesh,
		GT_Box,
		GT_Sphere,
		GT_Triangle,
		GT_Plane,
	};

	class Geometry
	{
	public:
		virtual GeometryType GetGeometryType() = 0;
	};
}