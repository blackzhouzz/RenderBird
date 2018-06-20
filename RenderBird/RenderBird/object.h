#pragma once
#include "coreprivate.h"
#include "typeinfo.h"

namespace RenderBird
{
	class Object
	{
		DECLARE_TYPEINFO_BASE(Object)
	public:
		const std::string& GetName()const { return m_name; }
	private:
		std::string m_name;
		uint m_id;
	};
	typedef std::unique_ptr<Object> ObjectPtr;
}