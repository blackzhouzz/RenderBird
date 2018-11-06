#pragma once
#include "RenderbirdPrivate.h"
#include "SceneObject.h"
#include "PathTracingCommon.h"
#include "LightComponent.h"
#include "Sampler.h"

namespace RenderBird
{
	class Light : public SceneObject
	{
	public:
		virtual void OnCreate(EntityId id);
		virtual bool Sample(Sampler* sampler, SurfaceSample* ss, LightSample* ls) = 0;
		virtual RGB32 Le(SurfaceSample* ss, const Vector3f& w)const;
		virtual Float Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const = 0;
		virtual bool IsLight()const { return true; }
		virtual Shape* GetShape()const { return nullptr; }
	protected:
		LightPropertyComponent* m_lightProp;
	public:
		size_t m_index;
	};

}