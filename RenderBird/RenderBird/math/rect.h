#pragma once

namespace MathLib
{
	template<typename T>
	class TRect
	{
	public:
		T left;
		T top;
		T right;
		T bottom;
	public:
		TRect() {}
		TRect(T l, T t, T r, T b)
			: left(l), top(t), right(r), bottom(b)
		{
		}

		bool operator == (const TRect<T>& rect) const
		{
			return left == rect.left &&
				top == rect.top &&
				right == rect.right &&
				bottom == rect.bottom;
		}
		bool operator != (const TRect<T>& rect) const
		{
			return left != rect.left ||
				top != rect.top ||
				right != rect.right ||
				bottom != rect.bottom;
		}
		static const TRect<T> ZERO;
	};
	using Rectf = TRect<float>;
	using Recti = TRect<int>;

	template <typename T>
	const TRect<T> TRect<T>::ZERO(0, 0, 0, 0);
}