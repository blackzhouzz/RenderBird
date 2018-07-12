#pragma once
#include "renderbird_private.h"

TYPE_ID_GROUP(RenderBird, 1000)
TYPE_ID_ITEM(Geometry, RenderBird, 1)
TYPE_ID_ITEM(TriangleMesh, RenderBird, 2)

extern void RegisterTypeInfoRenderBird();
