#pragma once
#include "core/coreprivate.h"
#include "core/typeinfo.h"

namespace RenderBird
{
	enum EObjectTypeId
	{
		TYPE_ID_ITEM(Object, 0)
	};
	class Object
	{
		DECLARE_TYPEINFO_BASE(Object)
		std::string m_name;
	};
	typedef std::unique_ptr<Object> ObjectPtr;
}