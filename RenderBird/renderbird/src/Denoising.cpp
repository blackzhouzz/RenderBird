#include "Denoising.h"
#include "ThreadUtils.h"

namespace RenderBird
{
	Pixmap3f collaborativeRegression(const Pixmap3f &image, const Pixmap3f &guide,
		const std::vector<PixmapF> &features, const Pixmap3f &imageVariance,
		int F, int R, Float k)
	{
		int w = image.GetWidth();
		int h = image.GetHeight();
		int d = features.size() + 3;

		// We parallellize by dicing up the image into 32x32 tiles
		const int TileSize = 32;
		int padSize = TileSize + 2 * F;

		struct Tile
		{
			Vector2i pos;
			Rect2i dstRect;
			Pixmap3f result;
			PixmapF resultWeights;
			Tile(int x, int y) : pos(x, y) {}
		};

		std::vector<Tile> tiles;
		for (int tileY : range(0, h, TileSize))
			for (int tileX : range(0, w, TileSize))
				tiles.emplace_back(tileX, tileY);

		struct PerThreadData { Pixmap3f tmpBufA, tmpBufB; std::vector<PixmapF> weights; };
		std::vector<std::unique_ptr<PerThreadData>> threadData(ThreadUtils::idealThreadCount());

		ThreadUtils::pool->enqueue([&](uint32 i, uint32, uint32 threadId) {
			//printProgressBar(i, tiles.size());

			if (!threadData[threadId]) {
				threadData[threadId].reset(new PerThreadData());
				threadData[threadId]->tmpBufA = Pixmap3f(padSize, padSize);
				threadData[threadId]->tmpBufB = Pixmap3f(padSize, padSize);
				for (int i = 0; i < (2 * R + 1)*(2 * R + 1); ++i)
					threadData[threadId]->weights.emplace_back(TileSize, TileSize);
			}
			auto &data = *threadData[threadId];
			Tile &tile = tiles[i];

			for (auto &w : data.weights)
				w.Clear();

			Rect2i srcRect(tile.pos, Vector2i::Min(tile.pos + TileSize, Vector2i(w, h)));
			tile.dstRect = srcRect;
			tile.dstRect.grow(R);
			tile.dstRect.intersect(Rect2i(Vector2i(0), Vector2i(w, h)));
			int dstW = tile.dstRect.diagonal().x, dstH = tile.dstRect.diagonal().y;

			tile.result = Pixmap3f(dstW, dstH);
			tile.resultWeights = PixmapF(dstW, dstH);

			// Precompute weights for entire tile
			for (int dy = -R, idxW = 0; dy <= R; ++dy)
				for (int dx = -R; dx <= R; ++dx, ++idxW)
					nlMeansWeights(data.weights[idxW], data.tmpBufA, data.tmpBufB, guide, imageVariance,
						srcRect, F, k, dx, dy, 2.0f);

			for (int y = srcRect.m_min.y; y < srcRect.m_max.y; ++y) {
				for (int x = srcRect.m_min.x; x < srcRect.m_max.x; ++x) {
					int x0 = max(x - R, 0), x1 = min(w, x + R + 1);
					int y0 = max(y - R, 0), y1 = min(h, y + R + 1);
					int n = (x1 - x0)*(y1 - y0);

					// Build weight matrix (W), feature matrix (X) and RHS (Y)
					Eigen::VectorXf W(n);
					Eigen::MatrixXf Y(n, 3);
					Eigen::MatrixXf X(n, d);

					for (int iy = y0; iy < y1; ++iy) {
						for (int ix = x0; ix < x1; ++ix) {
							int idxP = ix + iy * w;
							int idx = (ix - x0) + (iy - y0)*(x1 - x0);

							for (int i = 0; i < 3; ++i)
								Y(idx, i) = image[idxP][i];

							X(idx, 0) = 1.0f;
							X(idx, 1) = ix - x;
							X(idx, 2) = iy - y;
							for (size_t i = 0; i < features.size(); ++i)
								X(idx, i + 3) = features[i][idxP] - features[i][x + y * w];

							int idxW = (ix - x + R) + (iy - y + R)*(2 * R + 1);
							W[idx] = data.weights[idxW][Vector2i(x, y) - tile.pos];
						}
					}

					// Solve least squares system
					Eigen::VectorXf wSqrt = W.cwiseSqrt();
					Eigen::MatrixXf denoised = X * (wSqrt.asDiagonal()*X).colPivHouseholderQr().solve(wSqrt.asDiagonal()*Y);

					// Accumulate denoised patch into image
					for (int iy = y0; iy < y1; ++iy) {
						for (int ix = x0; ix < x1; ++ix) {
							Vector2i p = Vector2i(ix, iy) - tile.dstRect.m_min;
							int idx = (ix - x0) + (iy - y0)*(x1 - x0);
							Vector3f tmp;
							tmp[0] = denoised(idx, 0) * W[idx];
							tile.result[p] += Vector3f(denoised(idx, 0) * W[idx], denoised(idx, 1) * W[idx], denoised(idx, 2) * W[idx]);
							tile.resultWeights[p] += W[idx];
						}
					}
				}
			}

		}, tiles.size())->wait();

		// Gather results from all threads and divide by weights
		Pixmap3f result(w, h);
		PixmapF resultWeights(w, h);
		for (const auto &tile : tiles) {
			for (int y : tile.dstRect.range(1)) {
				for (int x : tile.dstRect.range(0)) {
					Vector2i p(x, y);
					result[p] += tile.result[p - tile.dstRect.m_min];
					resultWeights[p] += tile.resultWeights[p - tile.dstRect.m_min];
				}
			}
		}
		for (int j = 0; j < w*h; ++j)
		{
			result[j][0] /= resultWeights[j];
			result[j][1] /= resultWeights[j];
			result[j][2] /= resultWeights[j];
		}

		//printProgressBar(tiles.size(), tiles.size());

		return std::move(result);
	}

	Pixmap3f nforDenoiser(RenderBuffer3f image, std::vector<RenderBufferF> features)
	{
		int w = image.buffer->GetWidth(), h = image.buffer->GetHeight();

		// Feature cross-prefiltering (section 5.1)
		//printTimestampedLog("Prefiltering features...");
		std::vector<PixmapF> filteredFeaturesA(features.size());
		std::vector<PixmapF> filteredFeaturesB(features.size());
		SimdNlMeans featureFilter;
		for (size_t i = 0; i < features.size(); ++i) {
			featureFilter.addBuffer(filteredFeaturesA[i], *features[i].bufferA, *features[i].bufferB, *features[i].bufferVariance);
			featureFilter.addBuffer(filteredFeaturesB[i], *features[i].bufferB, *features[i].bufferA, *features[i].bufferVariance);
		}
		featureFilter.denoise(3, 5, 0.5f, 2.0f);
		features.clear();
		//printTimestampedLog("Prefiltering done");

		// Main regression (section 5.2)
		std::vector<Pixmap3f> filteredColorsA;
		std::vector<Pixmap3f> filteredColorsB;
		std::vector<Pixmap3f> mses;
		for (Float k : {0.5f, 1.0f}) {
			//printTimestampedLog(tfm::format("Beginning regression pass %d/2", mses.size() + 1));
			// Regression pass
			//printTimestampedLog("Denosing half buffer A...");
			Pixmap3f filteredColorA = collaborativeRegression(*image.bufferA, *image.bufferB, filteredFeaturesB, *image.bufferVariance, 3, 9, k);
			//printTimestampedLog("Denosing half buffer B...");
			Pixmap3f filteredColorB = collaborativeRegression(*image.bufferB, *image.bufferA, filteredFeaturesA, *image.bufferVariance, 3, 9, k);

			// MSE estimation (section 5.3)
			//printTimestampedLog("Estimating MSE...");
			Pixmap3f noisyMse(w, h);
			for (int i = 0; i < w*h; ++i) {
				Vector3f mseA = sqr((*image.bufferB)[i] - filteredColorA[i]) - (*image.bufferVariance)[i] * (Float)2.0;
				Vector3f mseB = sqr((*image.bufferA)[i] - filteredColorB[i]) - (*image.bufferVariance)[i] * (Float)2.0;
				Vector3f residualColorVariance = sqr(filteredColorB[i] - filteredColorA[i]) * (Float)0.25;

				noisyMse[i] = (mseA + mseB) * (Float)0.5 - residualColorVariance;
			}
			filteredColorsA.emplace_back(std::move(filteredColorA));
			filteredColorsB.emplace_back(std::move(filteredColorB));

			// MSE filtering
			mses.emplace_back(nlMeans(noisyMse, *image.buffer, *image.bufferVariance, 1, 9, 1.0f, 1.0f, true));
		}
		//printTimestampedLog("Regression pass done");

		// Bandwidth selection (section 5.3)
		// Generate selection map
		//printTimestampedLog("Generating selection maps...");
		Pixmap3f noisySelection(w, h);
		for (int i = 0; i < w*h; ++i)
			for (int j = 0; j < 3; ++j)
				noisySelection[i][j] = mses[0][i][j] < mses[1][i][j] ? 0.0f : 1.0f;
		mses.clear();
		// Filter selection map
		Pixmap3f selection = nlMeans(noisySelection, *image.buffer, *image.bufferVariance, 1, 9, 1.0f, 1.0f, true);

		// Apply selection map
		Pixmap3f resultA(w, h);
		Pixmap3f resultB(w, h);
		for (int i = 0; i < w*h; ++i) {
			resultA[i] += lerp(filteredColorsA[0][i], filteredColorsA[1][i], selection[i]);
			resultB[i] += lerp(filteredColorsB[0][i], filteredColorsB[1][i], selection[i]);
		}
		selection.Reset();
		filteredColorsA.clear();
		filteredColorsB.clear();

		// Second filter pass (section 5.4)
		//printTimestampedLog("Beginning second filter pass");
		//printTimestampedLog("Denoising final features...");
		std::vector<PixmapF> finalFeatures;
		for (size_t i = 0; i < filteredFeaturesA.size(); ++i) {
			PixmapF combinedFeature(w, h);
			PixmapF combinedFeatureVar(w, h);

			for (int j = 0; j < w*h; ++j) {
				combinedFeature[j] = (filteredFeaturesA[i][j] + filteredFeaturesB[i][j])*0.5f;
				combinedFeatureVar[j] = sqr(filteredFeaturesB[i][j] - filteredFeaturesA[i][j])*0.25f;
			}
			filteredFeaturesA[i].Reset();
			filteredFeaturesB[i].Reset();

			finalFeatures.emplace_back(nlMeans(combinedFeature, combinedFeature, combinedFeatureVar, 3, 2, 0.5f));
		}

		Pixmap3f combinedResult(w, h);
		Pixmap3f combinedResultVar(w, h);
		for (int j = 0; j < w*h; ++j) {
			combinedResult[j] = (resultA[j] + resultB[j])*(Float)0.5;
			combinedResultVar[j] = sqr(resultB[j] - resultA[j])*(Float)0.25;
		}
		//printTimestampedLog("Performing final regression...");
		return collaborativeRegression(combinedResult, combinedResult, finalFeatures, combinedResultVar, 3, 9, 1.0f);
	}

}