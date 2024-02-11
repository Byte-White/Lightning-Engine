#define MAGMA_ENTRYPOINT
#include "Magma/Magma.h"

class MyApplication:public mg::Application
{
public:
	MyApplication()
	{

	}

	void Init() override
	{
		SetSize(800,600);
		SetTitle("Magma Application");
	}

	~MyApplication()
	{

	}
	void Render() override
	{
		ImGui::Begin("Magma App Template");
    		if(mg::renderer->GetAPI() == mg::RenderAPI::Vulkan) ImGui::Text("Render API: Vulkan");
		else ImGui::Text("Render API: OpenGL");
		ImGui::End();
	}
};

mg::Application* CreateApplication(int argc, char** argv)
{
	return new MyApplication();
}
