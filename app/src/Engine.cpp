#include "Engine.h"
#include <thread>

LightningEngine::LightningEngine()
    :m_Camera(45.0f, 0.1f, 100.0f)
{
    MAGMA_INFO("CPU Cores: {0}",std::thread::hardware_concurrency());
    Material groundMaterial;
    groundMaterial.Albedo = { 1.0f, 1.0f, 1.0f };
    groundMaterial.Roughness = 0.5f;
    m_Scene.Materials.emplace_back(groundMaterial);
    
    Material lightMaterial;
    lightMaterial.Albedo = { 1.0f, 1.0f, 0.0f };
    lightMaterial.Roughness = 0.0f;
    lightMaterial.EmissionColor = { 1.0f, 1.0f, 0.0f };
    lightMaterial.EmissionPower = 2.5f;
    m_Scene.Materials.emplace_back(lightMaterial);

    Material greenSphere; 
    greenSphere.Albedo = { 0.0f, 1.0f, 0.0f };
    greenSphere.Roughness = 0.0f;
    m_Scene.Materials.emplace_back(greenSphere);

    Material redSphere;
    redSphere.Albedo = { 1.0f, 0.0f, 0.0f };
    redSphere.Roughness = 0.1f;
    m_Scene.Materials.emplace_back(redSphere);
    
    {
        Sphere groundSphere;
        groundSphere.Position = { 0.0f, 101.0f, 0.0f };
        groundSphere.Radius = 100.0f;
        groundSphere.MaterialIndex = 0;
        m_Scene.Spheres.push_back(groundSphere);
    }

    {
        Sphere lightSphere;
        lightSphere.Position = { 0.0f, -5.0f, 3.0f };
        lightSphere.Radius = 1.5f;
        lightSphere.MaterialIndex = 1;
        m_Scene.Spheres.push_back(lightSphere);
    }

    {
        Sphere sphere;
        sphere.Position = { 0.0f, 0.0f, 0.0f };
        sphere.Radius = 1.0f;
        sphere.MaterialIndex = 2;
        m_Scene.Spheres.push_back(sphere);
    }

    {
        Sphere sphere;
        sphere.Position = { 0.0f, 0.0f, -6.0f };
        sphere.Radius = 2.0f;
        sphere.MaterialIndex = 3;
        m_Scene.Spheres.push_back(sphere);
    }
}

void LightningEngine::Init()
{
    mg::DeepOceanTheme();
}

void LightningEngine::Destroy()
{
    m_Renderer.Destroy();
}

void LightningEngine::Render()
{
    if(m_Camera.OnUpdate(ImGui::GetIO().DeltaTime))m_Renderer.ResetFrameIndex();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    //ImGui::Text("%d,%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);
    
    uint32_t viewportWidth = ImGui::GetContentRegionAvail().x; 
    uint32_t viewportHeight = ImGui::GetContentRegionAvail().y;
    m_Renderer.OnResize(viewportWidth,viewportHeight);
    m_Camera.OnResize(viewportWidth, viewportHeight);
    m_Renderer.Render(m_Scene,m_Camera);

    ImGui::Image(m_Renderer.GetFinalImage()->GetImage(), { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y});
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::Begin("Settings");
    ImGui::SeparatorText("Rendering");
    ImGui::DragInt("Bounces", &(int)m_Renderer.GetBounces(), 0.f, 1, 10.f);
    ImGui::SeparatorText("Camera");
    ImGui::TextDisabled("Hold Right Click and use WASD to move.");
    ImGui::Text("FPS: %f (%f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    if (ImGui::Button("Reset Camera Position")) m_Camera.ResetPosition();
    ImGui::DragFloat3("position", (float*)(&m_Camera.GetPosition().x), 0.05);
    if(ImGui::DragFloat3("direction", (float*)(&m_Camera.GetDirection().x), 0.0125))m_Camera.RecalculateViewAndDirections();
    ImGui::DragFloat("Shift Speed", &m_Camera.GetShiftSpeed(),0.25f);
    ImGui::DragFloat("Normal Speed", &m_Camera.GetNormalSpeed(), 0.25f);
    ImGui::DragFloat("Control Speed", &m_Camera.GetControlSpeed(), 0.25f);

    ImGui::ColorEdit3("Sky Color", &m_Renderer.GetSkyColor().r);
    // ImGui::DragFloat3("Direction", &m_Renderer.GetSkyLightDirection().x); // might add it in the future
    ImGui::End();

    ImGui::Begin("Scene Editor");
    int sphereIndex = 0;
    for (auto& sphere : m_Scene.Spheres)
    {
        ImGui::PushID(sphereIndex);

        ImGui::Text("Sphere %d", sphereIndex);
        ImGui::SliderInt("Material Index", &sphere.MaterialIndex,0,m_Scene.Materials.size()-1);
        ImGui::DragFloat3("Position", &sphere.Position.x);
        ImGui::DragFloat("Radius", &sphere.Radius);
        
        ImGui::PopID();
        
        sphereIndex++;
    }
    ImGui::End();

    int materialIndex = 0;
    ImGui::Begin("Material Editor");
    for (auto& material : m_Scene.Materials)
    {
        ImGui::PushID(materialIndex);
        ImGui::Separator();
        ImGui::Text("Material Index: %d", materialIndex);
        ImGui::ColorPicker3("Albedo", &material.Albedo.r);
        ImGui::DragFloat("Metalic", &material.Metallic, 0.005, 0.f, 1.f);
        ImGui::DragFloat("Roughness", &material.Roughness, 0.005, 0.f, 1.f);
        ImGui::ColorPicker3("Emission Color", &material.EmissionColor.r);
        ImGui::DragFloat("Emission Power", &material.EmissionPower);
        ImGui::PopID();

        materialIndex++;
    }
    ImGui::End();
}