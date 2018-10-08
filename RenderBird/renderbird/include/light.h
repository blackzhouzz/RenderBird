#pragma once
#include "RenderbirdPrivate.h"
#include "SceneObject.h"
#include "PathTracingCommon.h"
#include "LightComponent.h"

namespace RenderBird
{
	class Light : public SceneObject
	{
	public:
		virtual void OnCreate(EntityId id);
		virtual bool Sample(const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf) = 0;
		virtual RGB32 Le(SurfaceSample* ss, const Vector3f& w)const;
		virtual Float Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const = 0;
		virtual bool IsLight()const { return true; }
	protected:
		LightPropertyComponent* m_lightProp;
	public:
		size_t m_index;
	};

}