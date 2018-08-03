#include "pathtracing.h"
#include "renderer.h"

namespace RenderBird
{
	PathTracing::PathTracing(Renderer* renderer)
		: m_renderer(renderer)
	{
		m_setting.m_maxBounce = 3;
		m_setting.m_numSamples = 10;
		m_sampler = new Sampler(m_setting.m_numSamples);
	}

	//ccl_device_inline float3 triangle_normal(KernelGlobals *kg, ShaderData *sd)
	//{
	//	/* load triangle vertices */
	//	const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, sd->prim);
	//	const float3 v0 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
	//	const float3 v1 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
	//	const float3 v2 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));

	//	/* return normal */
	//	if (sd->object_flag & SD_OBJECT_NEGATIVE_SCALE_APPLIED) {
	//		return normalize(cross(v2 - v0, v1 - v0));
	//	}
	//	else {
	//		return normalize(cross(v1 - v0, v2 - v0));
	//	}
	//}


	void PathTracing::InitSurfaceData(SurfaceData& surfData, const Ray& ray, const RayHitInfo& hitInfo)
	{
		surfData.m_normal = hitInfo.m_normal;
		surfData.m_geomNormal = hitInfo.m_geomNormal;
		if (surfData.m_normal.IsZero())
		{
			surfData.m_normal = surfData.m_geomNormal;
		}

		surfData.m_wo = -ray.direction;

		//surfData.m_isBackfacing = (Dot(surfData.m_geomNormal, surfData.m_wo) < 0.0);
		//if (surfData.m_isBackfacing)
		//{
		//	surfData.m_normal = -surfData.m_normal;
		//	surfData.m_geomNormal = -surfData.m_geomNormal;
		//}
	}

	void PathTracing::Integrate(State& state, Radiance& Lo)
	{
		//debug
		RayHitInfo hitInfo;
		//bool hasIntersect = m_renderer->m_scene->Intersect(state.m_cameraRay, &hitInfo);
		//if (hasIntersect)
		//{
		//	RGBA32 diffuseColor = hitInfo.m_meshData->m_materialData[hitInfo.m_materialId].m_diffuseColor;
		//	RGB32 color = RGB32(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
		//	m_renderer->SetColor(state.m_pixelX, state.m_pixelY, color);
		//}
		//else
		{
			m_renderer->SetColor(state.m_pixelX, state.m_pixelY, Core::RGB32::BLACK);
		}
		Ray ray = state.m_cameraRay;
		//path tracing loop begin
		for (;;)
		{
			RayHitInfo hitInfo;
			bool isHit = m_renderer->m_scene->Intersect(ray, &hitInfo);
			if (!isHit || state.m_bounceCount > m_setting.m_maxBounce)
			{
				break;
			}

			//SurfaceData surfData;
			//InitSurfaceData(surfData, ray, hitInfo);

			//RGBA32 diffuseColor = hitInfo.m_meshData->m_materialData[hitInfo.m_materialId].m_diffuseColor;
			RGBA32 diffuseColor = RGBA32::WHITE;
			if (hitInfo.m_material != nullptr)
			{
				diffuseColor = hitInfo.m_material->m_diffuseColor;
			}
			//Float nol = Dot(surfData.m_normal, Vector3f(1, 1, 1).GetNormalized());
			RGB32 color = RGB32(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			m_renderer->SetColor(state.m_pixelX, state.m_pixelY, color);
			break;

			//if (ProbabilityTerminate(state))
			//	break;

			//SurfaceLighting(state);

			//if (!SurfaceBounce(state, ray))
			//	break;
		}

		//path tracing loop end
	}

	void PathTracing::SurfaceLighting(State& state)
	{

	}

	bool PathTracing::ProbabilityTerminate(State& state)
	{
		return false;
	}

	void PathTracing::InitState(int pixelX, int pixelY, State& state)
	{
		state.m_pixelX = pixelX;
		state.m_pixelY = pixelY;
		state.m_bounceCount = 0;
		state.m_throughtPut = RGB32::WHITE;

		m_renderer->GenerateCameraRay(pixelX, pixelY, &state.m_cameraRay);
	}

	void PathTracing::Render(int pixelX, int pixelY, Radiance& Lo)
	{
		State state;

		InitState(pixelX, pixelY, state);
		Integrate(state, Lo);
	}

	bool PathTracing::SurfaceBounce(State& state, Ray& ray)
	{
		return true;
	}
}