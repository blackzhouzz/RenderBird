#include "stdafx.h"
#include "test_raytracing.h"


void TestRayTracingApp::Init()
{
	m_settings.m_resX = 700;
	m_settings.m_resY = 700;
	m_settings.m_tileSizeX = 700;
	m_settings.m_tileSizeY = 700;

	m_renderer = new Renderer(m_settings);
	m_renderer->Prepare();
}


void TestRayTracingApp::Run()
{
	m_renderer->Render();
	m_renderer->Finish();
}