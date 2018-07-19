#include "renderer.h"
#include "random.hpp"
using namespace effolkronium;

namespace RenderBird
{
	void TileRenderer::Render()
	{
		static Core::RGB32 randomColorList[6];

		randomColorList[0] = Core::RGB32::WHITE;
		randomColorList[1] = Core::RGB32::BLACK;
		randomColorList[2] = Core::RGB32::YELLOW;
		randomColorList[3] = Core::RGB32::BLUE;
		randomColorList[4] = Core::RGB32::RED;
		randomColorList[5] = Core::RGB32::GREEN;
		auto random_number = random_static::get(0, 5);
		static int index = 0;
		int startPixelX = m_x * m_sizeX;
		int endPixelX = (m_x + 1) * m_sizeX;
		int startPixelY = m_y * m_sizeY;
		int endPixelY = (m_y + 1) * m_sizeY;
		for (int pixelY = startPixelY; pixelY < endPixelY; ++pixelY)
		{
			for (int pixelX = startPixelX; pixelX < endPixelX; ++pixelX)
			{
				if (!m_renderer->IsInBound(pixelX, pixelY))
				{
					continue;
				}
				Ray ray;
				m_renderer->GenerateCameraRay(pixelX, pixelY, &ray);
				RayHitInfo hitInfo;
				if (m_renderer->m_scene->Intersect(ray, &hitInfo))
				{
					m_renderer->SetColor(pixelX, pixelY, Core::RGB32::WHITE);
				}
				else
				{
					m_renderer->SetColor(pixelX, pixelY, Core::RGB32::BLACK);
				}
			}
		}
		index++;
	}

	void TileRenderer::Finish()
	{
	}

	const RendererSetting& Renderer::GetRendererSetting()const
	{
		return m_setting;
	}

	Renderer::Renderer(const RendererSetting& setting)
		: m_setting(setting)
		, m_integrator(nullptr)
		, m_scene(new Scene())
	{
		m_scene->SetupTestScene();
	}

	bool Renderer::IsInBound(int x, int y)const
	{
		return x >= 0 && x < m_setting.m_resX && y >= 0 && y < m_setting.m_resY;
	}

	void Renderer::Prepare()
	{
		int numX = (m_setting.m_resX - 1) / m_setting.m_tileSizeX + 1;
		int numY = (m_setting.m_resY - 1) / m_setting.m_tileSizeY + 1;
		for (int x = 0; x < numX; ++x)
		{
			for (int y = 0; y < numY; ++y)
			{
				std::shared_ptr<TileRenderer> tileRenderer = std::shared_ptr<TileRenderer>(new TileRenderer(this, x, y, m_setting.m_tileSizeX, m_setting.m_tileSizeY));
				m_tileRenderers.push_back(tileRenderer);
			}
		}
		m_data = new Core::RGB32[m_setting.m_resX * m_setting.m_resY];

		PerspectiveCamera* m_camera = new PerspectiveCamera();
		const Float dist = 10;
		Vector3f pos = Vector3f(0, 0, 1) * dist;
		m_camera->LookAt(pos, Vector3f::ZERO, Vector3f::UNIT_Z);
		m_camera->PerspectiveFovLH(39.0f, 1.0, 1e-2f, 1000.f);
		//OrthoCamera* m_camera = new OrthoCamera();
		//const Float dist = 5.0;
		//Vector3f pos = Vector3f(0, 0, 1) * dist;
		//m_camera->LookAt(pos, Vector3f::ZERO, Vector3f::UNIT_Z);
		//m_camera->OrthoLH(m_setting.m_resX, m_setting.m_resY, 0, 100);

		m_renderContext.m_camera.m_screenBound = Rect2f(-1.0, -1.0, 1.0, 1.0);
		m_renderContext.m_camera.m_cameraToWorld = m_camera->GetViewMatrix().InverseSafe();
		Matrix4f screenToRaster = MathUtils::ScreenToRasterMatrix(m_renderContext.m_camera.m_screenBound, Point2i(m_setting.m_resX, m_setting.m_resY));
		Matrix4f cameraToRaster = m_camera->GetProjMatrix() * screenToRaster;
		Vector3f vec = Vector3f(0, 0, 0);
		m_renderContext.m_camera.m_rasterToCamera = cameraToRaster.InverseSafe();
		vec = m_renderContext.m_camera.m_rasterToCamera.TransformDirection(vec);


		m_renderContext.m_camera.m_rasterToWorld = (m_camera->GetViewMatrix() * m_camera->GetProjMatrix() * screenToRaster).InverseSafe();
	}

	void Renderer::GenerateCameraRay(int pixelX, int pixelY, Ray* ray)
	{
		Vector3f rayDir = m_renderContext.m_camera.m_rasterToCamera.TransformPerspective(Vector3f(pixelX, pixelY, 0));
		rayDir = m_renderContext.m_camera.m_cameraToWorld.TransformDirection(rayDir);
		rayDir.Normalize();

		Vector3f rayDir1 = m_renderContext.m_camera.m_rasterToWorld.TransformPerspective(Vector3f(pixelX, pixelY, 0));
		rayDir1.Normalize();

		Vector3f rayPos = Vector3f::ZERO;
		rayPos = m_renderContext.m_camera.m_cameraToWorld.TransformPoint(rayPos);
		ray->origin = rayPos;
		ray->direction = rayDir1;
	}

	void Renderer::Render()
	{
		for (int i = 0; i < m_tileRenderers.size();++i)
		{
			m_tileRenderers[i]->Render();
		}

		ImageOutput::WriteBMP("c:/test.bmp", m_data, m_setting.m_resX, m_setting.m_resY);
	}

	void Renderer::Finish()
	{
		for (int i = 0; i < m_tileRenderers.size();++i)
		{
			m_tileRenderers[i]->Finish();
		}
		m_tileRenderers.clear();
		delete[] m_data;
	}

	void Renderer::SetColor(int pixelX, int pixelY, const Core::RGB32& color)
	{
		int index = pixelY * m_setting.m_resX + pixelX;
		m_data[index] = color;
	}

	Core::RGB32 Renderer::GetColor(int pixelX, int pixelY)const
	{
		int index = pixelY * m_setting.m_resX + pixelX;
		return m_data[index];
	}
}