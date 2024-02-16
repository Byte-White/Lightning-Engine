#include "Magma/Magma.h"
#include "Renderer.h"
#ifndef LIGHTNING_ENGINE_H
#define LIGHTNING_ENGINE_H

class LightningEngine :public mg::Application
{
public:
	void Init();
	void Destroy();
	void Render();
public:
	Renderer m_Renderer;
};
#endif