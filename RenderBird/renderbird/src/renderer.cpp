#include "renderer.h"
#include "camera_component.h"
#include "trianglemesh.h"
#include "rayhitinfo.h"
#include "material.h"
#include <fjs/Manager.h>
#include <fjs/Counter.h>
#include <fjs/List.h>
#include <fjs/Queue.h>

namespace RenderBird
{
	void TileRenderer::Render()
	{
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
				m_renderer->m_integrator->Render(pixelX, pixelY);
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
		m_data = new Core::RGB32[m_setting.m_resX * m_setting.m_resY];

		//PerspectiveCamera* m_camera = new PerspectiveCamera();
		//const Float dist = 10;
		//Vector3f pos = Vector3f(0, 1, 1) * dist;
		//m_camera->LookAt(pos, Vector3f::ZERO, Vector3f::UNIT_Z);
		//m_camera->PerspectiveFovLH(39.0f, 1.0f, 1e-2f, 1000.f);
		//auto oldView = m_camera->GetViewMatrix();
		//auto oldProj = m_camera->GetProjMatrix();

		EntityId cameraId = m_scene->GetCamera();
		auto cameraComponent = EntityManager::IntancePtr()->GetComponent<CameraComponent>(cameraId);
		auto cameraTransform = EntityManager::IntancePtr()->GetComponent<Transform>(cameraId);
		auto viewMatrix = cameraTransform->m_cached;
		auto projMatrix = cameraComponent->m_proj;

		m_renderContext.m_camera.m_screenBound = Rect2f(-1.0f, -1.0f, 1.0f, 1.0f);
		m_renderContext.m_camera.m_cameraToWorld = viewMatrix.InverseSafe();
		Matrix4f screenToRaster = MathUtils::ScreenToRasterMatrix(m_renderContext.m_camera.m_screenBound, Point2i(m_setting.m_resX, m_setting.m_resY));
		Matrix4f cameraToRaster = projMatrix * screenToRaster;
		Vector3f vec = Vector3f(0, 0, 0);
		m_renderContext.m_camera.m_rasterToCamera = cameraToRaster.InverseSafe();
		vec = m_renderContext.m_camera.m_rasterToCamera.TransformDirection(vec);

		m_renderContext.m_camera.m_rasterToWorld = (viewMatrix * projMatrix * screenToRaster).InverseSafe();
	}

	void Renderer::GenerateCameraRay(int pixelX, int pixelY, Ray* ray)
	{
		Vector3f rayDir = m_renderContext.m_camera.m_rasterToCamera.TransformPerspective(Vector3f(pixelX, pixelY, 0.0f));
		rayDir = m_renderContext.m_camera.m_cameraToWorld.TransformDirection(rayDir);
		rayDir.Normalize();

		Vector3f rayPos = Vector3f::ZERO;
		rayPos = m_renderContext.m_camera.m_cameraToWorld.TransformPoint(rayPos);
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