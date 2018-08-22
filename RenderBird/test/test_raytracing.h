#pragma once
#include "renderer.h"
using namespace RenderBird;

class TestRayTracingApp
{
public:
	void Init();
	void Run();
private:
	Renderer* m_renderer;
};