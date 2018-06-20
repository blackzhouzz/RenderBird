#pragma once
#include "mathcore.h"

namespace RenderBird
{
	template<typename T, int N>
	class TSpectrum
	{
	public:
		TSpectrum()
		{
		}
		TSpectrum(const TSpectrum<T, N>& spec)
		{
			*this = spec;
		}
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
	private:
		T bands[N];
	};

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator + (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.bands[i] = this->bands[i] + val->bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator += (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.bands[i] += val->bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator - (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.bands[i] = this->bands[i] - val->bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator -= (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.bands[i] -= val->bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator * (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.bands[i] = this->bands[i] * val->bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator *= (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.bands[i] *= val->bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator * (T val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.bands[i] = this->bands[i] * val;
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator *= (T val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.bands[i] *= val;
		}
		return *this;
	}

	template<typename T, int N>
	inline TSpectrum<T, N> TSpectrum<T, N>::operator / (const TSpectrum<T, N>& val)const
	{
		TSpectrum<T, N> ret;
		for (int i = 0; i < N; ++i)
		{
			ret.bands[i] = this->bands[i] / val->bands[i];
		}
		return ret;
	}

	template<typename T, int N>
	inline TSpectrum<T, N>& TSpectrum<T, N>::operator /= (const TSpectrum<T, N>& val)
	{
		for (int i = 0; i < N; ++i)
		{
			this.bands[i] /= val->bands[i];
		}
		return *this;
	}

	template<typename T, int N>
	inline bool TSpectrum<T, N>::operator == (const TSpectrum<T, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (ret.bands[i] == val.bands[i]);
		}
		return ret;
	}

	template<typename T, int N>
	inline bool TSpectrum<T, N>::operator != (const TSpectrum<T, N>& val)const
	{
		bool ret = true;
		for (int i = 0; i < N; ++i)
		{
			ret |= (ret.bands[i] == val.bands[i]);
		}
		return !ret;
	}

	template<typename T, int N>
	inline T TSpectrum<T, N>::operator [] (size_t index)const
	{
		return bands[index];
	}

	template<typename T, int N>
	inline T& TSpectrum<T, N>::operator [] (size_t index)
	{
		return bands[index];
	}

	template<typename T, int N>
	TSpectrum<T, N>& TSpectrum<T, N>::operator = (const TSpectrum<T, N>& spec)
	{
		memcpy(bands, spec.bands, sizeof(T) * N);
	}

	using RGBSpectrum = TSpectrum<Float, 3>;
}
