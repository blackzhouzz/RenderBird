#pragma once
#include "RenderbirdPrivate.h"

namespace RenderBird
{
	class SampleUtils
	{
	public:
		static Vector2f ToUnitDisk(const Vector2f &rand2d);
		static Vector2f UniformTriangle(const Vector2f& rand2d);
		static void CosHemisphere(const Vector2f& rand2d, Vector3f* wi, Float* pdf);
		static Float CosHemispherePdf(Float cosTheta);
		static Float UniformHemispherePdf();
		static void UniformHemisphere(const Vector2f& rand2d, Vector3f* wi, Float* pdf);
		static void UniformSphere(const Vector2f& rand2d, Vector3f* wi, Float* pdf);
		static void UniformCone(const Vector2f& rand2d, Float angle, Vector3f* wi, Float* pdf);
		static Float PowerHeuristic(Float pdf1, Float pdf2);
		static Float BalanceHeuristic(Float pdf1, Float pdf2);
	};

	class DiscreteDistribution 
	{
	public:
		explicit inline DiscreteDistribution(size_t numEntries = 0)
		{
			Reserve(numEntries);
			Clear();
		}

		inline void Clear()
		{
			m_cdf.clear();
			m_cdf.push_back(0.0f);
			m_normalized = false;
		}

		inline void Reserve(size_t numEntries)
		{
			m_cdf.reserve(numEntries + 1);
		}

		inline void Append(Float pdfValue) 
		{
			m_cdf.push_back(m_cdf[m_cdf.size() - 1] + pdfValue);
		}

		inline size_t Size() const 
		{
			return m_cdf.size() - 1;
		}

		inline Float operator[](size_t entry) const 
		{
			return m_cdf[entry + 1] - m_cdf[entry];
		}

		inline bool IsNormalized() const 
		{
			return m_normalized;
		}

		inline Float GetSum() const 
		{
			return m_sum;
		}

		inline Float GetNormalization() const 
		{
			return m_normalization;
		}

		inline Float Normalize() 
		{
			m_sum = m_cdf[m_cdf.size() - 1];
			if (m_sum > 0) 
			{
				m_normalization = 1.0f / m_sum;
				for (size_t i = 1; i < m_cdf.size(); ++i)
					m_cdf[i] *= m_normalization;
				m_cdf[m_cdf.size() - 1] = 1.0f;
				m_normalized = true;
			}
			else 
			{
				m_normalization = 0.0f;
			}
			return m_sum;
		}

		inline size_t Sample(Float &sampleValue) const
		{
			size_t index = SampleImpl(sampleValue);
			sampleValue = (sampleValue - m_cdf[index])
				/ (m_cdf[index + 1] - m_cdf[index]);
			return index;
		}

		inline size_t Sample(Float &sampleValue, Float &pdf) const
		{
			size_t index = SampleImpl(sampleValue, pdf);
			sampleValue = (sampleValue - m_cdf[index])
				/ (m_cdf[index + 1] - m_cdf[index]);
			return index;
		}

		//std::string toString() const 
		//{
		//	std::ostringstream oss;
		//	oss << "DiscreteDistribution[sum=" << m_sum << ", normalized="
		//		<< (int)m_normalized << ", cdf={";
		//	for (size_t i = 0; i < m_cdf.size(); ++i) 
		//	{
		//		oss << m_cdf[i];
		//		if (i != m_cdf.size() - 1)
		//			oss << ", ";
		//	}
		//	oss << "}]";
		//	return oss.str();
		//}
	private:
		inline size_t SampleImpl(Float sampleValue) const
		{
			std::vector<Float>::const_iterator entry =
				std::lower_bound(m_cdf.begin(), m_cdf.end(), sampleValue);
			size_t index = std::min(m_cdf.size() - 2,
				(size_t)std::max((ptrdiff_t)0, entry - m_cdf.begin() - 1));

			/* Handle a rare corner-case where a entry has probability 0
			   but is sampled nonetheless */
			while (operator[](index) == 0 && index < m_cdf.size() - 1)
				++index;

			return index;
		}

		inline size_t SampleImpl(Float sampleValue, Float &pdf) const
		{
			size_t index = Sample(sampleValue);
			pdf = operator[](index);
			return index;
		}
	private:
		std::vector<Float> m_cdf;
		Float m_sum, m_normalization;
		bool m_normalized;
	};
}