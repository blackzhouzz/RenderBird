#include "pathtracing.h"
#include "renderer.h"
#include "arealight.h"
#include "bsdf.h"

namespace RenderBird
{
	PathTracing::PathTracing(Renderer* renderer)
		: m_renderer(renderer)
	{
		m_setting.m_maxBounce = 3;
		m_setting.m_numSamples = 1;
	}

	void PathTracing::InitSurfaceData(State* state, SurfaceSample* ss, const Ray& ray, const RayHitInfo& hitInfo)
	{
		ss->m_position = hitInfo.m_position;
		ss->m_normal = hitInfo.m_normal;
		ss->m_geomNormal = hitInfo.m_geomNormal;
		if (ss->m_normal.IsZero())
		{
			ss->m_normal = ss->m_geomNormal;
		}

		ss->m_wo = -ray.m_direction;

		//ss->m_isBackfacing = (Dot(ss->m_geomNormal, ss->m_wo) < 0.0f);
		//if (ss->m_isBackfacing)
		//{
		//	ss->m_normal = -ss->m_normal;
		//	ss->m_geomNormal = -ss->m_geomNormal;
		//}
	}

	void PathTracing::Integrate(State* state, Radiance* L)
	{
		Ray ray;
		m_renderer->GenerateCameraRay(state->m_pixelX, state->m_pixelY, &ray);

		//path tracing loop begin
		for (state->m_currentBounce = 0; ;state->m_currentBounce++)
		{
			RayHitInfo hitInfo;
			bool isHit = m_renderer->m_scene->Intersect(ray, &hitInfo);
			if (!isHit || state->m_currentBounce > m_setting.m_maxBounce)
			{
				break;
			}

			SurfaceSample ss;
			ss.m_bsdf = new DiffuseBSDF();
			InitSurfaceData(state, &ss, ray, hitInfo);

			RGBA32 diffuseColor = RGBA32::WHITE;
			if (hitInfo.m_material != nullptr)
			{
				diffuseColor = hitInfo.m_material->m_diffuseColor;
			}
			Float nol = Dot(hitInfo.m_normal.GetNormalized(), Vector3f(-1, -1, 1).GetNormalized());
			RGB32 color = RGB32(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			L->m_directDiffuse = color;
			break;
			
			EvalLight(state, &ss, L);

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

	void PathTracing::EvalLight(State* state, SurfaceSample* ss, Radiance* L)
	{
		Vector2f lightRand2d = state->m_sampler->Random2D();
		EntityId lightId = m_renderer->m_scene->m_diskLightId;
		LightSample ls;
		if (AreaLightUtils::SampleDisk(lightId, lightRand2d, ss, &ls))
		{
			BsdfSpectrum bs;
			Float pdf = 0.0;
			EvalBSDF(state, ss, ls.m_wi, &pdf, &bs);
			bs.Mul(ls.m_li / ls.m_pdf);
			if (!bs.IsZero())
			{
				AccumRadianceSpectrum(state, &bs, L);
			}
		}
	}

	void PathTracing::AccumBSDFSpectrum(SurfaceSample* ss, BsdfSpectrum* bs, const RGB32& value)
	{
		bs->m_diffuse += value;
	}

	void PathTracing::AccumRadianceSpectrum(State* state, BsdfSpectrum* bs, Radiance* L)
	{
		L->m_directDiffuse += state->m_throughtput * bs->m_diffuse;
	}

	bool PathTracing::ProbabilityStop()
	{
		return false;
	}

	void PathTracing::InitState(int pixelX, int pixelY, State* state)
	{
		state->m_pixelX = pixelX;
		state->m_pixelY = pixelY;
		state->m_currentBounce = 0;
		state->m_throughtput = RGB32::WHITE;
		state->m_sampler = new Sampler(m_setting.m_numSamples);
	}

	void PathTracing::Render(int pixelX, int pixelY)
	{
		if (pixelX == 128 && pixelY == 128)
		{
			pixelX = pixelX;
		}
		Radiance L;
		PathTracingUtils::InitRadiance(&L);
		State state;
		InitState(pixelX, pixelY, &state);
		for (int samplerIndex = 0; samplerIndex < state.m_sampler->m_numSamplers; ++samplerIndex)
		{
			state.m_curSamplerIndex = samplerIndex;
			Integrate(&state, &L);
		}
		m_renderer->SetColor(pixelX, pixelY, L.m_directDiffuse);
	}

	bool PathTracing::SurfaceBounce(State* state, SurfaceSample* ss, Ray& ray)
	{
		Vector2f rand2d = state->m_sampler->Random2D();
		RGB32 bdsf;
		Vector3f wi;
		Float pdf = 0.0;
		BsdfSpectrum bs;

		ss->m_bsdf->Sample(ss, rand2d, &pdf, &wi, &bdsf);

		//auto v = ReflectVector(*wi, ss->m_normal);

		if (pdf == 0.0 || bdsf.IsZero())
			return false;

		EvalBSDF(state, ss, wi, &pdf, &bs);
		if (pdf == 0.0)
			return false;
		Float invPdf = 1.0f / pdf;
		state->m_throughtput *= bs.m_diffuse * invPdf;

		ray.m_origin = ss->m_position;
		ray.m_direction = wi;

		return true;
	}
}