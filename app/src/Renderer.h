#ifndef LIGHTNING_RENDERER_H
#define LIGHTNING_RENDERER_H
#include "Magma/Magma.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include "Ray.h"

class Renderer
{
public:
	void OnResize(uint32_t width, uint32_t height);
	void Render(const Camera& camera);
	std::shared_ptr<mg::Image> GetFinalImage() const { return  m_FinalImage; }
	void Destroy();
private:
	glm::vec4 TraceRay(const Ray& ray);
private:
	std::shared_ptr<mg::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
};

#endif