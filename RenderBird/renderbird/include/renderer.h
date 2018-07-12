#pragma once
#include <vector>
#include <memory>

namespace RenderBird
{
	enum IntegratorType
	{

	};

	struct RendererSetting
	{
		int m_tileSizeX;
		int m_tileSizeY;
		int m_resolutionX;
		int m_resolutionY;
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

	class Renderer
	{
	public:
		Renderer(const RendererSetting& setting)
			: m_setting(setting)
		{
		}
		void Prepare();
		void Render();
		void Finish();
		const RendererSetting& GetRendererSetting()const;
	private:
		RendererSetting m_setting;
		std::vector<std::shared_ptr<TileRenderer>> m_tileRenderers;
	};
}