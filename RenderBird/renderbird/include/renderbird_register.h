#pragma once
#include "renderbird_private.h"

TYPE_ID_GROUP(RenderBird, 1000)
TYPE_ID_CLASS(RenderBird, Material, 1)
TYPE_ID_CLASS(RenderBird, Texture, 2)
TYPE_ID_CLASS(RenderBird, TriangleMesh, 3)
TYPE_ID_STRUCT(RenderBird, LightPropertyComponent, 101)
TYPE_ID_STRUCT(RenderBird, DistantLightComponent, 102)
TYPE_ID_STRUCT(RenderBird, MeshComponent, 103)
TYPE_ID_STRUCT(RenderBird, CameraComponent, 104)
TYPE_ID_STRUCT(RenderBird, DiskComponent, 105)
TYPE_ID_STRUCT(RenderBird, AreaLightComponent, 106)
TYPE_ID_STRUCT(RenderBird, SphereComponent, 107)
TYPE_ID_STRUCT(RenderBird, TriangleComponent, 108)


extern void RegisterTypeInfoRenderBird();
