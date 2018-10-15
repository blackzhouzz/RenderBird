#pragma once
#include <vector>
#include <memory>
#include "ImageIO.h"
#include "PathTracing.h"
#include "Scene.h"
#include "AuxiliaryBuffer.h"
#include "Denoising.h"
#include "Sampler.h"

namespace RenderBird
{
	struct RendererSetting
	{
		int m_tileSizeX;
		int m_tileSizeY;
		int m_resX;
		int m_resY;
		bool m_useJob;
		int m_maxBounce;
		int m_rrBounce;
		int m_numSamples;
		Float m_clampValue;
		bool m_enableClamp;
		bool m_useMis;
		bool m_denoising;
	};
	class Renderer;
	class TileRenderer
	{
	public:
		TileRenderer(Renderer* renderer, int x, int y, int sizeX, int sizeY);
		void Render();
		void Finish();
		const Vector2i& GetBoundMin()const { return m_boundMin; }
		const Vector2i& GetBoundMax()const { return m_boundMax; }
	public:
		Renderer* m_renderer;
		int m_sizeX;
		int m_sizeY;
		int m_x;
		int m_y;
		Vector2i m_boundMin;
		Vector2i m_boundMax;
		std::unique_ptr<Sampler> m_sampler;
	};

	struct CameraData
	{
		Matrix4f m_worldToCamera;
		Matrix4f m_cameraToWorld;
		Matrix4f m_worldToScreen;
		Matrix4f m_rasterToCamera;
		Rect2f m_screenBound;
	};

	struct RenderContext
	{
		CameraData m_camera;
	};

	class Renderer : public Singleton<Renderer>
	{
		friend class TileRenderer;
	public:
		Renderer();
		void Prepare();
		void Render();
		void Finish();
		void Denoising();
		void ExtractFeatures3f(OutputBufferVec3f* buf, std::vector<RenderBufferF>& features);
		void ExtractFeatures1f(OutputBufferF* buf, std::vector<RenderBufferF>& features);
		bool IsInBound(int x, int y)const;
		const RendererSetting& GetRendererSetting()const;
		void GenerateCameraRay(Float pixelX, Float pixelY, Ray* ray);
		void InitBuffers();
	public:
		RendererSetting m_setting;
		std::vector<TileRenderer*> m_tileRenderers;
		PathTracing* m_integrator;
		RenderContext m_renderContext;
	public:
		Scene* m_scene;
		std::unique_ptr<Sampler> m_sampler;
		AuxiliaryBuffer m_buffers;
	};
}