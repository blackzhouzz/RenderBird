#pragma once
#include "RenderbirdPrivate.h"
#include "TriangleMesh.h"

namespace RenderBird
{
	struct MeshComponent
	{
		TriangleMesh* m_trimesh;
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, MeshComponent);
DEFAULT_BEGIN(RenderBird, MeshComponent)
DEFAULT_DATA(nullptr)
DEFAULT_END()
