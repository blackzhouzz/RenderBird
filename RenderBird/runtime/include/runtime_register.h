#pragma once
#include "runtime_private.h"

TYPE_ID_GROUP(Runtime, 500)
TYPE_ID_ITEM(Object, Runtime, 1)
TYPE_ID_ITEM(ComponentBase, Runtime, 2)
TYPE_ID_ITEM(Transform, Runtime, 3)
TYPE_ID_ITEM(TestComponent, Runtime, 4)

extern void RegisterTypeInfoRuntime();
