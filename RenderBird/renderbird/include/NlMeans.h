#pragma once
#include "RenderbirdPrivate.h"
#include "Pixelmap.h"
#include "ThreadUtils.h"

namespace RenderBird
{
	template<typename Texel>
	void boxFilterSlow(const Pixmap<Texel> &src, Pixmap<Texel> &result, int R, Rect2i subImage = Rect2i())
	{
		if (subImage.empty())
			subImage = Rect2i(Vector2i(0), Vector2i(src.GetWidth(), src.GetHeight()));

		int x0 = subImage.m_min.x, x1 = subImage.m_max.x;
		int y0 = subImage.m_min.y, y1 = subImage.m_max.y;

		for (int y = y0; y < y1; ++y) {
			for (int x = x0; x < x1; ++x) {
				Texel sum(0.0f);
				int pixelCount = 0;
				for (int dy = -R; dy <= R; ++dy) {
					for (int dx = -R; dx <= R; ++dx) {
						int xp = x + dx;
						int yp = y + dy;
						if (xp >= x0 && xp < x1 && yp >= y0 && yp < y1) {
							sum += src(xp, yp);
							pixelCount++;
						}
					}
				}

				result(x, y) = sum / (Float)pixelCount;
			}
		}
	}

	template<typename Texel>
	void boxFilter(const Pixmap<Texel> &src, Pixmap<Texel> &tmp, Pixmap<Texel> &result, int R, Rect2i subImage = Rect2i())
	{
		if (subImage.empty())
			subImage = Rect2i(Vector2i(0), Vector2i(src.GetWidth(), src.GetHeight()));

		int x0 = subImage.m_min.x, x1 = subImage.m_max.x;
		int y0 = subImage.m_min.y, y1 = subImage.m_max.y;

		if (x1 - x0 < 2 * R || y1 - y0 < 2 * R) {
			boxFilterSlow(src, result, R, subImage);
			return;
		}

		Float factor = 1.0f / (2 * R + 1);

		for (int y = y0; y < y1; ++y) {
			Texel sumL(0.0f);
			Texel sumR(0.0f);
			for (int x = 0; x < 2 * R; ++x) {
				sumL += src(x0 + x, y);
				sumR += src(x1 - 1 - x, y);
				if (x >= R) {
					tmp(x0 + x - R, y) = sumL / (Float)(x + 1);
					tmp(x1 - 1 - (x - R), y) = sumR / (Float)(x + 1);
				}
			}
			for (int x = x0 + R; x < x1 - R; ++x) {
				sumL += src(x + R, y);
				tmp(x, y) = sumL * factor;
				sumL -= src(x - R, y);
			}
		}

		for (int x = x0; x < x1; ++x) {
			Texel sumL(0.0f);
			Texel sumR(0.0f);
			for (int y = 0; y < 2 * R; ++y) {
				sumL += tmp(x, y0 + y);
				sumR += tmp(x, y1 - 1 - y);
				if (y >= R) {
					result(x, y0 + y - R) = sumL / (Float)(y + 1);
					result(x, y1 - 1 - (y - R)) = sumR / (Float)(y + 1);
				}
			}
			for (int y = y0 + R; y < y1 - R; ++y) {
				sumL += tmp(x, y + R);
				result(x, y) = sumL * factor;
				sumL -= tmp(x, y - R);
			}
		}
	}

	static inline Float fastExp(Float f)
	{
		return std::exp(f);
	}
	static inline Vector3f fastExp(Vector3f f)
	{
		return Vector3f(fastExp(f[0]), fastExp(f[1]), fastExp(f[2]));
	}
	static inline Vector4f fastExp(Vector4f f)
	{
		return Vector4f(fastExp(f[0]), fastExp(f[1]), fastExp(f[2]), fastExp(f[3]));
	}

	template<typename T>
	inline void convertWeight(T &out, const T &in)
	{
		out = in;
	}
	static inline void convertWeight(Float &out, const Vector3f &in)
	{
		out = MinComponent(in);
	}
	template<typename T>
	T sqr(T val)
	{
		return val * val;
	}

	// There is a substantial amount of shared computation when evaluating
	// the NL-means weights of adjancent pixels. This function computes NL-Means
	// weights of an entire rectangle (srcRect) and reuses computation where
	// possible
	template<typename WeightTexel, typename Texel>
	void nlMeansWeights(Pixmap<WeightTexel> &weights, Pixmap<Texel> &distances, Pixmap<Texel> &tmp,
		const Pixmap<Texel> &guide, const Pixmap<Texel> &variance,
		Rect2i srcRect, int F, Float k, int dx, int dy, const Float varianceScale = 1.0f)
	{
		const Float Epsilon = 1e-7f;
		const Float MinCenterWeight = 1e-4f;
		const Float DistanceClamp = 10000.0f;

		Rect2i imageRect(Vector2i(0), Vector2i(guide.GetWidth(), guide.GetHeight()));
		Vector2i delta = Vector2i(dx, dy);

		Rect2i clippedSrc(srcRect.m_min + delta, srcRect.m_max + delta);
		clippedSrc.intersect(imageRect);
		clippedSrc = Rect2i(clippedSrc.m_min - delta, clippedSrc.m_max - delta);

		Rect2i paddedClippedSrc = srcRect;
		paddedClippedSrc.grow(F);
		paddedClippedSrc.intersect(imageRect);
		paddedClippedSrc = Rect2i(paddedClippedSrc.m_min + delta, paddedClippedSrc.m_max + delta);
		paddedClippedSrc.intersect(imageRect);
		paddedClippedSrc = Rect2i(paddedClippedSrc.m_min - delta, paddedClippedSrc.m_max - delta);

		// From Rousselle et al.'s paper
		auto squaredDist = [&](Vector2i p) {
			Vector2i q = p + delta;
			Texel varP = variance[p] * varianceScale;
			Texel varQ = variance[q] * varianceScale;
			Texel squaredDiff = sqr(guide[p] - guide[q]) - (varP + Min(varP, varQ));
			Texel dist = squaredDiff / ((varP + varQ)*k*k + Epsilon);
			return Min(dist, Texel(DistanceClamp));
		};

		for (int y : paddedClippedSrc.range(1))
			for (int x : paddedClippedSrc.range(0))
				distances[Vector2i(x, y) - paddedClippedSrc.m_min] = squaredDist(Vector2i(x, y));

		boxFilter(distances, tmp, distances, F, Rect2i(Vector2i(0), paddedClippedSrc.diagonal()));

		for (int y : clippedSrc.range(1))
			for (int x : clippedSrc.range(0))
				convertWeight(weights[Vector2i(x, y) - srcRect.m_min], fastExp(-Max(distances[Vector2i(x, y) - paddedClippedSrc.m_min], Texel(0.0f))));

		if (dx == 0 && dy == 0)
			for (int y : clippedSrc.range(1))
				for (int x : clippedSrc.range(0))
					weights[Vector2i(x, y) - srcRect.m_min] = Max(weights[Vector2i(x, y) - srcRect.m_min], WeightTexel(MinCenterWeight));
	}

	template<typename Texel>
	Pixmap<Texel> nlMeans(const Pixmap<Texel> &image, const Pixmap<Texel> &guide, const Pixmap<Texel> &variance,
		int F, int R, Float k, const Float varianceScale = 1.0f, bool printProgress = false)
	{
		int w = image.GetWidth();
		int h = image.GetHeight();

		// We parallelize by dicing the input image up into 32x32 tiles
		const int TileSize = 32;
		int padSize = TileSize + 2 * F;

		std::vector<Vector2i> tiles;
		for (int tileY : range(0, h, TileSize))
			for (int tileX : range(0, w, TileSize))
				tiles.emplace_back(tileX, tileY);

		struct PerThreadData { Pixmap<Texel> weights, tmpBufA, tmpBufB; };
		std::vector<std::unique_ptr<PerThreadData>> threadData(ThreadUtils::idealThreadCount());

		Pixmap<Texel> result(w, h), resultWeights(w, h);
		ThreadUtils::startThreads(Max(ThreadUtils::idealThreadCount() - 1, 1u));

		ThreadUtils::pool->enqueue([&](uint32 i, uint32, uint32 threadId) {
			//if (printProgress)
			//	printProgressBar(i, tiles.size());

			if (!threadData[threadId]) {
				threadData[threadId].reset(new PerThreadData{
					Pixmap<Texel>(TileSize, TileSize),
					Pixmap<Texel>(padSize, padSize),
					Pixmap<Texel>(padSize, padSize)
					});
			}
			auto &data = *threadData[threadId];

			Vector2i tile = tiles[i];
			Rect2i tileRect(tile, Vector2i::Min(tile + TileSize, Vector2i(w, h)));

			for (int dy = -R; dy <= R; ++dy) {
				for (int dx = -R; dx <= R; ++dx) {
					Rect2i shiftedRect(Vector2i(-dx, -dy), Vector2i(w - dx, h - dy));
					shiftedRect.intersect(tileRect);

					nlMeansWeights(data.weights, data.tmpBufA, data.tmpBufB, guide, variance, shiftedRect, F, k, dx, dy, varianceScale);

					for (int y : shiftedRect.range(1)) {
						for (int x : shiftedRect.range(0)) {
							Vector2i p(x, y);
							Texel weight = data.weights[p - shiftedRect.m_min];
							result[p] += weight * image[p + Vector2i(dx, dy)];
							resultWeights[p] += weight;
						}
					}
				}
			}
		}, tiles.size())->wait();

		for (int j = 0; j < w*h; ++j)
			result[j] /= resultWeights[j];
		//if (printProgress)
		//	printProgressBar(tiles.size(), tiles.size());

		return std::move(result);
	}

	// This class gathers up 1-channel images and denoises them 4 of them
	// simultaneously by packing 4 images into one SIMD Float. This is
	// useful when denoising feature buffers and yields a ~2x speedup compared
	// to filtering each 1-channel image separately.
	class SimdNlMeans
	{
		struct NlMeansParams
		{
			PixmapF &dst;
			const PixmapF &image, &guide, &variance;
		};

		std::vector<NlMeansParams> _params;

	public:
		void addBuffer(PixmapF &dst, const PixmapF &image, const PixmapF &guide, const PixmapF &variance)
		{
			_params.emplace_back(NlMeansParams{ dst, image, guide, variance });
		}

		void denoise(int F, int R, Float k, Float varianceScale = 1.0f)
		{
			if (_params.empty())
				return;
			int w = _params[0].image.GetWidth();
			int h = _params[0].image.GetHeight();

			Pixmap4f image(w, h), guide(w, h), variance(w, h);

			int numBlocks = (_params.size() + 3) / 4;
			for (size_t i = 0; i < _params.size(); i += 4) {
				int lim = Min(i + 4, _params.size());

				for (int k = i; k < lim; ++k) {
					int idx = k % 4;
					for (int j = 0; j < w*h; ++j) {
						image[j][idx] = _params[k].image[j];
						guide[j][idx] = _params[k].guide[j];
						variance[j][idx] = _params[k].variance[j];
					}
				}

				//printTimestampedLog(tfm::format("Denoising feature set %d/%d", i / 4 + 1, numBlocks));
				image = nlMeans(image, guide, variance, F, R, k, varianceScale, true);

				for (int k = i; k < lim; ++k) {
					_params[k].dst = PixmapF(w, h);
					for (int j = 0; j < w*h; ++j)
						_params[k].dst[j] = image[j][k % 4];
				}
			}

			_params.clear();
		}
	};

}

