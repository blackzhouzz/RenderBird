#include "pathtracing.h"
#include "renderer.h"
#include "area_light_component.h"
#include "bsdf.h"
#include "light_component.h"
#include "area_light_component.h"

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
		//path tracing loop begin
		RayHitInfo hitInfo;
		m_renderer->m_scene->Intersect(ray, &hitInfo);

		for (state->m_currentBounce = 0; ;state->m_currentBounce++)
		{
			if (!hitInfo.IsHit() || state->m_currentBounce >= maxBounce)
			{
				break;
			}

			SurfaceSample ss(ray, hitInfo);

			if (hitInfo.m_obj->IsLight())
			{
				const Light* light = static_cast<const Light*>(hitInfo.m_obj);
				L->m_directDiffuse += state->m_throughtput * light->Le(&ss, -ray.m_direction);
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
			RGB32 bsdfWeight = RGB32::BLACK;
			Vector3f wi;
			Float bsdfPdf = 0.0;

			ss.m_bsdf->Sample(&ss, rand2d, &wi, &bsdfPdf, &bsdfWeight);

			if (bsdfPdf == 0.0 || bsdfWeight.Lum() < C_FLOAT_EPSILON)
				break;

			ray.m_origin = ss.m_pos;
			ray.m_direction = wi;

			bool hitLight = false;

			if (m_renderer->m_scene->Intersect(ray, &hitInfo))
			{
				if (hitInfo.m_obj->IsLight())
				{
					hitLight = true;
				}
			}
			else
			{
				break;
			}
			state->m_throughtput *= bsdfWeight / bsdfPdf;

			if (hitLight)
			{
				const Light* light = static_cast<const Light*>(hitInfo.m_obj);
				auto li = light->Le(&ss, -ray.m_direction);
				Float lightPdf = light->Pdf(hitInfo, &ss, wi);
				Float misWeight = SampleUtils::PowerHeuristic(bsdfPdf, lightPdf);
				L->m_directDiffuse += state->m_throughtput * li * misWeight;
			}
			const Float eta = 1.0f;

			if (state->m_currentBounce >= rrBounce)
			{
				Float q = std::min(state->m_throughtput.Max() * eta * eta, (Float) 0.95f);
				if (state->m_sampler->Random1D() >= q)
					break;
				state->m_throughtput /= q;
			}
		}
	}

	bool PathTracing::SampleLight(State* state, SurfaceSample* ss, Radiance* L)
	{
		Vector2f lightRand2d = state->m_sampler->Random2D();
		Float rand1d = state->m_sampler->Random1D();
		int index = Clamp<int>((int)((m_renderer->m_scene->m_lights.size() - 1) * rand1d), 0, m_renderer->m_scene->m_lights.size() - 1);
		Light* light = m_renderer->m_scene->m_lights[index];
		LightSample ls;
		Float lightPdf = 0;
		if (light->Sample(lightRand2d, ss, &ls, &lightPdf) && lightPdf > 0)
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
				if (lightHitInfo.m_obj != light)
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
		if (pixelX == 104 && pixelY == 207)
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