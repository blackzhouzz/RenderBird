#pragma once
#include "math_core.h"

namespace Core
{
	template<typename T>
	class TRect2
	{
	public:
		mathfu::Vector<T, 2> m_min, m_max;
	public:
		TRect2() = default;
		TRect2(const mathfu::Vector<T, 2>& min, const mathfu::Vector<T, 2>& max)
			: m_min(min)
			, m_max(max)
		{
		}

		TRect2(T minX, T minY, T maxX, T maxY)
			: m_min(mathfu::Vector<T, 2>(minX, minY))
			, m_max(mathfu::Vector<T, 2>(maxX, maxY))
		{
		}
		static const TRect2<T> ZERO;
	};
	using Rect2f = TRect2<Float>;
	using Rect2i = TRect2<int>;

	template <typename T>
	const TRect2<T> TRect2<T>::ZERO(0, 0, 0, 0);
}