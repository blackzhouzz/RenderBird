#pragma once
#include "ecs/component.h"
#include "math/vector.h"
#include "math/quaternion.h"

namespace Runtime
{
	class Transform : public Component<Transform>
	{
	public:
		Vector3f m_position;
		Quaternion m_rotation;
		Vector3f m_scale;
		bool m_needUpdate;
	};

	class TestComponent : public Component<TestComponent>
	{
	public: 
		int m_test;
	};
}

