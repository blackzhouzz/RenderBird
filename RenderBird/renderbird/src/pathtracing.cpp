#include "pathtracing.h"
#include "renderer.h"
#include "arealight.h"
#include "bsdf.h"
#include "light_component.h"
#include "arealight.h"

namespace RenderBird
{
	PathTracing::PathTracing(Renderer* renderer)
		: m_renderer(renderer)
	{

	}

	void PathTracing::Integrate(State* state, Radiance* L)
	{
		Ray ray;
		m_renderer->GenerateCameraRay(state->m_cameraSample, &ray);
		const int maxBounce = m_renderer->GetRendererSetting().m_maxBounce;
		//path tracing loop begin
		RayHitInfo hitInfo;
		m_renderer->m_scene->Intersect(ray, &hitInfo);

		for (state->m_currentBounce = 0; ;state->m_currentBounce++)
		{
			if (!hitInfo.IsHit() || state->m_currentBounce > maxBounce)
			{
				break;
			}

			SurfaceSample ss(ray, hitInfo);

			if (m_renderer->m_scene->IsLight(hitInfo.m_id))
			{
				L->m_directDiffuse += state->m_throughtput * AreaLightUtils::Le(hitInfo.m_id, &ss, -ray.m_direction);
			}

			//RGBA32 diffuseColor = RGBA32::WHITE;
			//if (hitInfo.m_material != nullptr)
			//{
			//	diffuseColor = hitInfo.m_material->m_diffuseColor;
			//}
			//Float nol = std::max(Vector3f::DotProduct(hitInfo.m_n.Normalized(), Vector3f(1, 0, 1).Normalized()), 0.0);
			//RGB32 color = RGB32(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			//L->m_directDiffuse = color;
			//break;
			
			SampleLight(state, &ss, L);
			BsdfSpectrum bs;
			Vector2f rand2d = state->m_sampler->Random2D();
			RGB32 sampleWeight;
			Vector3f wi;
			Float bsdfPdf = 0.0;

			ss.m_bsdf->Sample(&ss, rand2d, &wi, &bsdfPdf, &sampleWeight);

			if (bsdfPdf == 0.0 || sampleWeight.IsZero())
				break;

			ray.m_origin = ss.m_pos;
			ray.m_direction = wi;

			bool hitLight = false;

			if (m_renderer->m_scene->Intersect(ray, &hitInfo))
			{
				if (m_renderer->m_scene->IsLight(hitInfo.m_id))
				{
					hitLight = true;
				}
			}
			else
			{
				break;
			}
			state->m_throughtput *= sampleWeight / bsdfPdf;

			if (hitLight)
			{
				auto li = AreaLightUtils::Le(hitInfo.m_id, &ss, -ray.m_direction);
				Float lightPdf = AreaLightUtils::PdfDisk(hitInfo, &ss, wi);
				Float misWeight = SampleUtils::PowerHeuristic(bsdfPdf, lightPdf);
				L->m_directDiffuse += state->m_throughtput * li * misWeight;
			}
		}
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
			RGB32 eval;
			ss->m_bsdf->Eval(ss, ls.m_wi, &bsdfPdf, &eval);
			if (bsdfPdf == 0.0)
				return false;

			Ray lightRay(ss->m_pos, ls.m_wi);
			RayHitInfo lightHitInfo;
			if (m_renderer->m_scene->Intersect(lightRay, &lightHitInfo))
			{
				if (lightHitInfo.m_id != lightId)
				{
					return false;
				}
			}

			bs.Add(eval);
			bs.Mul(ls.m_li / lightPdf);

			if (!bs.IsZero())
			{
				Float misWeight = SampleUtils::PowerHeuristic(lightPdf, bsdfPdf);
				L->m_directDiffuse += state->m_throughtput * bs.m_diffuse * misWeight;
				//AccumRadiance(state, &bs, L);
				return true;
			}
		}
		return false;
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
		if (pixelX == 126 && pixelY == 41)
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