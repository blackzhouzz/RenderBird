#include "renderer.h"
#include "CameraComponent.h"
#include "TriangleMesh.h"
#include "Material.h"
#include <fjs/Manager.h>
#include <fjs/Counter.h>
#include <fjs/List.h>
#include <fjs/Queue.h>
#include "RenderStatistic.h"
#include <iostream>

namespace RenderBird
{
	TileRenderer::TileRenderer(Renderer* renderer, int x, int y, int sizeX, int sizeY)
		: m_renderer(renderer), m_x(x), m_y(y), m_sizeX(sizeX), m_sizeY(sizeY)
	{
		auto resX = m_renderer->GetRendererSetting().m_resX;
		auto resY = m_renderer->GetRendererSetting().m_resY;
		m_boundMin.x = m_x * sizeX;
		m_boundMin.y = m_y * sizeY;
		m_boundMax.x = std::min((m_x + 1) * sizeX, resX);
		m_boundMax.y = std::min((m_y + 1) * sizeY, resY);
		m_sampler.reset(m_renderer->m_sampler->Clone());
		m_sampler->Init(resX, resY);
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
		m_setting.m_maxBounce = 8;
		m_setting.m_rrBounce = 4;
		m_setting.m_numSamples = 16;
		m_setting.m_denoising = true;
		m_setting.m_enableClamp = false;
		m_setting.m_clampValue = 1.0;
		m_sampler.reset(new HaltonSampler(m_setting.m_numSamples));
		m_scene->SetupSceneTest();
	}

	bool Renderer::IsInBound(int x, int y)const
	{
		return x >= 0 && x < m_setting.m_resX && y >= 0 && y < m_setting.m_resY;
	}

	void Renderer::InitBuffers()
	{
		Vector2u res; 
		res.x = m_setting.m_resX;
		res.y = m_setting.m_resY;

		m_buffers._colorBuffer.reset(new OutputBufferVec3f(res, OutputBufferTypeEnum::OutputColor, true, true));
		m_buffers._depthBuffer.reset(new OutputBufferF(res, OutputBufferTypeEnum::OutputDepth, true, true));
		m_buffers._normalBuffer.reset(new OutputBufferVec3f(res, OutputBufferTypeEnum::OutputNormal, true, true));
		m_buffers._albedoBuffer.reset(new OutputBufferVec3f(res, OutputBufferTypeEnum::OutputAlbedo, true, true));
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

		InitBuffers();

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

	}

	void Renderer::GenerateCameraRay(Float pixelX, Float pixelY, Ray* ray)
	{
		Vector3f rayDir = m_renderContext.m_camera.m_rasterToCamera * (Vector3f(pixelX, pixelY, 0.0f));
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
		Renderer::Instance().Finish();
	}

	void Renderer::Render()
	{
		m_scene->BeginRender();

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
			Finish();
		}
	}

	std::unique_ptr<PixmapF> slicePixmap3f(Vector3f* src, int channel, Vector2u res)
	{
		int w = res.x, h = res.y;

		auto result = std::unique_ptr<PixmapF>(new PixmapF(w, h));
		for (int j = 0; j < w*h; ++j)
			(*result)[j] = src[j][channel];

		return std::move(result);
	}

	std::unique_ptr<PixmapF> slicePixmapf(Float* src, Vector2u res)
	{
		int w = res.x, h = res.y;

		auto result = std::unique_ptr<PixmapF>(new PixmapF(w, h));
		for (int j = 0; j < w*h; ++j)
			(*result)[j] = src[j];

		return std::move(result);
	}

	void Renderer::ExtractFeatures3f(OutputBufferVec3f* buf, std::vector<RenderBufferF>& features)
	{
		std::unique_ptr<Vector3f[]> buffer = buf->GetBuffer();
		std::unique_ptr<Vector3f[]> variance = buf->GetVariance();
		for (int i = 0; i < 3; ++i)
		{
			features.emplace_back();
			features.back().buffer = slicePixmap3f(buffer.get(), i, buf->_res);
			features.back().bufferA = slicePixmap3f(buf->_bufferA.get(), i, buf->_res);
			features.back().bufferB = slicePixmap3f(buf->_bufferB.get(), i, buf->_res);
			features.back().bufferVariance = slicePixmap3f(buf->_variance.get(), i, buf->_res);
		}
	}

	void Renderer::ExtractFeatures1f(OutputBufferF* buf, std::vector<RenderBufferF>& features)
	{
		std::unique_ptr<Float[]> buffer = buf->GetBuffer();
		std::unique_ptr<Float[]> variance = buf->GetVariance();
		features.emplace_back();
		features.back().buffer = slicePixmapf(buffer.get(), buf->_res);
		features.back().bufferA = slicePixmapf(buf->_bufferA.get(), buf->_res);
		features.back().bufferB = slicePixmapf(buf->_bufferB.get(), buf->_res);
		features.back().bufferVariance = slicePixmapf(buf->_variance.get(), buf->_res);
	}

	void Renderer::Denoising()
	{
		auto res = m_buffers._colorBuffer->_res;
		std::vector<RenderBufferF> features;
		if (m_buffers._normalBuffer != nullptr)
		{
			ExtractFeatures3f(m_buffers._normalBuffer.get(), features);

			std::unique_ptr<Pixmap3f> tmp(new Pixmap3f(res.x, res.y, m_buffers._normalBuffer->GetBuffer().get()));
			ImageIO::WriteBMP("c:/normal_a.bmp", *tmp);
		}
		if (m_buffers._colorBuffer != nullptr)
		{
			//ExtractFeatures3f(m_buffers._colorBuffer.get(), features);
			std::unique_ptr<Pixmap3f> tmp(new Pixmap3f(res.x, res.y, m_buffers._colorBuffer->GetBuffer().get()));
			ImageIO::WriteBMP("c:/test.bmp", *tmp);
		}
		if (m_buffers._albedoBuffer != nullptr)
		{
			ExtractFeatures3f(m_buffers._albedoBuffer.get(), features);
			std::unique_ptr<Pixmap3f> tmp(new Pixmap3f(res.x, res.y, m_buffers._albedoBuffer->GetBuffer().get()));
			ImageIO::WriteBMP("c:/albedo_a.bmp", *tmp);
		}
		if (m_buffers._depthBuffer != nullptr)
		{
			ExtractFeatures1f(m_buffers._depthBuffer.get(), features);
		}
		RenderBuffer3f image;
		image.buffer = std::unique_ptr<Pixmap3f>(new Pixmap3f(res.x, res.y, m_buffers._colorBuffer->GetBuffer().get()));
		image.bufferA = std::unique_ptr<Pixmap3f>(new Pixmap3f(res.x, res.y, m_buffers._colorBuffer->_bufferA.get()));
		image.bufferB = std::unique_ptr<Pixmap3f>(new Pixmap3f(res.x, res.y, m_buffers._colorBuffer->_bufferB.get()));
		image.bufferVariance = std::unique_ptr<Pixmap3f>(new Pixmap3f(res.x, res.y, m_buffers._colorBuffer->GetVariance().get()));

		Pixmap3f result = nforDenoiser(std::move(image), std::move(features));
		ImageIO::WriteBMP("c:/test_a.bmp", result);
	}

	void Renderer::Finish()
	{
		m_scene->EndRnder();

		if (m_setting.m_denoising)
		{
			Denoising();
		}

		RenderStatistic::Print();

		for (int i = 0; i < m_tileRenderers.size();++i)
		{
			m_tileRenderers[i]->Finish();
			delete m_tileRenderers[i];
		}
		m_tileRenderers.clear();
	}

}