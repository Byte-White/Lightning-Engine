#include "Renderer.h"
#include <execution>


namespace Utils
{
static glm::vec3 RandomVec3(float min, float max)
{
	/*
	float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	// Scale and shift the random values to fit the specified range [min, max]
	float scaledX = randX * (max - min) + min;
	float scaledY = randY * (max - min) + min;
	float scaledZ = randZ * (max - min) + min;

	return glm::vec3(scaledX, scaledY, scaledZ);
	*/
	return glm::vec3(mg::Random::Float(min,max), mg::Random::Float(min, max), mg::Random::Float(min, max));
}

static glm::vec3 RandomInUnitSphere()
{
	return glm::normalize(RandomVec3(-1.0f, 1.0f));
}

static uint32_t ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = (uint8_t)(color.r * 255.0f);
	uint8_t g = (uint8_t)(color.g * 255.0f);
	uint8_t b = (uint8_t)(color.b * 255.0f);
	uint8_t a = (uint8_t)(color.a * 255.0f);

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
	return result;
}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<mg::Image>(width, height, mg::ImageFormat::RGBA,nullptr, mg::ImageFilter::Nearest);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];


	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec4[width * height];

	m_ImageHorizontalIter.resize(width);
	m_ImageVerticalIter.resize(height);
	for (uint32_t i = 0; i < width; i++)
		m_ImageHorizontalIter[i] = i;
	for (uint32_t i = 0; i < height; i++)
		m_ImageVerticalIter[i] = i;
}


void Renderer::Render(Scene& scene, Camera& camera)
{
	m_ActiveScene = &scene;
	m_ActiveCamera = &camera;
	
	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));

	std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
		[this](uint32_t y)
		{
			std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
			[this, y](uint32_t x)
				{
					glm::vec4 color = RayGen(x, y);
					m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;

					glm::vec4 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
					accumulatedColor /= (float)m_FrameIndex;

					accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
					m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(accumulatedColor);
				});
		});


	m_FinalImage->SetData(m_ImageData);
	m_FrameIndex++;
}

glm::vec4 Renderer::RayGen(uint32_t x,uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 color(0.0f);
	float multiplier = 1.0f;
	glm::vec3 contribution(1.0f);
	glm::vec3 light(0.0f);

	for (int i = 0; i < m_Bounces; i++)
	{
		Renderer::HitPayload payload = TraceRay(ray);
		if (payload.HitDistance < 0.0f)
		{
			light += m_skyColor * contribution;
			break;
		}



		const Object* object = m_ActiveScene->Objects[payload.ObjectIndex];
		const Material& material = m_ActiveScene->Materials[object->MaterialIndex];

		glm::vec3 sphereColor = material.Albedo;
		color += sphereColor * multiplier;

		multiplier *= 0.5f;

		contribution *= material.Albedo;
		light += material.GetEmission();
		if (material.Metallic > 0.0f) 
		{
			// Calculate reflection direction for metallic materials
			glm::vec3 reflectedDir = glm::reflect(ray.Direction, payload.WorldNormal);
			// Apply roughness
			reflectedDir = glm::normalize(reflectedDir + material.Roughness * Utils::RandomInUnitSphere());
			ray.Direction = reflectedDir;
		}
		else 
		{
			ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
			ray.Direction = glm::normalize(payload.WorldNormal + Utils::RandomInUnitSphere());
		}
	}

	return glm::vec4(light, 1.0f);
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
	// (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
	// where
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance

	int closestObject = -1;
	float hitDistance = std::numeric_limits<float>::max();
	for (size_t i = 0; i < m_ActiveScene->Objects.size(); i++)
	{
		Object* object = m_ActiveScene->Objects[i];
		glm::vec3 origin = ray.Origin - object->Position;
		if (object->hit(ray, origin, hitDistance))
		{
			//MAGMA_CORE_TRACE("hit at {0}", i);
			// object.hit changes hitDistance on hit
			closestObject = i;
		}
	}

	if (closestObject < 0)
		return Miss(ray);

	return ClosestHit(ray, hitDistance, closestObject);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Object* closestObject = m_ActiveScene->Objects[objectIndex];

	glm::vec3 origin = ray.Origin - closestObject->Position;
	payload.WorldPosition = origin + ray.Direction * hitDistance;
	payload.WorldNormal = glm::normalize(payload.WorldPosition);
	
	payload.WorldPosition += closestObject->Position;

	return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;
	return payload;
}

void Renderer::Destroy()
{
	m_FinalImage.reset();
	delete[] m_ImageData;

	for (auto obj : m_ActiveScene->Objects) delete obj;
}