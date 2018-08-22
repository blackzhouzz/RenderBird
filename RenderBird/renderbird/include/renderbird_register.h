#pragma once
#include "renderbird_private.h"

TYPE_ID_GROUP(RenderBird, 1000)
TYPE_ID_CLASS(RenderBird, TriangleMesh, 2)
TYPE_ID_STRUCT(RenderBird, LightProperty, 3)
TYPE_ID_STRUCT(RenderBird, DistantLightComponent, 4)
TYPE_ID_STRUCT(RenderBird, MeshComponent, 5)
TYPE_ID_STRUCT(RenderBird, CameraComponent, 6)

extern void RegisterTypeInfoRenderBird();
