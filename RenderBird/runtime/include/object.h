#pragma once
#include "runtime_private.h"
#include "runtime_register.h"
#include "typeinfo.h"

namespace Runtime
{
	class Object
	{
		//DECLARE_TYPEINFO_BASE(Object)
	};
	typedef std::unique_ptr<Object> ObjectPtr;
}

DECLEAR_TYPE_BASE(Runtime, Object);