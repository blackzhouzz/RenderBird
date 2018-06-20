#include "renderer.h"

namespace RenderBird
{
	void TileRenderer::Render()
	{
	}

	void TileRenderer::Finish()
	{
	}

	const RendererSetting& Renderer::GetRendererSetting()const
	{
		return m_setting;
	}

	void Renderer::Prepare()
	{
		m_tileRenderers.clear();
		int numX = (m_setting.m_resolutionX - 1) / m_setting.m_tileSizeX + 1;
		int numY = (m_setting.m_resolutionY - 1) / m_setting.m_tileSizeY + 1;
		for (int x = 0; x < numX; ++x)
		{
			for (int y = 0; y < numY; ++y)
			{
				std::shared_ptr<TileRenderer> tileRenderer = std::shared_ptr<TileRenderer>(new TileRenderer(this, x, y, m_setting.m_tileSizeX, m_setting.m_tileSizeY));
				m_tileRenderers.push_back(tileRenderer);
			}
		}
	}

	void Renderer::Render()
	{
		for (int i = 0; i < m_tileRenderers.size();++i)
		{
			m_tileRenderers[i]->Render();
		}
	}

	void Renderer::Finish()
	{
		for (int i = 0; i < m_tileRenderers.size();++i)
		{
			m_tileRenderers[i]->Finish();
		}
	}
}