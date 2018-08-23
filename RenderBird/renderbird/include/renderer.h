#pragma once
#include <vector>
#include <memory>
#include "image_output.h"
#include "pathtracing.h"
#include "scene.h"

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
		int m_numSamples;
		bool m_useMis;
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
	private:
		Renderer* m_renderer;
		int m_sizeX;
		int m_sizeY;
		int m_x;
		int m_y;
		Vector2i m_boundMin;
		Vector2i m_boundMax;
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
		bool IsInBound(int x, int y)const;
		const RendererSetting& GetRendererSetting()const;
		void AddSample(const Vector2f& uv, const Vector2i& boundMin, const Vector2i& boundMax, const Radiance& L);
		PixelData& GetPixelDataAt(int pixelX, int pixelY);
		void GenerateCameraRay(const CameraSample& cameraSample, Ray* ray);
	public:
		RendererSetting m_setting;
		std::vector<TileRenderer*> m_tileRenderers;
		PixelData* m_data;
		PathTracing* m_integrator;
		RenderContext m_renderContext;
	public:
		Scene* m_scene;
	};
}