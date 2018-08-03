#pragma once
#include <vector>
#include <memory>
#include "image_output.h"
#include "math/spectrum.h"
#include "camera.h"
#include "pathtracing.h"
#include "math/rect2.h"
#include "scene.h"

namespace RenderBird
{
	struct RendererSetting
	{
		int m_tileSizeX;
		int m_tileSizeY;
		int m_resX;
		int m_resY;
	};
	class Renderer;

	class TileRenderer
	{
	public:
		TileRenderer(Renderer* renderer, int x, int y, int sizeX, int sizeY)
			: m_renderer(renderer), m_x(x), m_y(y), m_sizeX(sizeX), m_sizeY(sizeY)
		{
		}
		void Render();
		void Finish();
	private:
		Renderer* m_renderer;
		int m_sizeX;
		int m_sizeY;
		int m_x;
		int m_y;
	};

	struct CameraData
	{
		Matrix4f m_worldToCamera;
		Matrix4f m_cameraToWorld;
		Matrix4f m_rasterToWorld;
		Matrix4f m_worldToScreen;
		Matrix4f m_rasterToCamera;
		Rect2f m_screenBound;
	};

	struct RenderContext
	{
		CameraData m_camera;

	};

	class Renderer
	{
		friend class TileRenderer;
	public:
		Renderer(const RendererSetting& setting);
		void Prepare();
		void Render();
		void Finish();
		bool IsInBound(int x, int y)const;
		const RendererSetting& GetRendererSetting()const;
		void SetColor(int pixelX, int pixelY, const Core::RGB32& color);
		Core::RGB32 GetColor(int pixelX, int pixelY)const;
		void GenerateCameraRay(int pixelX, int pixelY, Ray* ray);
	private:
		RendererSetting m_setting;
		std::vector<std::shared_ptr<TileRenderer>> m_tileRenderers;
		Core::RGB32* m_data;
		PathTracing* m_integrator;
		RenderContext m_renderContext;
	public:
		Scene* m_scene;
	};
}