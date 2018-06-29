#pragma once
#include "core/coreprivate.h"
#include "scene.h"

namespace RenderBird
{
	class SceneManager
	{
	public:
		SceneManager();
		Scene* LoadScene(std::string path);
		void UnloadScene(Scene* scene);
		Scene* GetActiveScene() { return m_activeScene; }
	private:
		Scene* m_activeScene;
	};
}