#pragma once
#include "object.h"
#include <unordered_map>

namespace RenderBird
{
	class ObjectManager
	{
	public:
		ObjectManager();
		~ObjectManager();
		void Initalize();
		void UnInitalize();
		uint GenerateObjectID();
	private:
		typedef std::unordered_map<uint, ObjectPtr> ObjectMap;
		ObjectMap m_objects;
	};
}