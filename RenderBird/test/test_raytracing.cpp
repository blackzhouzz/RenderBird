#include "stdafx.h"
#include "test_raytracing.h"


void TestRayTracingApp::Init()
{
	m_settings.m_resX = 512;
	m_settings.m_resY = 512;
	m_settings.m_tileSizeX = 512;
	m_settings.m_tileSizeY = 512;

	m_renderer = new Renderer(m_settings);
	m_renderer->Prepare();
}


void TestRayTracingApp::Run()
{
	m_renderer->Render();
	m_renderer->Finish();
}