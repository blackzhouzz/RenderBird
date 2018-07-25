#pragma once
#include "renderbird_private.h"

TYPE_ID_GROUP(RenderBird, 1000)
TYPE_ID_CLASS(Geometry, RenderBird, 1)
TYPE_ID_CLASS(TriangleMesh, RenderBird, 2)
TYPE_ID_CLASS(Light, RenderBird, 4)

extern void RegisterTypeInfoRenderBird();
