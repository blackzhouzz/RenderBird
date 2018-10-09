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
		OutputVisibility = 4,
	};

	struct OutputBufferSettings
	{
		OutputBufferTypeEnum _type;
		std::string _outputDirectory;
		bool _twoBufferVariance;
		bool _sampleVariance;
		OutputBufferSettings()
			: _type(OutputColor),
			_twoBufferVariance(false),
			_sampleVariance(false)
		{
		}

		OutputBufferSettings(OutputBufferTypeEnum type, const std::string& outputDirectory, bool twoBufferVariance, bool sampleVariance)
			: _type(type),
			_outputDirectory(outputDirectory),
			_twoBufferVariance(twoBufferVariance),
			_sampleVariance(sampleVariance)
		{
		}

		void setOutputDirectory(const std::string &directory)
		{
			_outputDirectory = directory;
		}

		void setType(OutputBufferTypeEnum type)
		{
			_type = type;
		}

		OutputBufferTypeEnum type() const
		{
			return _type;
		}

		bool twoBufferVariance() const
		{
			return _twoBufferVariance;
		}

		bool sampleVariance() const
		{
			return _sampleVariance;
		}

		const std::string &outputDirectory() const
		{
			return _outputDirectory;
		}

	};


	template<typename T>
	class OutputBuffer
	{
	public:
		Vector2u _res;

		std::unique_ptr<T[]> _bufferA, _bufferB;
		std::unique_ptr<T[]> _variance;
		std::unique_ptr<uint32[]> _sampleCount;

		const OutputBufferSettings &_settings;

		inline Float average(Float x) const
		{
			return x;
		}
		inline Float average(const Vector3f &x) const
		{
			return (x[0] + x[1] + x[2]) / 3.0;
		}

		const Float *elementPointer(const Float *p) const
		{
			return p;
		}
		const Float *elementPointer(const Vector3f *p) const
		{
			return p->data_;
		}

		int elementCount(Float /*x*/) const
		{
			return 1;
		}
		int elementCount(Vector3f /*x*/) const
		{
			return 3;
		}

		template<typename Texel>
		void saveLdr(const Texel *hdr, const std::string &path, bool rescale) const
		{
			uint32 pixelCount = _res.x * _res.y;
			std::unique_ptr<Vector3c[]> ldr(new Vector3c[pixelCount]);

			Texel minimum, maximum;
			if (_settings.type() == OutputDepth) {
				minimum = maximum = Texel(0.0f);
				for (uint32 i = 0; i < pixelCount; ++i)
					if (average(hdr[i]) != std::numeric_limits<Float>::infinity())
						maximum = Max(maximum, hdr[i]);
			}
			else if (_settings.type() == OutputNormal) {
				minimum = Texel(-1.0f);
				maximum = Texel(1.0f);
			}
			else {
				rescale = false;
			}

			for (uint32 i = 0; i < pixelCount; ++i) {
				Texel f = hdr[i];
				if (rescale)
					f = (f - minimum) / (maximum - minimum);
				if (std::isnan(average(f)) || std::isinf(average(f)))
					ldr[i] = Vector3c(255);
				else
					ldr[i] = Vector3c(Clamp(Vector3i(Vector3f(f*255.0f)), Vector3i(0), Vector3i(255)));
			}

			ImageIO::saveLdr(path, &ldr[0].x, _res.x, _res.y, 3);
		}

	public:
		OutputBuffer(Vector2u res, const OutputBufferSettings &settings)
			: _res(res),
			_settings(settings)
		{
			size_t numPixels = res[0] * res[1];

			_bufferA = zeroAlloc<T>(numPixels);
			if (settings.twoBufferVariance())
				_bufferB = zeroAlloc<T>(numPixels);
			if (settings.sampleVariance())
				_variance = zeroAlloc<T>(numPixels);
			_sampleCount = zeroAlloc<uint32>(numPixels);
		}

		void addSample(Vector2u pixel, T c)
		{
			if (IsNaN(c) || IsInf(c))
				return;

			int idx = pixel.x + pixel.y*_res.x;
			uint32 sampleIdx = _sampleCount[idx]++;
			if (_variance) {
				T curr;
				if (_bufferB && sampleIdx > 0) {
					uint32 sampleCountA = (sampleIdx + 1) / 2;
					uint32 sampleCountB = sampleIdx / 2;
					curr = (_bufferA[idx] * (Float)sampleCountA + _bufferB[idx] * (Float)sampleCountB) / (Float)sampleIdx;
				}
				else {
					curr = _bufferA[idx];
				}
				T delta = c - curr;
				curr += delta / (Float)(sampleIdx + 1);
				_variance[idx] += delta * (c - curr);
			}

			if (_bufferB) {
				T *feature = (sampleIdx & 1) ? _bufferB.get() : _bufferA.get();
				uint32 perBufferSampleCount = sampleIdx / 2 + 1;
				feature[idx] += (c - feature[idx]) / (Float)perBufferSampleCount;
			}
			else {
				_bufferA[idx] += (c - _bufferA[idx]) / (Float)(sampleIdx + 1);
			}
		}

		inline T operator[](uint32 idx) const
		{
			if (_bufferB) {
				uint32 sampleIdx = _sampleCount[idx];
				uint32 sampleCountA = (sampleIdx + 1) / 2;
				uint32 sampleCountB = sampleIdx / 2;
				return (_bufferA[idx] * (Float)sampleCountA + _bufferB[idx] * (Float)sampleCountB) / Float(Max(sampleIdx, uint32(1)));
			}
			else {
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

		//void save(const std::string& path) const
		//{
		//	std::string ldrFile = path;
		//	std::string hdrFile = path	;
		//	std::string ldrVariance = ldrFile + "Variance" + ".bmp";
		//	std::string hdrVariance = hdrFile + "Variance" + ".exr";
		//	std::string ldrFileA = ldrFile + "A" + ".bmp";
		//	std::string hdrFileA = hdrFile + "A" + ".exr";
		//	std::string ldrFileB = ldrFile + "B" + ".bmp";
		//	std::string hdrFileB = hdrFile + "B" + ".exr";

		//	uint32 numPixels = _res.x * _res.y;
		//	if (_bufferB) {
		//		std::unique_ptr<T[]> hdr(new T[numPixels]);
		//		for (uint32 i = 0; i < numPixels; ++i)
		//			hdr[i] = (*this)[i];

		//		if (!hdrFile.empty()) {
		//			ImageIO::saveHdr(hdrFile, elementPointer(hdr.get()), _res.x, _res.y, elementCount(hdr[0]));
		//			ImageIO::saveHdr(hdrFileA, elementPointer(_bufferA.get()), _res.x, _res.y, elementCount(_bufferA[0]));
		//			ImageIO::saveHdr(hdrFileB, elementPointer(_bufferB.get()), _res.x, _res.y, elementCount(_bufferB[0]));
		//		}
		//		//if (!ldrFile.empty()) {
		//		//	saveLdr(hdr.get(), ldrFile, true);
		//		//	saveLdr(_bufferA.get(), ldrFileA, true);
		//		//	saveLdr(_bufferB.get(), ldrFileB, true);
		//		//}
		//	}
		//	else {
		//		if (!hdrFile.empty())
		//			ImageIO::saveHdr(hdrFile, elementPointer(_bufferA.get()), _res.x, _res.y, elementCount(_bufferA[0]));
		//		//if (!ldrFile.empty())
		//		//	saveLdr(_bufferA.get(), ldrFile, true);
		//	}
		//	if (_variance) {
		//		std::unique_ptr<T[]> scaled(new T[numPixels]);
		//		for (uint32 i = 0; i < numPixels; ++i)
		//			scaled[i] = _variance[i] / T(_sampleCount[i] * Max(int(1), _sampleCount[i] - 1));

		//		if (!hdrFile.empty())
		//			ImageIO::saveHdr(hdrVariance, elementPointer(scaled.get()), _res.x, _res.y, elementCount(T(0.0f)));
		//		//if (!ldrFile.empty())
		//		//	saveLdr(scaled.get(), ldrVariance, false);
		//	}
		//}

		//inline T variance(int x, int y) const
		//{
		//	auto data = _variance[x + y * _res.x];
		//	return data / (Float)Max(int(1), _sampleCount[x + y * _res.x] - 1);
		//}
	};

	typedef OutputBuffer<Float> OutputBufferF;
	typedef OutputBuffer<Vector3f> OutputBufferVec3f;

}
