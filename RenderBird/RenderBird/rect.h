#pragma once

namespace Luna
{
	template<typename T>
	class TRect
	{
	public:
		T mLeft;
		T mTop;
		T mRight;
		T mBottom;
	public:
		TRect() {}
		TRect(T left, T top, T right, T bottom)
			: mLeft(left), mTop(top), mRight(right), mBottom(bottom)
		{}

		bool operator == (const TRect<T>& rect) const
		{
			return 
				mLeft	== rect.mLeft && 
				mTop	== rect.mTop && 
				mRight	== rect.mRight && 
				mBottom	== rect.mBottom;
		}
		bool operator != (const TRect<T>& rect) const
		{
			return 
				mLeft	!= rect.mLeft || 
				mTop	!= rect.mTop || 
				mRight	!= rect.mRight || 
				mBottom != rect.mBottom;
		}
		static const TRect<T> ZERO;
	};
	typedef TRect<float> RectFloat;
	typedef TRect<int> RectInt;
	const RectFloat RectFloat::ZERO = RectFloat(0,0,0,0);
	const RectInt RectInt::ZERO = RectInt(0,0,0,0);
}