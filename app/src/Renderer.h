#include "Magma/Magma.h"
#include "glm/glm.hpp"
#ifndef LIGHTNING_RENDERER_H
#define LIGHTNING_RENDERER_H

class Renderer
{
public:
	void OnResize(uint32_t width, uint32_t height);
	void Render();
	std::shared_ptr<mg::Image> GetFinalImage() const { return  m_FinalImage; }
	void Destroy();
private:
	glm::vec4 PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<mg::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};

#endif