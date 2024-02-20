#ifndef LIGHTNING_RENDERER_H
#define LIGHTNING_RENDERER_H
#include "Magma/Magma.h"
#include "glm/glm.hpp"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

class Renderer
{
public:
	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);
	std::shared_ptr<mg::Image> GetFinalImage() const { return  m_FinalImage; }
	int& GetBounces() { return m_Bounces; }
	void Destroy();
private:
	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};


	glm::vec4 RayGen(uint32_t x, uint32_t y);
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);

private:
	std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	std::shared_ptr<mg::Image> m_FinalImage;
	const Scene* m_ActiveScene = nullptr;
	const Camera* m_ActiveCamera = nullptr;

	uint32_t* m_ImageData = nullptr;

	int m_Bounces = 3;
};

#endif