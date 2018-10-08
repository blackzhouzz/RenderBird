#include "pathtracing.h"
#include "renderer.h"
#include "AreaLightComponent.h"
#include "BSDF.h"
#include "LightComponent.h"
#include "AreaLightComponent.h"
#include "RenderStatistic.h"

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
		const int rrBounce = m_renderer->GetRendererSetting().m_rrBounce;
		if (m_renderer->m_scene->m_lights.size() == 0)
			return;
		RayHitInfo hitInfo;
		m_renderer->m_scene->Intersect(ray, &hitInfo);

		for (state->m_currentBounce = 0; ;state->m_currentBounce++)
		{
			if (state->m_currentBounce > RenderStatistic::m_maxDepth)
				RenderStatistic::m_maxDepth = state->m_currentBounce;
			if (!hitInfo.IsHit() || state->m_currentBounce >= maxBounce)
			{
				break;
			}
			RenderStatistic::m_numSampleProcessed++;
			SurfaceSample ss(ray, hitInfo);

			if (hitInfo.m_obj->IsLight() && state->m_currentBounce == 0)
			{
				const Light* light = static_cast<const Light*>(hitInfo.m_obj);
				L->m_directDiffuse += state->m_throughtput * light->Le(&ss, -ray.m_direction);
			}
			Float sampleLightPdf = 0.0f;
			Light* sampleLight = GetSampleLight(state, sampleLightPdf);
			SampleLight(state, sampleLight, sampleLightPdf, &ss, L);

			BsdfSpectrum bs;
			Vector2f rand2d = state->m_sampler->Random2D();
			RGB32 bsdfWeight = RGB32::BLACK;
			Vector3f wi;
			Float bsdfPdf = 0.0;

			ss.m_bsdf->Sample(&ss, rand2d, &wi, &bsdfPdf, &bsdfWeight);

			if (bsdfPdf == 0.0 || bsdfWeight.Lum() < C_FLOAT_EPSILON)
				break;

			state->m_throughtput *= bsdfWeight / bsdfPdf;

			ray.m_origin = ss.m_pos;
			ray.m_direction = wi;

			bool hitLight = false;

			RayHitInfo tempHitInfo;
			if (m_renderer->m_scene->Intersect(ray, &tempHitInfo))
			{
				if (tempHitInfo.m_obj->IsLight()
					&& sampleLight == tempHitInfo.m_obj
					)
				{
					hitLight = true;
				}
			}
			else
			{
				break;
			}

			if (hitLight)
			{
				const Light* light = static_cast<const Light*>(tempHitInfo.m_obj);
				Float sampleLightPdf = (*m_renderer->m_scene->m_distribution)[light->m_index];
				Float lightPdf = light->Pdf(tempHitInfo, &ss, wi);
				auto li = light->Le(&ss, -ray.m_direction);

				Float misWeight = SampleUtils::PowerHeuristic(bsdfPdf, lightPdf);
				L->m_directDiffuse += state->m_throughtput * li * misWeight / sampleLightPdf;
			}
			const Float eta = 1.0f;

			if (state->m_currentBounce >= rrBounce)
			{
				Float q = std::min(state->m_throughtput.Max() * eta * eta, (Float) 0.95f);
				if (state->m_sampler->Random1D() >= q)
					break;
				state->m_throughtput /= q;
			}
			hitInfo = tempHitInfo;
			//break;
		}
	}

	Light* PathTracing::GetSampleLight(State* state, Float& sampleLightPdf)
	{
		Float rand1d = state->m_sampler->Random1D();
		//random select light and get sample light pdf
		size_t index = m_renderer->m_scene->m_distribution->Sample(rand1d, sampleLightPdf);
		Light* light = m_renderer->m_scene->m_lights[index];
		return light;
	}

	bool PathTracing::SampleLight(State* state, Light* light, Float sampleLightPdf, SurfaceSample* ss, Radiance* L)
	{
		Vector2f lightRand2d = state->m_sampler->Random2D();

		LightSample ls;
		Float lightPdf = 0;
		if (light->Sample(lightRand2d, ss, &ls, &lightPdf))
		{
			if (lightPdf > 0)
			{
				BsdfSpectrum bs;
				Float bsdfPdf = 0.0;
				RGB32 eval;
				ss->m_bsdf->Eval(ss, ls.m_wi, &bsdfPdf, &eval);
				if (bsdfPdf == 0.0)
					return false;

				bool shadowBlocked = false;
				Ray lightRay(ss->m_pos, ls.m_wi);
				RayHitInfo lightHitInfo;
				if (m_renderer->m_scene->Intersect(lightRay, &lightHitInfo))
				{
					if (lightHitInfo.m_obj != light)
					{
						shadowBlocked = true;
					}
				}

				bs.Add(eval);
				bs.Mul(ls.m_li / lightPdf);

				if (!shadowBlocked)
				{
					Float misWeight = SampleUtils::PowerHeuristic(lightPdf, bsdfPdf);
					L->m_directDiffuse += state->m_throughtput * bs.m_diffuse * misWeight / sampleLightPdf;
					return true;
				}
			}
		}
		return false;
	}

	void PathTracing::Render(int pixelX, int pixelY, TileRenderer* tile)
	{
		if (pixelX == 109 && pixelY == 113)
		{
			pixelX = pixelX;
		}
		State state;
		auto setting = m_renderer->GetRendererSetting();
		state.m_pixelX = pixelX;
		state.m_pixelY = pixelY;
		state.m_sampler = new Sampler(setting.m_numSamples);
		state.m_useMis = setting.m_useMis;

		for (uint32 samplerIndex = 0; samplerIndex < state.m_sampler->m_numSamplers; ++samplerIndex)
		{
			Radiance L;
			state.m_cameraSample = state.m_sampler->GetCameraSample(pixelX, pixelY);

			state.m_curSamplerIndex = samplerIndex;
			state.m_throughtput = RGB32::WHITE;
			state.m_currentBounce = 0;

			Integrate(&state, &L);

			m_renderer->AddSample(state.m_cameraSample.m_pixel, tile->GetBoundMin(), tile->GetBoundMax(), L);
		}

		delete state.m_sampler;
	}
}