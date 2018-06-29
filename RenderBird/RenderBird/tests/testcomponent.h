#pragma once
#include "math/vector.h"
#include "math/quaternion.h"
#include "ecs/component.h"
#include "core/typeinfo.h"
#include "ecs/componentregister.h"
using namespace MathLib;

namespace RenderBird
{
	struct Transform : public Component<Transform>
	{
		Vector3f m_position;
		Quaternion m_rotation;
		Vector3f m_scale;
		bool m_needUpdate;
	};

	struct TestComponent : public Component<TestComponent>
	{
		TestComponent()
		{
			m_str = "123";
		}
		~TestComponent()
		{
			m_str = "";
		}
		std::string m_str;
	};


	//template <typename T> struct CompType;

	//template <> struct TCompReg<Transform>
}
