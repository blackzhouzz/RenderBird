#pragma once
#include "object.h"

namespace RenderBird
{
	class ResourceManager
	{
	public:
		Object* LoadObject();
		void UnloadObject(Object* obj);
	};
}