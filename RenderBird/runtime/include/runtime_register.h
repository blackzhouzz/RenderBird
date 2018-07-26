#pragma once
#include "runtime_private.h"

TYPE_ID_GROUP(Runtime, 500)
TYPE_ID_CLASS(Object, Runtime, 1)
TYPE_ID_STRUCT(Transform, Runtime, 3)

extern void RegisterTypeInfoRuntime();
