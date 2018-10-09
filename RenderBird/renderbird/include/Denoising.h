#pragma once
#include "RenderbirdPrivate.h"
#include <Eigen/Dense>
#include "Pixelmap.h"
#include "NlMeans.h"
namespace RenderBird
{
	template<typename Texel>
	struct RenderBuffer
	{
		std::unique_ptr<Pixmap<Texel>> buffer;
		std::unique_ptr<Pixmap<Texel>> bufferA;
		std::unique_ptr<Pixmap<Texel>> bufferB;
		std::unique_ptr<Pixmap<Texel>> bufferVariance;
	};
	typedef RenderBuffer<Float> RenderBufferF;
	typedef RenderBuffer<Vector3f> RenderBuffer3f;

	Pixmap3f collaborativeRegression(const Pixmap3f &image, const Pixmap3f &guide,
		const std::vector<PixmapF> &features, const Pixmap3f &imageVariance,
		int F, int R, Float k);
	Pixmap3f nforDenoiser(RenderBuffer3f image, std::vector<RenderBufferF> features);
}