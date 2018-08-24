#include "pathtracing.h"
#include "renderer.h"
#include "arealight.h"
#include "bsdf.h"
#include "light_component.h"

namespace RenderBird
{
	PathTracing::PathTracing(Renderer* renderer)
		: m_renderer(renderer)
	{

	}

	void PathTracing::InitSurfaceData(State* state, SurfaceSample* ss, const Ray& ray, const RayHitInfo& hitInfo)
	{
		ss->m_position = hitInfo.m_position;
		ss->m_normal = hitInfo.m_normal;
		ss->m_geomNormal = hitInfo.m_geomNormal;
		if (ss->m_normal == C_Zero_v3f)
		{
			ss->m_normal = ss->m_geomNormal;
		}

		ss->m_wo = -ray.m_direction;

		//ss->m_isBackfacing = (Vector3f::DotProduct(ss->m_geomNormal, ss->m_wo) < 0.0f);
		//if (ss->m_isBackfacing)
		//{
		//	ss->m_normal = -ss->m_normal;
		//	ss->m_geomNormal = -ss->m_geomNormal;
		//}
	}

	void PathTracing::Integrate(State* state, Radiance* L)
	{
		Ray ray;
		m_renderer->GenerateCameraRay(state->m_cameraSample, &ray);
		const int maxBounce = m_renderer->GetRendererSetting().m_maxBounce;
		//path tracing loop begin

		for (state->m_currentBounce = 0; ;state->m_currentBounce++)
		{
			RayHitInfo hitInfo;
			bool isHit = m_renderer->m_scene->Intersect(ray, &hitInfo);
			if (!isHit || state->m_currentBounce > maxBounce)
			{
				break;
			}
			if (state->m_currentBounce > 0)
				state->m_currentBounce = state->m_currentBounce;

			SurfaceSample ss;
			ss.m_bsdf = new DiffuseBSDF();
			InitSurfaceData(state, &ss, ray, hitInfo);

			//RGBA32 diffuseColor = RGBA32::WHITE;
			//if (hitInfo.m_material != nullptr)
			//{
			//	diffuseColor = hitInfo.m_material->m_diffuseColor;
			//}
			//Float nol = std::max(Vector3f::DotProduct(hitInfo.m_normal.Normalized(), Vector3f(1, 0, 1).Normalized()), 0.0);
			//RGB32 color = RGB32(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			//L->m_directDiffuse = color;
			//break;
			
			SampleLight(state, &ss, L);
			break;
			if (!SurfaceBounce(state, &ss, ray))
				break;

			//if (ProbabilityStop())
			//	break;
		}
	}

	void PathTracing::EvalBSDF(State* state, SurfaceSample* ss, const Vector3f& wi, Float* pdf, BsdfSpectrum* bs)
	{
		ss->m_bsdf->Eval(ss, wi, pdf, bs);
	}

	bool PathTracing::SampleLight(State* state, SurfaceSample* ss, Radiance* L)
	{
		Vector2f lightRand2d = state->m_sampler->Random2D();
		EntityId lightId = m_renderer->m_scene->m_lightId;
		LightSample ls;
		Float lightPdf = 0;
		if (AreaLightUtils::SampleDisk(lightId, lightRand2d, ss, &ls, &lightPdf) && lightPdf > 0)
			//if (DistantLightComponentUtils::Sample(lightId, lightRand2d, ss, &ls, &lightPdf) && lightPdf > 0)
		{
			BsdfSpectrum bs;
			Float bsdfPdf = 0.0;
			EvalBSDF(state, ss, ls.m_wi, &bsdfPdf, &bs);
			Float weight = 1.0;
			if (state->m_useMis)
			{
				weight = SampleUtils::PowerHeuristic(lightPdf, bsdfPdf);
			}

			bs.Mul(ls.m_li / lightPdf * weight);

			if (!bs.IsZero())
			{
				AccumRadiance(state, &bs, L);
				return true;
			}
		}
		return false;
	}

	bool PathTracing::SurfaceBounce(State* state, SurfaceSample* ss, Ray& ray)
	{
		Vector2f rand2d = state->m_sampler->Random2D();
		RGB32 bdsf;
		Vector3f wi;
		Float bsdfPdf = 0.0;

		ss->m_bsdf->Sample(ss, rand2d, &bsdfPdf, &wi, &bdsf);

		if (bsdfPdf == 0.0 || bdsf.IsZero())
			return false;

		BsdfSpectrum bs;
		EvalBSDF(state, ss, wi, &bsdfPdf, &bs);
		if (bsdfPdf == 0.0)
			return false;
		state->m_throughtput *= bs.m_diffuse / bsdfPdf;
		if (state->m_throughtput.IsZero())
			return false;
		ray.m_origin = ss->m_position;
		ray.m_direction = wi;

		return true;
	}

	void PathTracing::AccumRadiance(State* state, BsdfSpectrum* bs, Radiance* L)
	{
		L->m_directDiffuse += state->m_throughtput * bs->m_diffuse;
	}

	bool PathTracing::ProbabilityStop()
	{
		return false;
	}

	void PathTracing::InitState(int pixelX, int pixelY, State* state)
	{
		auto setting = m_renderer->GetRendererSetting();
		state->m_pixelX = pixelX;
		state->m_pixelY = pixelY;
		state->m_sampler = new Sampler(setting.m_numSamples);
		state->m_useMis = setting.m_useMis;
	}

	void PathTracing::Render(int pixelX, int pixelY, TileRenderer* tile)
	{
		if (pixelX == 23 && pixelY == 26)
		{
			pixelX = pixelX;
		}
		State state;
		InitState(pixelX, pixelY, &state);
		for (uint32 samplerIndex = 0; samplerIndex < state.m_sampler->m_numSamplers; ++samplerIndex)
		{
			Radiance L;
			state.m_cameraSample = state.m_sampler->GetCameraSample(pixelX, pixelY);
			state.m_curSamplerIndex = samplerIndex;
			state.m_throughtput = RGB32::WHITE;
			state.m_currentBounce = 0;

			Integrate(&state, &L);
			if (!L.IsZero())
			{
				L = L;
			}
			m_renderer->AddSample(state.m_cameraSample.m_pixel, tile->GetBoundMin(), tile->GetBoundMax(), L);
		}
	}
}