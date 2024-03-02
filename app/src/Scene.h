#ifndef LIGHTNING_ENGINE_SCENE_H
#define LIGHTNING_ENGINE_SCENE_H

#include "Ray.h"
#include <glm/glm.hpp>
#include "imgui.h"
#include "Magma/Core/Logging.h"

#include <vector>

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;

	glm::vec3 EmissionColor{ 0.0f };
	float EmissionPower = 0.0f;

	glm::vec3 GetEmission() const { return EmissionColor * EmissionPower; }
};

struct Object
{
	glm::vec3 Position{ 0.0f };
	int MaterialIndex = 0;

	virtual bool hit(const Ray& ray,const glm::vec3& origin, float& hitDistance)
	{
		MAGMA_WARN("default Object.hit called.");
		return false;
	}
	virtual void EditUI() { }
};

struct Sphere : public Object
{
	float Radius = 0.5f;

	bool hit(const Ray& ray,const glm::vec3& origin, float& hitDistance) override
	{
		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2.0f * glm::dot(origin, ray.Direction);
		float c = glm::dot(origin, origin) - Radius * Radius;


		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			return false;

		float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
		if (closestT > 0.0f && closestT < hitDistance)
		{
			hitDistance = closestT;
			return true;
		}

		return false; 
	}

	void EditUI() override
	{
		ImGui::DragFloat("Radius", &Radius);
	}
};

struct Scene
{
	std::vector<Object*> Objects;
	std::vector<Material> Materials;
};
#endif