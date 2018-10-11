#pragma once

#include "RenderbirdPrivate.h"
#include "ImageIO.h"
#include <memory>

namespace RenderBird
{
	enum OutputBufferTypeEnum
	{
		OutputColor = 0,
		OutputDepth = 1,
		OutputNormal = 2,
		OutputAlbedo = 3,
	};

	template<typename T>
	class OutputBuffer
	{
	public:
		Vector2u _res;

		std::unique_ptr<T[]> _bufferA, _bufferB;
		std::unique_ptr<T[]> _variance;
		std::unique_ptr<uint32[]> _sampleCount;

		OutputBufferTypeEnum _type;
		bool _twoBufferVariance;
		bool _sampleVariance;

	public:
		OutputBuffer(Vector2u res, OutputBufferTypeEnum type, bool twoBufferVariance, bool sampleVariance)
			: _res(res)
			, _type(type)
			, _twoBufferVariance(twoBufferVariance)
			, _sampleVariance(sampleVariance)
		{
			size_t numPixels = res[0] * res[1];

			_bufferA = zeroAlloc<T>(numPixels);
			if (twoBufferVariance)
				_bufferB = zeroAlloc<T>(numPixels);
			if (sampleVariance)
				_variance = zeroAlloc<T>(numPixels);
			_sampleCount = zeroAlloc<uint32>(numPixels);
		}

		void addSample(Vector2u pixel, T c)
		{
			if (IsNaN(c) || IsInf(c))
				return;

			int idx = pixel.x + pixel.y*_res.x;
			uint32 sampleIdx = _sampleCount[idx]++;
			if (_variance) 
			{
				T curr;
				if (_bufferB && sampleIdx > 0) 
				{
					uint32 sampleCountA = (sampleIdx + 1) / 2;
					uint32 sampleCountB = sampleIdx / 2;
					curr = (_bufferA[idx] * (Float)sampleCountA + _bufferB[idx] * (Float)sampleCountB) / (Float)sampleIdx;
				}
				else 
				{
					curr = _bufferA[idx];
				}
				T delta = c - curr;
				curr += delta / (Float)(sampleIdx + 1);
				_variance[idx] += delta * (c - curr);
			}

			if (_bufferB) 
			{
				T *feature = (sampleIdx & 1) ? _bufferB.get() : _bufferA.get();
				uint32 perBufferSampleCount = sampleIdx / 2 + 1;
				feature[idx] += (c - feature[idx]) / (Float)perBufferSampleCount;
			}
			else 
			{
				_bufferA[idx] += (c - _bufferA[idx]) / (Float)(sampleIdx + 1);
			}
		}

		inline T operator[](uint32 idx) const
		{
			if (_bufferB) 
			{
				uint32 sampleIdx = _sampleCount[idx];
				uint32 sampleCountA = (sampleIdx + 1) / 2;
				uint32 sampleCountB = sampleIdx / 2;
				return (_bufferA[idx] * (Float)sampleCountA + _bufferB[idx] * (Float)sampleCountB) / Float(Max(sampleIdx, uint32(1)));
			}
			else 
			{
				return _bufferA[idx];
			}
		}

		std::unique_ptr<T[]> GetBuffer()
		{
			uint32 numPixels = _res.x * _res.y;
			std::unique_ptr<T[]> hdr(new T[numPixels]);
			for (uint32 i = 0; i < numPixels; ++i)
				hdr[i] = (*this)[i];
			return std::move(hdr);
		}

		std::unique_ptr<T[]> GetVariance()
		{
			uint32 numPixels = _res.x * _res.y;
			std::unique_ptr<T[]> scaled(new T[numPixels]);
			for (uint32 i = 0; i < numPixels; ++i)
				scaled[i] = _variance[i] / T(_sampleCount[i] * Max(uint32(1), _sampleCount[i] - 1));
			return std::move(scaled);
		}
	};

	typedef OutputBuffer<Float> OutputBufferF;
	typedef OutputBuffer<Vector3f> OutputBufferVec3f;

}
