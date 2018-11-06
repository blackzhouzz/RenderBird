#include "DisneyBSDF.h"
#include "MicrofacetDistribution.h"
#include "SampleUtils.h"

namespace RenderBird
{
	static Float GTR1(Float nh, Float a)
	{
		if (a >= 1) return 1 / C_PI;
		Float a2 = a * a;
		Float t = 1 + (a2 - 1)*nh*nh;
		return (a2 - 1) / (C_PI*log(a2)*t);
	}

	static Float SeparableSmithGGXG1(const Vector3f& w, Float a)
	{
		Float a2 = a * a;
		Float absDotNV = AbsCosTheta(w);

		return 2.0f / (1.0f + std::sqrt(a2 + (1 - a2) * absDotNV * absDotNV));
	}

	static Float EvaluateDisneyClearcoat(Float clearcoat, Float alpha, const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, Float& pdf)
	{
		if (clearcoat <= 0.0f) 
		{
			return 0.0f;
		}

		Float absDotNH = AbsCosTheta(wm);
		Float absDotNL = AbsCosTheta(wi);
		Float absDotNV = AbsCosTheta(wo);
		Float dotHL = Vector3f::DotProduct(wm, wi);

		Float d = GTR1(absDotNH, Lerp(0.1, 0.001, alpha));
		Float f = Fresnel::Schlick(0.04f, dotHL);
		Float gl = SeparableSmithGGXG1(wi, 0.25f);
		Float gv = SeparableSmithGGXG1(wo, 0.25f);

		pdf = d / (4.0f * absDotNL);

		return 0.25f * clearcoat * d * f * gl * gv;
	}

	static Float GgxAnisotropicD(const Vector3f& wm, Float ax, Float ay)
	{
		Float dotHX2 = Square(wm.x);
		Float dotHY2 = Square(wm.y);
		Float cos2Theta = Cos2Theta(wm);
		Float ax2 = Square(ax);
		Float ay2 = Square(ay);

		return 1.0f / (C_PI * ax * ay * Square(dotHX2 / ax2 + dotHY2 / ay2 + cos2Theta));
	}

	static Float GgxAnisotropicPdf(const Vector3f& wo, const Vector3f& wm, Float ax, Float ay)
	{
		Float d = GgxAnisotropicD(wm, ax, ay);

		return d * CosTheta(wm) * 0.25 / AbsDotProduct(wo, wm);
	}

	static Float SeparableSmithGGXG1(const Vector3f& w, const Vector3f& wm, Float ax, Float ay)
	{
		Float dotHW = Vector3f::DotProduct(w, wm);
		if (dotHW <= 0.0f) {
			return 0.0f;
		}

		Float absTanTheta = std::abs(TanTheta(w));
		if (IsInf(absTanTheta)) {
			return 0.0f;
		}

		Float a = std::sqrt(Cos2Phi(w) * ax * ax + Sin2Phi(w) * ay * ay);
		Float a2Tan2Theta = Square(a * absTanTheta);

		Float lambda = 0.5f * (-1.0f + std::sqrt(1.0f + a2Tan2Theta));
		return 1.0f / (1.0f + lambda);
	}

	static RGB32 CalculateTint(RGB32 baseColor)
	{
		// -- The color tint is never mentioned in the SIGGRAPH presentations as far as I recall but it was done in
		// --  the BRDF Explorer so I'll replicate that here.
		Float luminance = (baseColor[0] * 0.212671 + baseColor[1] * 0.715160 + baseColor[2] * 0.072169);
		return (luminance > 0.0f) ? baseColor * (1.0f / luminance) : RGB32::WHITE;
	}

	static Float ThinTransmissionRoughness(Float ior, Float roughness)
	{
		// -- Disney scales by (.65 * eta - .35) based on figure 15 of the 2015 PBR course notes. Based on their figure
		// -- the results match a geometrically thin solid fairly well.
		return Saturate((0.65 * ior - 0.35) * roughness);
	}

	static Vector3f GGXSampleAnisotropic(const Vector2f& rand2d, Float alphaU, Float alphaV)
	{
		Float cosTheta = 0.0f;
		Float cosPhi = 0;
		Float sinPhi = 0;
		Float phi = std::atan(alphaU / alphaV * std::tan(C_2_PI * rand2d.y)) + C_PI * std::floor(2 * rand2d.y + 0.5);
		SinCos(phi, sinPhi, cosPhi);
		Float root = (1 - rand2d.x) * (Square(cosPhi / alphaU) + Square(sinPhi / alphaV));
		cosTheta = std::cos(std::atan(std::sqrt(rand2d.x / root)));

		Float sinTheta = std::sqrt(Max(1.0 - cosTheta * cosTheta, 0.0));
		return Vector3f(cosPhi * sinTheta, sinPhi * sinTheta, cosTheta);
	}

	DisneyBSDF::DisneyBSDF()
		//: m_roughness(roughness)
		//, m_metallic(metallic)
		//, m_sheen(sheen)
		//, m_sheenTint(sheenTint)
	{
		m_roughness = 0.02;
		m_sheen = 0;
		m_sheenTint = 0;
		m_metallic = 0;
		m_clearcoat = 0;
		m_clearcoatGloss = 0;
		m_diffTrans = 0;
		m_specTrans = 0;
		m_specularTint = 0;
		m_anisotropic = 0;
		m_flatness = 0;
		m_flags = DiffuseLobe | SpecularLobe;
		m_etaI = 1.0;
		m_etaT = 2.4;
		m_relativeEta = m_etaT / m_etaI;
	}

	RGB32 DisneyBSDF::DisneyFresnel(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi)
	{
		Float dotHV = AbsDotProduct(wm, wo);

		RGB32 tint = CalculateTint(Albedo());

		// -- See section 3.1 and 3.2 of the 2015 PBR presentation + the Disney BRDF explorer (which does their
		// -- 2012 remapping rather than the SchlickR0FromRelativeIOR seen here but they mentioned the switch in 3.2).
		RGB32 R0 = Lerp(RGB32::WHITE, tint, m_specularTint) * Fresnel::SchlickR0FromRelativeIOR(m_relativeEta);
		R0 = Lerp(R0, Albedo(), m_metallic);

		Float cosThetaT = 0;
		Float dielectricFresnel = Fresnel::Dielectric(1.0f, m_etaT, dotHV, cosThetaT);
		RGB32 metallicFresnel = Fresnel::Schlick(R0, Vector3f::DotProduct(wi, wm));

		return Lerp(RGB32(dielectricFresnel, dielectricFresnel, dielectricFresnel), metallicFresnel, m_metallic);
	}

	RGB32 DisneyBSDF::EvaluateDisneyBRDF(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, Float& fPdf)
	{
		fPdf = 0.0f;

		Float dotNL = CosTheta(wi);
		Float dotNV = CosTheta(wo);
		if (dotNL <= 0.0f || dotNV <= 0.0f)
		{
			return RGB32::BLACK;
		}

		Float ax, ay;
		ConvertAnisotropyToRoughness(m_roughness, m_anisotropic, ax, ay);

		Float d = GgxAnisotropicD(wm, ax, ay);
		Float gl = SeparableSmithGGXG1(wi, wm, ax, ay);
		Float gv = SeparableSmithGGXG1(wo, wm, ax, ay);

		RGB32 f = DisneyFresnel(wo, wm, wi);

		fPdf = d * CosTheta(wm) * 0.25 / AbsDotProduct(wo, wm);

		return f * d * gl * gv / (4.0f * dotNL * dotNV);
	}

	RGB32 DisneyBSDF::EvaluateDisneySpecTransmission(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, Float ax, Float ay, bool thin)
	{
		Float relativeIor = m_relativeEta;
		Float n2 = relativeIor * relativeIor;

		Float absDotNL = AbsCosTheta(wi);
		Float absDotNV = AbsCosTheta(wo);
		Float dotHL = Vector3f::DotProduct(wm, wi);
		Float dotHV = Vector3f::DotProduct(wm, wo);
		Float absDotHL = std::abs(dotHL);
		Float absDotHV = std::abs(dotHV);

		Float d = GgxAnisotropicD(wm, ax, ay);
		Float gl = SeparableSmithGGXG1(wi, wm, ax, ay);
		Float gv = SeparableSmithGGXG1(wo, wm, ax, ay);

		Float cosThetaT = 0;
		Float f = Fresnel::Dielectric(1.0, m_etaT, dotHV, cosThetaT);
		RGB32 baseColor = Albedo();
		RGB32 color;
		if (thin)
			color = baseColor.Sqrt();
		else
			color = baseColor;

		// Note that we are intentionally leaving out the 1/n2 spreading factor since for VCM we will be evaluating
		// particles with this. That means we'll need to model the air-[other medium] transmission if we ever place
		// the camera inside a non-air medium.
		Float c = (absDotHL * absDotHV) / (absDotNL * absDotNV);
		Float t = (n2 / Square(dotHL + relativeIor * dotHV));
		return color * c * t * (1.0f - f) * gl * gv * d;
	}

	RGB32 DisneyBSDF::EvaluateDisney(const Vector3f& localWi, const Vector3f& localWo, bool thin, Float& forwardPdf)
	{
		Vector3f wo = localWo;
		Vector3f wi = localWi;
		Vector3f wm = (wo + wi).Normalized();

		Float dotNV = CosTheta(wo);
		Float dotNL = CosTheta(wi);

		RGB32 reflectance = RGB32::BLACK;
		forwardPdf = 0.0f;

		Float pBRDF, pDiffuse, pClearcoat, pSpecTrans;
		CalculateLobePdfs(pBRDF, pDiffuse, pClearcoat, pSpecTrans);

		RGB32 baseColor = Albedo();
		Float metallic = m_metallic;
		Float specTrans = m_specTrans;
		Float roughness = m_roughness;

		// calculate all of the anisotropic params
		Float ax, ay;
		ConvertAnisotropyToRoughness(m_roughness, m_anisotropic, ax, ay);

		Float diffuseWeight = (1.0f - metallic) * (1.0f - specTrans);
		Float transWeight = (1.0f - metallic) * specTrans;

		// -- Clearcoat
		bool upperHemisphere = dotNL > 0.0f && dotNV > 0.0f;
		if (upperHemisphere && m_clearcoat > 0.0f)
		{
			Float forwardClearcoatPdfW;

			Float clearcoat = EvaluateDisneyClearcoat(m_clearcoat, m_clearcoatGloss, wo, wm, wi, forwardClearcoatPdfW);
			reflectance += RGB32(clearcoat, clearcoat, clearcoat);
			forwardPdf += pClearcoat * forwardClearcoatPdfW;
		}

		// -- Diffuse
		if (diffuseWeight > 0.0f && dotNL > 0.0f)
		{
			Float forwardDiffusePdfW = AbsCosTheta(wi);
			//Float reverseDiffusePdfW = AbsCosTheta(wo);
			Float diffuse = EvaluateDisneyDiffuse(wo, wm, wi, thin);

			RGB32 sheen = EvaluateSheen(wo, wm, wi);

			reflectance += diffuseWeight * (diffuse * baseColor + sheen);

			forwardPdf += pDiffuse * forwardDiffusePdfW;
		}

		// -- transmission
		if (transWeight > 0.0f)
		{
			// Scale roughness based on IOR (Burley 2015, Figure 15).
			//Float rscaled = thin ? ThinTransmissionRoughness(m_etaI, m_roughness) : m_roughness;
			//Float tax, tay;
			//ConvertAnisotropyToRoughness(rscaled, m_anisotropic, tax, tay);

			//RGB32 transmission = EvaluateDisneySpecTransmission(wo, wm, wi, tax, tay, thin);
			//reflectance += transWeight * transmission;

			//Float forwardTransmissivePdfW = GgxAnisotropicPdf(wo, wm, ax, ay);

			//Float dotLH = Vector3f::DotProduct(wm, wi);
			//Float dotVH = Vector3f::DotProduct(wm, wo);
			//forwardPdf += pSpecTrans * forwardTransmissivePdfW / (Square(dotLH + m_relativeEta * dotVH));
		}

		// -- specular
		if (upperHemisphere)
		{
			Float forwardMetallicPdfW;
			//Float reverseMetallicPdfW;
			RGB32 specular = EvaluateDisneyBRDF(wo, wm, wi, forwardMetallicPdfW);

			reflectance += specular;
			forwardPdf += pBRDF * forwardMetallicPdfW /*/ (4 * AbsDotProduct(wo, wm))*/;
		}

		reflectance = reflectance * std::abs(dotNL);

		return reflectance;
	}

	void DisneyBSDF::CalculateLobePdfs(Float& pSpecular, Float& pDiffuse, Float& pClearcoat, Float& pSpecTrans)
	{
		Float metallicBRDF = m_metallic;
		Float specularBSDF = (1.0f - m_metallic) * m_specTrans;
		Float dielectricBRDF = (1.0f - m_specTrans) * (1.0f - m_metallic);

		Float specularWeight = metallicBRDF + dielectricBRDF;
		Float transmissionWeight = specularBSDF;
		Float diffuseWeight = dielectricBRDF;
		Float clearcoatWeight = 1.0f * m_clearcoat;

		Float norm = 1.0f / (specularWeight + transmissionWeight + diffuseWeight + clearcoatWeight);

		pSpecular = specularWeight * norm;
		pSpecTrans = transmissionWeight * norm;
		pDiffuse = diffuseWeight * norm;
		pClearcoat = clearcoatWeight * norm;
	}

	RGB32 DisneyBSDF::EvaluateSheen(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi)
	{
		if (m_sheen <= 0.0f) 
		{
			return RGB32::BLACK;
		}

		Float dotHL = Vector3f::DotProduct(wm, wi);
		RGB32 tint = CalculateTint(Albedo());
		return Lerp(RGB32::WHITE, tint, m_sheenTint) * Fresnel::SchlickWeight(dotHL) * m_sheen;
	}
	
	Float DisneyBSDF::EvaluateDisneyDiffuse(const Vector3f& wo, const Vector3f& wm, const Vector3f& wi, bool thin)
	{
		Float dotNL = AbsCosTheta(wi);
		Float dotNV = AbsCosTheta(wo);
		Float dotHL = Vector3f::DotProduct(wm, wi);
		Float lh2 = dotHL * dotHL;

		Float fl = Fresnel::SchlickWeight(dotNL);
		Float fv = Fresnel::SchlickWeight(dotNV);

		Float hanrahanKrueger = 0.0f;

		if (thin && m_flatness > 0.0f)
		{
			Float roughness = m_roughness * m_roughness;

			Float fss90 = lh2 * roughness;
			Float fss = Lerp(1.0, fss90, fl) * Lerp(1.0, fss90, fv);

			Float ss = 1.25f * (fss * (1.0f / (dotNL + dotNV) - 0.5f) + 0.5f);
			hanrahanKrueger = ss;
		}
		Float subsurfaceApprox = Lerp(1.0, hanrahanKrueger, thin ? m_flatness : 0.0);

		Float rr = 2 * m_roughness * lh2;
		Float retro = rr * (fl + fv + fl * fv * (rr - 1.0));

		return C_1_INV_PI * (retro + subsurfaceApprox * (1.0f - 0.5f * fl) * (1.0f - 0.5f * fv));
	}

	RGB32 DisneyBSDF::Eval(SurfaceSample* ss)
	{
		auto localWi = ss->m_wi;
		auto localWo = ss->m_wo;

		return EvaluateDisney(localWi, localWo, false, ss->m_pdf);
	}

	bool DisneyBSDF::SampleDisneySpecTransmission(SurfaceSample* ss, Sampler* sampler, Vector3f* outWi, Float* outPdf, bool thin, RGB32& weight)
	{
		Vector3f wo = ss->m_wo;
		if (CosTheta(wo) == 0.0) 
		{
			*outPdf = 0;
			return false;
		}
		auto baseColor = Albedo();

		// -- Scale roughness based on IOR
		Float rscaled = thin ? ThinTransmissionRoughness(m_etaI, m_roughness) : m_roughness;

		Float tax, tay;
		ConvertAnisotropyToRoughness(rscaled, m_anisotropic, tax, tay);

		// -- Sample visible distribution of normals
		Vector3f wm = GGXSampleAnisotropic(sampler->Next2D(), tax, tay);

		Float dotVH = Vector3f::DotProduct(wo, wm);
		if (wm.y < 0.0f) 
		{
			dotVH = -dotVH;
		}

		Float ni = wo.y > 0.0f ? 1.0f : m_etaI;
		Float nt = wo.y > 0.0f ? m_etaI : 1.0f;
		Float relativeIOR = ni / nt;

		// -- Disney uses the full dielectric Fresnel equation for transmission. We also importance sample F
		// -- to switch between refraction and reflection at glancing angles.
		Float cosThetaT = 0;
		Float F = Fresnel::Dielectric(1.0f, m_etaT, dotVH, cosThetaT);

		// -- Since we're sampling the distribution of visible normals the pdf cancels out with a number of other terms.
		// -- We are left with the weight G2(wi, wo, wm) / G1(wi, wm) and since Disney uses a separable masking function
		// -- we get G1(wi, wm) * G1(wo, wm) / G1(wi, wm) = G1(wo, wm) as our weight.
		Float G1v = SeparableSmithGGXG1(wo, wm, tax, tay);

		Float pdf = 0;

		Vector3f wi;
		if (sampler->Next1D() <= F) 
		{
			wi = Reflect(wm, wo).Normalized();

			//sample.flags = SurfaceEventFlags::eScatterEvent;
			//sample.reflectance = G1v * baseColor;
			weight += G1v * baseColor;

			Float jacobian = (4 * AbsDotProduct(wo, wm));
			pdf = F / jacobian;
		}
		else
		{
			if (thin) 
			{
				// -- When the surface is thin so it refracts into and then out of the surface during this shading event.
				// -- So the ray is just reflected then flipped and we use the sqrt of the surface color.
				wi = Reflect(wm, wo);
				wi.y = -wi.y;
				weight += G1v * baseColor.Sqrt();

				// -- Since this is a thin surface we are not ending up inside of a volume so we treat this as a scatter event.
				//sample.flags = SurfaceEventFlags::eScatterEvent;
			}
			else
			{
				//if (Transmit(wm, wo, relativeIOR, wi)) 
				//{
				//	sample.flags = SurfaceEventFlags::eTransmissionEvent;
				//	sample.medium.phaseFunction = dotVH > 0.0f ? MediumPhaseFunction::eIsotropic : MediumPhaseFunction::eVacuum;
				//	sample.medium.extinction = CalculateExtinction(surface.transmittanceColor, surface.scatterDistance);
				//}
				//else 
				{
					//sample.flags = SurfaceEventFlags::eScatterEvent;
					wi = Reflect(wm, wo);
				}

				weight += G1v * baseColor;
			}

			wi = wi.Normalized();

			Float dotLH = AbsDotProduct(wi, wm);
			Float jacobian = dotLH / (Square(dotLH + m_relativeEta * dotVH));
			pdf = (1.0f - F) / jacobian;
		}

		if (CosTheta(wi) == 0.0f) 
		{
			*outPdf = 0;
			return false;
		}

		if (m_roughness < 0.01f) 
		{
			// -- This is a hack to allow us to sample the correct IBL texture when a path bounced off a smooth surface.
			//sample.flags |= SurfaceEventFlags::eDiracEvent;
		}

		// -- calculate VNDF pdf terms and apply Jacobian and Fresnel sampling adjustments
		*outPdf = GgxAnisotropicPdf(wo, wm, tax, tay) * pdf;
		*outWi = wi;

		return true;
	}

	bool DisneyBSDF::SampleDisneyClearcoat(SurfaceSample* ss, Sampler* sampler, Vector3f* outWi, Float* outPdf, RGB32& weight)
	{
		Vector3f wo = ss->m_wo;

		Float a = 0.25f;
		Float a2 = a * a;

		Vector2f rand2D = sampler->Next2D();
		Float cosTheta = std::sqrt(Max<Float>(0, (1.0 - pow(a2, 1.0 - rand2D.x)) / (1.0 - a2)));
		Float sinTheta = std::sqrt(Max<Float>(0, 1.0 - cosTheta * cosTheta));
		Float phi = C_2_PI * rand2D.y;

		Vector3f wm = Vector3f(sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi));
		if (Vector3f::DotProduct(wm, wo) < 0.0f) 
		{
			wm = -wm;
		}

		Vector3f wi = Reflect(wm, wo);
		if (Vector3f::DotProduct(wi, wo) < 0.0f) 
		{
			return false;
		}

		Float clearcoatWeight = m_clearcoat;
		Float clearcoatGloss = m_clearcoatGloss;

		Float dotNH = CosTheta(wm);
		Float dotLH = Vector3f::DotProduct(wm, wi);
		Float absDotNL = AbsCosTheta(wi);
		Float absDotNV = AbsCosTheta(wo);

		Float d = GTR1(std::abs(dotNH), Lerp(0.1, 0.001, clearcoatGloss));
		Float f = Fresnel::Schlick(0.04f, dotLH);
		Float g = SeparableSmithGGXG1(wi, 0.25f) * SeparableSmithGGXG1(wo, 0.25f);

		Float fPdf = d / (4.0f * Vector3f::DotProduct(wo, wm));
		Float clearCoat = 0.25f * clearcoatWeight * g * f * d / fPdf;
		weight += RGB32(clearCoat, clearCoat, clearCoat);
		*outWi = wi;
		*outPdf = fPdf;

		return true;
	}

	bool DisneyBSDF::SampleDisneyDiffuse(SurfaceSample* ss, Sampler* sampler, Vector3f* outWi, Float* outPdf, bool thin, RGB32& weight)
	{
		Vector3f wo = ss->m_wo;

		Vector3f wi;
		SampleUtils::CosHemisphere(sampler->Next2D(), &wi, outPdf);
		Vector3f wm = (wi + wo).Normalized();

		Float dotNL = CosTheta(wi);
		if (dotNL == 0.0f) 
		{
			*outPdf = 0;
			return false;
		}
		Float dotNV = CosTheta(wo);

		Float pdf;

		RGB32 color = Albedo();

		Float p = sampler->Next1D();
		if (p <= m_diffTrans) 
		{
			wi = -wi;
			pdf = m_diffTrans;

			if (thin)
			{
				color = color.Sqrt();
			}
			else 
			{
				//eventType = SurfaceEventFlags::eTransmissionEvent;

				//sample.medium.phaseFunction = MediumPhaseFunction::eIsotropic;
				//sample.medium.extinction = CalculateExtinction(surface.transmittanceColor, surface.scatterDistance);
			}
		}
		else 
		{
			pdf = (1.0f - m_diffTrans);
		}

		RGB32 sheen = EvaluateSheen(wo, wm, wi);

		Float diffuse = EvaluateDisneyDiffuse(wo, wm, wi, thin);
		weight += sheen + color * (diffuse / pdf);
		*outWi = wi;
		*outPdf = std::abs(dotNL) * pdf;
		return true;
	}

	bool DisneyBSDF::Sample(SurfaceSample* ss, Sampler* sampler, RGB32& weight)
	{
		auto localWo = ss->m_wo;
		ss->m_pdf = 0;
		Float pBRDF, pDiffuse, pClearcoat, pSpecTrans;
		CalculateLobePdfs(pBRDF, pDiffuse, pClearcoat, pSpecTrans);
		if (pDiffuse > 0)
		{
			Float diffPdf = 0;
			if (SampleDisneyDiffuse(ss, sampler, &ss->m_wi, &diffPdf, false, weight))
			{
				ss->m_pdf += pDiffuse * diffPdf;
			}
		}

		if (pClearcoat > 0)
		{
			Float clearCoatPdf = 0;
			if (SampleDisneyClearcoat(ss, sampler, &ss->m_wi, &clearCoatPdf, weight))
			{
				ss->m_pdf += pClearcoat * clearCoatPdf;
			}
		}

		Float transPdf = 0;
		if (SampleDisneySpecTransmission(ss, sampler, &ss->m_wi, &transPdf, false, weight))
		{
			ss->m_pdf += transPdf;
		}

		return ss->m_pdf > 0;
	}
}