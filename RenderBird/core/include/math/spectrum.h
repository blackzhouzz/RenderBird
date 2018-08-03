#pragma once
#include "core_private.h"
#include "mathcore.h"

namespace Core
{
	template<typename T, typename DT, int N>
	class TSpectrum
	{
	public:
		TSpectrum() = default;
		T operator + (const TSpectrum<T, DT, N>& val)const;
		T& operator += (const TSpectrum<T, DT, N>& val);
		T operator - (const TSpectrum<T, DT, N>& val)const;
		T & operator -= (const TSpectrum<T, DT, N>& val);
		T  operator * (const TSpectrum<T, DT, N>& val)const;
		T & operator *= (const TSpectrum<T, DT, N>& val);
		T  operator * (DT val)const;
		T & operator *= (DT val);
		T  operator / (const TSpectrum<T, DT, N>& val)const;
		T & operator /= (const TSpectrum<T, DT, N>& val);
		bool operator == (const TSpectrum<T, DT, N>& val)const;
		bool operator != (const TSpectrum<T, DT, N>& val)const;
		DT operator [] (size_t index)const;
		DT& operator [] (size_t i);
		bool IsZero()const;
		void SetZero();
	protected:
		DT m_bands[N];
	};

	template<typename T, typename DT, int N>
	inline T  TSpectrum<T, DT, N>::operator + (const TSpectrum<T, DT, N>& val)const
	{
		T ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] + val->m_bands[i];
		}
		return ret;
	}

	template<typename T, typename DT, int N>
	inline T& TSpectrum<T, DT, N>::operator += (const TSpectrum<T, DT, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] += val->m_bands[i];
		}
		return *this;
	}

	template<typename T, typename DT, int N>
	inline T TSpectrum<T, DT, N>::operator - (const TSpectrum<T, DT, N>& val)const
	{
		T ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] - val->m_bands[i];
		}
		return ret;
	}

	template<typename T, typename DT, int N>
	inline T& TSpectrum<T, DT, N>::operator -= (const TSpectrum<T, DT, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] -= val->m_bands[i];
		}
		return *this;
	}

	template<typename T, typename DT, int N>
	inline T TSpectrum<T, DT, N>::operator * (const TSpectrum<T, DT, N>& val)const
	{
		T ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] * val->m_bands[i];
		}
		return ret;
	}

	template<typename T, typename DT, int N>
	inline T& TSpectrum<T, DT, N>::operator *= (const TSpectrum<T, DT, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] *= val->m_bands[i];
		}
		return *this;
	}

	template<typename T, typename DT, int N>
	inline T TSpectrum<T, DT, N>::operator * (DT val)const
	{
		T ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] * val;
		}
		return ret;
	}

	template<typename T, typename DT, int N>
	inline T& TSpectrum<T, DT, N>::operator *= (DT val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] *= val;
		}
		return *this;
	}

	template<typename T, typename DT, int N>
	inline T TSpectrum<T, DT, N>::operator / (const TSpectrum<T, DT, N>& val)const
	{
		T ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] / val->m_bands[i];
		}
		return ret;
	}

	template<typename T, typename DT, int N>
	inline T& TSpectrum<T, DT, N>::operator /= (const TSpectrum<T, DT, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] /= val->m_bands[i];
		}
		return *this;
	}

	template<typename T, typename DT, int N>
	inline bool TSpectrum<T, DT, N>::operator == (const TSpectrum<T, DT, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (m_bands[i] == val.m_bands[i]);
		}
		return ret;
	}

	template<typename T, typename DT, int N>
	inline bool TSpectrum<T, DT, N>::operator != (const TSpectrum<T, DT, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (m_bands[i] == val.m_bands[i]);
		}
		return !ret;
	}

	template<typename T, typename DT, int N>
	inline DT TSpectrum<T, DT, N>::operator [] (size_t index)const
	{
		return m_bands[index];
	}

	template<typename T, typename DT, int N>
	inline DT& TSpectrum<T, DT, N>::operator [] (size_t index)
	{
		return m_bands[index];
	}

	template<typename T, typename DT, int N>
	bool TSpectrum<T, DT, N>::IsZero()const
	{
		for (int i = 0; i < N; ++i)
		{
			if (std::abs(m_bands[i]) > C_FLOAT_EPSILON)
				return false;
		}
		return true;
	}

	template<typename T, typename DT, int N>
	void TSpectrum<T, DT, N>::SetZero()
	{
		for (int i = 0; i < N; ++i)
		{
			m_bands[i] = 0;
		}
	}

	class RGB32 : public TSpectrum<RGB32, Float, 3>
	{
	public:
		RGB32() = default;
		RGB32(Float r, Float g, Float b)
		{
			m_bands[0] = r;
			m_bands[1] = g;
			m_bands[2] = b;
		}
		static const RGB32 RED;
		static const RGB32 GREEN;
		static const RGB32 BLUE;
		static const RGB32 BLACK;
		static const RGB32 WHITE;
		static const RGB32 YELLOW;
	};

	class RGB8 : public TSpectrum<RGB8, uint8, 3>
	{
	public:
		RGB8() = default;
		RGB8(uint8 r, uint8 g, uint8 b)
		{
			m_bands[0] = r;
			m_bands[1] = g;
			m_bands[2] = b;
		}
		static const RGB8 RED;
		static const RGB8 GREEN;
		static const RGB8 BLUE;
		static const RGB8 BLACK;
		static const RGB8 WHITE;
		static const RGB8 YELLOW;
	};

	inline RGB8 RGB32toRGB8(const RGB32& rgb)
	{
		return RGB8(
			(uint8)Clamp(rgb[0] * 255.0, 0.0, 255.0),
			(uint8)Clamp(rgb[1] * 255.0, 0.0, 255.0),
			(uint8)Clamp(rgb[2] * 255.0, 0.0, 255.0)
		);
	}

	inline RGB32 RGB8toRGB32(const RGB8& rgb)
	{
		static const Float inv255 = 1.0 / 255.0;
		return RGB32(rgb[0] * inv255, rgb[1] * inv255, rgb[2] * inv255);
	}

	class RGBA32 : public TSpectrum<RGBA32, Float, 4>
	{
	public:
		RGBA32() = default;
		RGBA32(Float r, Float g, Float b, Float a)
		{
			m_bands[0] = r;
			m_bands[1] = g;
			m_bands[2] = b;
			m_bands[3] = a;
		}
		static const RGBA32 RED;
		static const RGBA32 GREEN;
		static const RGBA32 BLUE;
		static const RGBA32 BLACK;
		static const RGBA32 WHITE;
		static const RGBA32 YELLOW;
	};

	class RGBA8 : public TSpectrum<RGBA8, uint8, 4>
	{
	public:
		RGBA8() = default;
		RGBA8(uint8 r, uint8 g, uint8 b, uint8 a)
		{
			m_bands[0] = r;
			m_bands[1] = g;
			m_bands[2] = b;
			m_bands[3] = a;
		}
		static const RGBA8 RED;
		static const RGBA8 GREEN;
		static const RGBA8 BLUE;
		static const RGBA8 BLACK;
		static const RGBA8 WHITE;
		static const RGBA8 YELLOW;
	};

	inline RGBA8 RGBA32toRGBA8(const RGBA32& rgb)
	{
		return RGBA8(
			(uint8)Clamp(rgb[0] * 255.0, 0.0, 255.0),
			(uint8)Clamp(rgb[1] * 255.0, 0.0, 255.0),
			(uint8)Clamp(rgb[2] * 255.0, 0.0, 255.0),
			(uint8)Clamp(rgb[3] * 255.0, 0.0, 255.0)
		);
	}

	inline RGBA32 RGBA8toRGBA32(const RGBA8& rgb)
	{
		static const Float inv255 = 1.0 / 255.0;
		return RGBA32(rgb[0] * inv255, rgb[1] * inv255, rgb[2] * inv255, rgb[3] * inv255);
	}
}
