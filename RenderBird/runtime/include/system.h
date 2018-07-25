#pragma once
#include "runtime_private.h"

namespace Runtime
{
	class System : NonCopyable
	{
	public:
		virtual void Initialize() {}
		virtual void Destroy(){}
		virtual void Update() {}
	};
}