#include "PathTracing.h"
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
		auto pixel = Vector2u(state->m_pixelX, state->m_pixelY);
		Ray ray;
		m_renderer->GenerateCameraRay(state->m_cameraSample.m_pixel.x, state->m_cameraSample.m_pixel.y, &ray);
		const int maxBounce = m_renderer->GetRendererSetting().m_maxBounce;
		const int rrBounce = m_renderer->GetRendererSetting().m_rrBounce;
		if (m_renderer->m_scene->m_lights.size() == 0)
			return;

		bool recordedOutputValues = false;

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

			if (!recordedOutputValues)
			{
				if (m_renderer->m_buffers._depthBuffer != nullptr)
					m_renderer->m_buffers._depthBuffer->addSample(pixel, hitInfo.m_t);
				if (m_renderer->m_buffers._normalBuffer != nullptr)
					m_renderer->m_buffers._normalBuffer->addSample(pixel, hitInfo.m_ng);
				if (m_renderer->m_buffers._albedoBuffer != nullptr)
				{
					Vector3f albedo = C_Zero_v3f;
					SurfaceSample ss(ray, hitInfo);

					if (ss.m_bsdf != nullptr)
					{
						RGB32 color = ss.m_bsdf->Albedo();
						for (int i = 0; i < 3; ++i)
							albedo[i] = color[i];
						m_renderer->m_buffers._albedoBuffer->addSample(pixel, albedo);
					}
				}
				recordedOutputValues = true;
			}

			RenderStatistic::m_numSampleProcessed++;
			SurfaceSample ss(ray, hitInfo);

			if (hitInfo.m_obj->IsLight() && state->m_currentBounce == 0)
			{
				const Light* light = static_cast<const Light*>(hitInfo.m_obj);
				L->m_directEmission += state->m_throughtput * light->Le(&ss, -ray.m_direction);
			}

			Float sampleLightPdf = 0.0f;
			Light* sampleLight = GetSampleLight(state, sampleLightPdf);
			SampleLight(state, sampleLight, sampleLightPdf, &ss, L);

			LightSpectrum lightSpectrum;
			Vector2f rand2d = state->m_sampler->Get2D(SamplerDimension::BSDFSampleU, SamplerDimension::BSDFSampleV);
			Vector3f wi;
			Float bsdfPdf = 0.0;

			ss.m_bsdf->Sample(&ss, rand2d, &wi, &bsdfPdf, &lightSpectrum);

			if (bsdfPdf == 0.0)
				break;
			auto newThroughtput = state->m_throughtput * lightSpectrum.Resolve() / bsdfPdf;

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

				lightSpectrum.Mul(state->m_throughtput * misWeight * li / (bsdfPdf * sampleLightPdf));
				L->Accum(&lightSpectrum, state->m_currentBounce == 0);
			}
			state->m_throughtput = newThroughtput;

			const Float eta = 1.0f;

			if (state->m_currentBounce >= rrBounce)
			{
				Float q = std::min(state->m_throughtput.Max() * eta * eta, (Float) 0.95f);
				if (state->m_sampler->Get1D(SamplerDimension::RussianRoulette) >= q)
					break;
				state->m_throughtput /= q;
			}
			hitInfo = tempHitInfo;
			//break;
		}
		if (!recordedOutputValues)
		{
			if (state->m_currentBounce == 0)
			{
				if (m_renderer->m_buffers._depthBuffer != nullptr)
					m_renderer->m_buffers._depthBuffer->addSample(pixel, 0.0f);
			}

			if (m_renderer->m_buffers._normalBuffer != nullptr)
				m_renderer->m_buffers._normalBuffer->addSample(pixel, -ray.m_direction);
			//if (m_renderer->m_buffers._albedoBuffer && info.primitive && info.primitive->isInfinite())
			//	m_renderer->m_buffers._albedoBuffer->addSample(pixel, info.primitive->evalDirect(data, info));
		}
	}

	Light* PathTracing::GetSampleLight(State* state, Float& sampleLightPdf)
	{
		Float rand1d = state->m_sampler->Get1D(SamplerDimension::LightIndex);
		//random select light and get sample light pdf
		size_t index = m_renderer->m_scene->m_distribution->Sample(rand1d, sampleLightPdf);
		Light* light = m_renderer->m_scene->m_lights[index];
		return light;
	}

	bool PathTracing::SampleLight(State* state, Light* light, Float sampleLightPdf, SurfaceSample* ss, Radiance* L)
	{
		Vector2f lightRand2d = state->m_sampler->Get2D(SamplerDimension::LightSampleU, SamplerDimension::LightSampleV);

		LightSample ls;
		Float lightPdf = 0;
		if (light->Sample(lightRand2d, ss, &ls, &lightPdf))
		{
			if (lightPdf > 0)
			{
				LightSpectrum lightSpectrum;
				Float bsdfPdf = 0.0;
				ss->m_bsdf->Eval(ss, ls.m_wi, &bsdfPdf, &lightSpectrum);
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


				if (!shadowBlocked)
				{
					Float misWeight = SampleUtils::PowerHeuristic(lightPdf, bsdfPdf);
					lightSpectrum.Mul(state->m_throughtput * misWeight * ls.m_li / (lightPdf * sampleLightPdf));
					L->Accum(&lightSpectrum, state->m_currentBounce == 0);
					return true;
				}
			}
		}
		return false;
	}

	void PathTracing::Render(int pixelX, int pixelY, TileRenderer* tile)
	{
		//if (pixelX == 109 && pixelY == 113)
		//{
		//	pixelX = pixelX;
		//}
		State state;
		auto setting = m_renderer->GetRendererSetting();
		state.m_pixelX = pixelX;
		state.m_pixelY = pixelY;
		state.m_sampler = tile->m_sampler.get();
		state.m_useMis = setting.m_useMis;
		state.m_sampler->StartSample(pixelX, pixelY);

		for (uint32 samplerIndex = 0; samplerIndex < state.m_sampler->GetNumSamplers(); ++samplerIndex)
		{
			Radiance L;

			state.m_cameraSample = state.m_sampler->GetCameraSample(pixelX, pixelY);
			state.m_curSamplerIndex = samplerIndex;
			state.m_throughtput = RGB32::WHITE;
			state.m_currentBounce = 0;

			Integrate(&state, &L);

			state.m_sampler->NextSample();

			auto color = L.Resolve();
			Float sum = std::abs(color[0]) + std::abs(color[1]) + std::abs(color[2]);
			if (IsNaN(sum) || IsInf(sum))
				continue;
			if (m_renderer->m_setting.m_enableClamp)
			{
				auto indirect = L.ResolveIndirect();
				Float sumIndirect = std::abs(indirect[0]) + std::abs(indirect[1]) + std::abs(indirect[2]);
				if (sumIndirect > m_renderer->m_setting.m_clampValue)
				{
					auto scale = m_renderer->m_setting.m_clampValue / sumIndirect;
					L.m_indirectDiffuse *= scale;
				}

				color = L.Resolve();
				if (m_renderer->m_buffers._colorBuffer != nullptr)
					m_renderer->m_buffers._colorBuffer->addSample(Vector2u(pixelX, pixelY), Vector3f(color[0], color[1], color[2]));
			}
			else
			{
				if (m_renderer->m_buffers._colorBuffer != nullptr)
					m_renderer->m_buffers._colorBuffer->addSample(Vector2u(pixelX, pixelY), Vector3f(color[0], color[1], color[2]));
			}
		}
	}
}