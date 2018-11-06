#pragma once
#include "RenderBirdPrivate.h"
#include "BSDF.h"

namespace RenderBird
{
	class DisneyBSDF : public BSDF
	{
	public:
		DisneyBSDF();
		virtual RGB32 Eval(SurfaceSample* ss);
		virtual bool Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight);
		void CalculateLobePdfs(Float& pSpecular, Float& pDiffuse, Float& pClearcoat, Float& pSpecTrans);
		RGB32 EvaluateSheen(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi);
		RGB32 EvaluateDisneyBRDF(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, Float& fPdf);
		RGB32 DisneyFresnel(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi);
		RGB32 EvaluateDisney(const Vector3f& localWi, const Vector3f& localWo, bool thin, Float& forwardPdf);
		Float EvaluateDisneyDiffuse(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, bool thin);
		RGB32 EvaluateDisneySpecTransmission(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, Float ax, Float ay, bool thin);
		bool SampleDisneySpecTransmission(SurfaceSample* ss, Sampler* sampler, Vector3f* outWi, Float* outPdf, bool thin, RGB32& weight);
		bool SampleDisneyDiffuse(SurfaceSample* ss, Sampler* sampler, Vector3f* outWi, Float* outPdf, bool thin, RGB32& weight);
		bool SampleDisneyClearcoat(SurfaceSample* ss, Sampler* sampler, Vector3f* outWi, Float* outPdf, RGB32& weight);
	private:
		Float m_roughness;
		Float m_sheen;
		Float m_sheenTint;
		Float m_metallic;
		Float m_clearcoat;
		Float m_clearcoatGloss;
		Float m_diffTrans;
		Float m_specTrans;
		Float m_specularTint;
		Float m_anisotropic;
		Float m_flatness;
		Float m_etaI;
		Float m_etaT;
		Float m_relativeEta;
	};
}