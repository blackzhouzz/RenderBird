#pragma once
#include "core_private.h"
#include "mathcore.h"

namespace Core
{
	template<typename T, int N>
	class TSpectrum
	{
	public:
		TSpectrum() = default;
		TSpectrum<T, N> operator + (const TSpectrum<T, N>& val)const;
		TSpectrum<T, N>& operator += (const TSpectrum<T, N>& val);
		TSpectrum<T, N> operator - (const TSpectrum<T, N>& val)const;
		TSpectrum<T, N>& operator -= (const TSpectrum<T, N>& val);
		TSpectrum<T, N> operator * (const TSpectrum<T, N>& val)const;
		TSpectrum<T, N>& operator *= (const TSpectrum<T, N>& val);
		TSpectrum<T, N> operator * (T val)const;
		TSpectrum<T, N>& operator *= (T val);
		TSpectrum<T, N> operator / (const TSpectrum<T, N>& val)const;
		TSpectrum<T, N>& operator /= (const TSpectrum<T, N>& val);
		bool operator == (const TSpectrum<T, N>& val)const;
		bool operator != (const TSpectrum<T, N>& val)const;
		T operator [] (size_t index)const;
		T& operator [] (size_t i);
		TSpectrum<T, N>& operator = (const TSpectrum<T, N>& spec);
		bool IsZero()const;
		void SetZero();
	protected:
		T m_bands[N];
	};

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator + (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] + val->m_bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator += (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] += val->m_bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator - (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] - val->m_bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator -= (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] -= val->m_bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator * (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] * val->m_bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator *= (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] *= val->m_bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator * (T val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] * val;
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator *= (T val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] *= val;
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator / (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.m_bands[i] = this->m_bands[i] / val->m_bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator /= (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.m_bands[i] /= val->m_bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline bool TSpectrum<T, N>::operator == (const TSpectrum<T, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (m_bands[i] == val.m_bands[i]);
		}
		return ret;
	}

	template<typename T, int N>
	inline bool TSpectrum<T, N>::operator != (const TSpectrum<T, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (m_bands[i] == val.m_bands[i]);
		}
		return !ret;
	}

	template<typename T, int N>
	inline T TSpectrum<T, N>::operator [] (size_t index)const
	{
		return m_bands[index];
	}

	template<typename T, int N>
	inline T& TSpectrum<T, N>::operator [] (size_t index)
	{
		return m_bands[index];
	}

	template<typename T, int N>
	TSpectrum<T, N>& TSpectrum<T, N>::operator = (const TSpectrum<T, N>& spec)
	{
		memcpy(m_bands, spec.m_bands, sizeof(T) * N);
		return *this;
	}

	template<typename T, int N>
	bool TSpectrum<T, N>::IsZero()const
	{
		for (int i = 0; i < N; ++i)
		{
			if (std::abs(m_bands[i]) > C_FLOAT_EPSILON)
				return false;
		}
		return true;
	}

	template<typename T, int N>
	void TSpectrum<T, N>::SetZero()
	{
		for (int i = 0; i < N; ++i)
		{
			m_bands[i] = 0;
		}
	}

	class RGB32 : public TSpectrum<Float, 3>
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

	class RGB8 : public TSpectrum<uint8, 3>
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
		return RGB8((uint8)Clamp(rgb[0] * 255.0, 0.0, 255.0), (uint8)Clamp(rgb[1] * 255.0, 0.0, 255.0), (uint8)Clamp(rgb[2] * 255.0, 0.0, 255.0));
	}

	inline RGB32 RGB8toRGB32(const RGB8& rgb)
	{
		static const Float inv255 = 1.0 / 255.0;
		return RGB32(rgb[0] * inv255, rgb[1] * inv255, rgb[2] * inv255);
	}
}
