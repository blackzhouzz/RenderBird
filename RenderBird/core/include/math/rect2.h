#pragma once
#include "vector.h"

namespace Core
{
	template<typename T>
	class TRect2
	{
	public:
		TVector2<T> m_min, m_max;
	public:
		TRect2() = default;
		TRect2(const TVector2<T>& min, const TVector2<T>& max)
			: m_min(min)
			, m_max(max)
		{
		}

		TRect2(T minX, T minY, T maxX, T maxY)
			: m_min(TVector2<T>(minX, minY))
			, m_max(TVector2<T>(maxX, maxY))
		{
		}
		static const TRect2<T> ZERO;
	};
	using Rect2f = TRect2<Float>;
	using Rect2i = TRect2<int>;

	template <typename T>
	const TRect2<T> TRect2<T>::ZERO(0, 0, 0, 0);
}