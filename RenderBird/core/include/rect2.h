#pragma once
#include "MathCore.h"

namespace Core
{
	template<typename T> class RangeIterator;

	template<typename T>
	class Range
	{
		T _start, _end, _step;

	public:
		Range(T start, T end, T step = T(1))
			: _start(start), _end(end), _step(step)
		{
		}

		RangeIterator<T> begin() const;
		RangeIterator<T> end() const;
	};

	template<typename T>
	Range<T> range(T end)
	{
		return Range<T>(T(0), end, T(1));
	}

	template<typename T>
	Range<T> range(T start, T end, T step = T(1))
	{
		return Range<T>(start, end, step);
	}

	template<typename T>
	class RangeIterator
	{
		T _pos, _step;

	public:
		RangeIterator(T pos, T step) : _pos(pos), _step(step) {}

		bool operator!=(const RangeIterator &o) const { return _pos < o._pos; }
		RangeIterator &operator++() { _pos += _step; return *this; }
		RangeIterator operator++(int) { RangeIterator copy(*this); operator++(); return copy; }
		T operator*() const { return _pos; }
	};

	template<typename T>
	RangeIterator<T> Range<T>::begin() const { return RangeIterator<T>(_start, _step); }

	template<typename T>
	RangeIterator<T> Range<T>::end() const { return RangeIterator<T>(_end, _step); }


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
		void grow(T t)
		{
			m_min -= mathfu::Vector<T, 2>(t);
			m_max += mathfu::Vector<T, 2>(t);
		}

		void grow(const mathfu::Vector<T, 2> &p)
		{
			m_min = mathfu::Vector<T, 2>::Min(m_min, p);
			m_max = mathfu::Vector<T, 2>::Max(m_max, p);
		}

		void grow(const TRect2<T> &box)
		{
			m_min = mathfu::Vector<T, 2>::Min(m_min, box.m_min);
			m_max = mathfu::Vector<T, 2>::Max(m_max, box.m_max);
		}

		void intersect(const TRect2 &box)
		{
			m_min = mathfu::Vector<T, 2>::Max(m_min, box.m_min);
			m_max = mathfu::Vector<T, 2>::Min(m_max, box.m_max);
		}

		Range<T> range(int dimension) const
		{
			return Range<T>(m_min[dimension], m_max[dimension]);
		}

		inline mathfu::Vector<T, 2> diagonal() const
		{
			return mathfu::Vector<T, 2>::Max(m_max - m_min, mathfu::Vector<T, 2>(T(0)));
		}
		
		inline bool empty() const
		{
			for (unsigned i = 0; i < 2; ++i)
				if (m_max[i] <= m_min[i])
					return true;
			return false;
		}

		static const TRect2<T> ZERO;
	};
	using Rect2f = TRect2<Float>;
	using Rect2i = TRect2<int>;

	template <typename T>
	const TRect2<T> TRect2<T>::ZERO(0, 0, 0, 0);
}