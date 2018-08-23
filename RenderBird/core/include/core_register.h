#pragma once
#include "core_private.h"

TYPE_ID_GROUP(Core, 500)
TYPE_ID_CLASS(Core, Object, 1)
TYPE_ID_STRUCT(Core, Transform, 2)
TYPE_ID_STRUCT(Core, TransformParent, 3)

extern void RegisterTypeInfoCore();
