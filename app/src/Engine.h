#ifndef LIGHTNING_ENGINE_H
#define LIGHTNING_ENGINE_H
#include "Magma/Magma.h"
#include "Renderer.h"

class LightningEngine :public mg::Application
{
public:
	LightningEngine();
	void Init();
	void Destroy();
	void Render();
public:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
};
#endif