#pragma once
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
	private:
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
			ret |= (ret.m_bands[i] == val.m_bands[i]);
		}
		return ret;
	}

	template<typename T, int N>
	inline bool TSpectrum<T, N>::operator != (const TSpectrum<T, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (ret.m_bands[i] == val.m_bands[i]);
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

	class RGBSpectrum : public TSpectrum<Float, 3>
	{

	};
}
