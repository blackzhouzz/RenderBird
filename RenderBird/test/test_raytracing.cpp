#include "stdafx.h"
#include "test_raytracing.h"


void TestRayTracingApp::Init()
{
	m_settings.m_resX = 256;
	m_settings.m_resY = 256;
	m_settings.m_tileSizeX = 64;
	m_settings.m_tileSizeY = 64;
	m_settings.m_useJob = true;

	m_renderer = new Renderer();
	m_renderer->m_setting = m_settings;
	m_renderer->Prepare();
}

void TestRayTracingApp::Run()
{
	m_renderer->Render();
	m_renderer->Finish();
}