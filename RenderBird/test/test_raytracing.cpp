#include "stdafx.h"
#include "test_raytracing.h"

void TestRayTracingApp::Init()
{
	m_renderer = new Renderer();
	m_renderer->Prepare();
}

void TestRayTracingApp::Run()
{
	m_renderer->Render();
}