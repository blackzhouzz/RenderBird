#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"

namespace RenderBird
{
	class SceneObject
	{
	public:
		SceneObject();
		virtual void OnCreate(EntityId id);
		virtual void OnDestroy() {}
		virtual bool IsLight()const { return false; }
		virtual bool Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const { return false; }
		virtual Vector2f GetDistanceNearFar(const Vector3f& normal) { return Vector2f(C_FLOAT_MAX, -C_FLOAT_MAX); }
		virtual void UpdateBoundingBox() {}
		const BoundingBox& GetBoundingBox()const { return m_boundingBox; }
		virtual bool CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const { return false; }
		Transform* GetTransform()const { return m_transform; }
	protected:
		EntityId m_id;
		Transform* m_transform;
		BoundingBox m_boundingBox;
	};

	template<typename T>
	struct SceneObjectFactory
	{
		static T* CreateSceneObjectImpl()
		{
			return new T();
		}
	};

	template<typename T>
	inline T* CreateSceneObject(EntityId id)
	{
		T* obj = SceneObjectFactory<T>::CreateSceneObjectImpl();
		obj->OnCreate(id);
		return obj;
	}
}