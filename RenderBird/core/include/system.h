#pragma once
#include "CorePrivate.h"

namespace Core
{
	class System : NonCopyable
	{
	public:
		virtual void Initialize() {}
		virtual void Destroy(){}
		virtual void Update() {}
	};
}