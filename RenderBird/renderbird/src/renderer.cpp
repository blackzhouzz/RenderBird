#include "renderer.h"
#include "camera_component.h"
#include "trianglemesh.h"
#include "material.h"
#include <fjs/Manager.h>
#include <fjs/Counter.h>
#include <fjs/List.h>
#include <fjs/Queue.h>

namespace RenderBird
{
	TileRenderer::TileRenderer(Renderer* renderer, int x, int y, int sizeX, int sizeY)
		: m_renderer(renderer), m_x(x), m_y(y), m_sizeX(sizeX), m_sizeY(sizeY)
	{
		m_boundMin.x = m_x * m_sizeX;
		m_boundMin.y = m_y * m_sizeY;
		m_boundMax.x = std::min((m_x + 1) * m_sizeX, m_renderer->GetRendererSetting().m_resX);
		m_boundMax.y = std::min((m_y + 1) * m_sizeY, m_renderer->GetRendererSetting().m_resY);
	}

	void TileRenderer::Render()
	{
		for (int pixelY = m_boundMin.y; pixelY < m_boundMax.y; ++pixelY)
		{
			for (int pixelX = m_boundMin.x; pixelX < m_boundMax.x; ++pixelX)
			{
				if (!m_renderer->IsInBound(pixelX, pixelY))
				{
					continue;
				}
				m_renderer->m_integrator->Render(pixelX, pixelY, this);
			}
		}
	}

	void TileRenderer::Finish()
	{
	}

	const RendererSetting& Renderer::GetRendererSetting()const
	{
		return m_setting;
	}

	Renderer::Renderer()
		: m_integrator(new PathTracing(this))
		, m_scene(new Scene())
	{
		m_setting.m_resX = 256;
		m_setting.m_resY = 256;
		m_setting.m_tileSizeX = 16;
		m_setting.m_tileSizeY = 16;
		m_setting.m_useJob = true;
		m_setting.m_maxBounce = 5;
		m_setting.m_numSamples = 10;
		m_scene->SetupSceneTest();
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
				TileRenderer* tileRenderer = (new TileRenderer(this, x, y, m_setting.m_tileSizeX, m_setting.m_tileSizeY));
				m_tileRenderers.push_back(tileRenderer);
			}
		}
		m_data = new PixelData[m_setting.m_resX * m_setting.m_resY];

		//PerspectiveCamera* m_camera = new PerspectiveCamera();
		//const Float dist = 10;
		//Vector3f pos = Vector3f(0, 1, 1) * dist;
		//m_camera->LookAt(pos, C_Zero_v3f, C_AxisZ_v3f);
		//m_camera->PerspectiveFovLH(39.0f, 1.0f, 1e-2f, 1000.f);
		//auto oldView = m_camera->GetViewMatrix();
		//auto oldProj = m_camera->GetProjMatrix();

		EntityId cameraId = m_scene->GetCamera();
		auto cameraComponent = EntityManager::IntancePtr()->GetComponent<CameraComponent>(cameraId);
		auto cameraTransform = EntityManager::IntancePtr()->GetComponent<Transform>(cameraId);
		auto viewMatrix = TransformUtils::GetMatrix(cameraTransform);
		auto projMatrix = cameraComponent->m_proj;

		m_renderContext.m_camera.m_screenBound = Rect2f(-1.0f, -1.0f, 1.0f, 1.0f);
		m_renderContext.m_camera.m_cameraToWorld = viewMatrix.Inverse();
		Matrix4f screenToRaster = MathUtils::ScreenToRasterMatrix(m_renderContext.m_camera.m_screenBound, Vector2i(m_setting.m_resX, m_setting.m_resY));
		Matrix4f cameraToRaster = projMatrix * screenToRaster;
		Matrix4f rasterToScreen = screenToRaster.Inverse();
		m_renderContext.m_camera.m_rasterToCamera = projMatrix.Inverse() * rasterToScreen;
		Vector3f vec = Vector3f(0, 0, 0);
		vec = m_renderContext.m_camera.m_rasterToCamera * (vec);
	}

	void Renderer::GenerateCameraRay(const CameraSample& cameraSample, Ray* ray)
	{
		Vector3f rayDir = m_renderContext.m_camera.m_rasterToCamera * (Vector3f(cameraSample.m_pixel[0], cameraSample.m_pixel[1], 0.0f));
		rayDir = MathUtils::TransformDirection(m_renderContext.m_camera.m_cameraToWorld, rayDir).Normalized();
		Vector3f rayPos = C_Zero_v3f;
		rayPos = m_renderContext.m_camera.m_cameraToWorld * (rayPos);
		ray->m_origin = rayPos;
		ray->m_direction = rayDir;
	}

	static void RenderJob(TileRenderer* tileRenderer)
	{
		tileRenderer->Render();
	}

	static void JobMain(fjs::Manager* mgr)
	{
		fjs::List list(mgr);
		for (int i = 0; i < Renderer::Instance().m_tileRenderers.size();++i)
		{
			list.Add(fjs::JobPriority::Normal, RenderJob, Renderer::Instance().m_tileRenderers[i]);
		}
		list.Wait();

		ImageOutput::WriteBMP("c:/test.bmp", Renderer::Instance().m_data, Renderer::Instance().m_setting.m_resX, Renderer::Instance().m_setting.m_resY);
	}

	void Renderer::Render()
	{
		if (m_setting.m_useJob)
		{
			fjs::ManagerOptions managerOptions;
			managerOptions.NumFibers = managerOptions.NumThreads * 20;
			managerOptions.ThreadAffinity = true;

			managerOptions.HighPriorityQueueSize = 256;
			managerOptions.NormalPriorityQueueSize = 512;
			managerOptions.LowPriorityQueueSize = 512;

			managerOptions.ShutdownAfterMainCallback = true;

			// Manager
			fjs::Manager manager(managerOptions);

			manager.Run(JobMain);
		}
		else
		{
			for (int i = 0; i < m_tileRenderers.size();++i)
			{
				m_tileRenderers[i]->Render();
			}
			ImageOutput::WriteBMP("c:/test.bmp", m_data, m_setting.m_resX, m_setting.m_resY);
		}
	}

	void Renderer::Finish()
	{
		for (int i = 0; i < m_tileRenderers.size();++i)
		{
			m_tileRenderers[i]->Finish();
			delete m_tileRenderers[i];
		}
		m_tileRenderers.clear();
		delete[] m_data;
	}

	void Renderer::AddSample(const Vector2f& uv, const Vector2i& boundMin, const Vector2i& boundMax, const Radiance& L)
	{
		const Float filterRadius = 0.5;
		Vector2f pFilmDiscrete = uv - Vector2f(0.5f, 0.5f);
		Vector2i p0 = Ceil<int, 2>(pFilmDiscrete - filterRadius);
		Vector2i p1 = Floor<int, 2>(pFilmDiscrete + filterRadius) + Vector2i(1, 1);
		p0 = Max(p0, boundMin);
		p1 = Min(p1, boundMax);

		for (int y = p0.y; y < p1.y; ++y) 
		{
			for (int x = p0.x; x < p1.x; ++x) 
			{
				PixelData& pixelData = GetPixelDataAt(x, y);
				pixelData.m_colorSum += L.m_directDiffuse;
				pixelData.m_weight += 1.0;
			}
		}
	}

	PixelData& Renderer::GetPixelDataAt(int pixelX, int pixelY)
	{
		int index = pixelY * m_setting.m_resX + pixelX;
		return m_data[index];
	}
}