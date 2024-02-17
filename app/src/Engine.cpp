#include "Engine.h"

LightningEngine::LightningEngine()
    :m_Camera(45.0f, 0.1f, 100.0f)
{

}

void LightningEngine::Init()
{

}

void LightningEngine::Destroy()
{
    m_Renderer.Destroy();
}

void LightningEngine::Render()
{
    m_Camera.OnUpdate(ImGui::GetIO().DeltaTime);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    //ImGui::Text("%d,%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);
    
    uint32_t viewportWidth = ImGui::GetContentRegionAvail().x; 
    uint32_t viewportHeight = ImGui::GetContentRegionAvail().y;
    m_Renderer.OnResize(viewportWidth,viewportHeight);
    m_Camera.OnResize(viewportWidth, viewportHeight);
    m_Renderer.Render(m_Camera);

    ImGui::Image(m_Renderer.GetFinalImage()->GetImage(), { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y});
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::Begin("Settings");
    ImGui::SeparatorText("Camera");
    ImGui::TextDisabled("Hold Right Click and use WASD to move.");
    ImGui::Text("FPS: %f (%f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    if (ImGui::Button("Reset Camera Position")) m_Camera.ResetPosition();
    ImGui::DragFloat3("position", (float*)(&m_Camera.GetPosition().x), 0.05);
    if(ImGui::DragFloat3("direction", (float*)(&m_Camera.GetDirection().x), 0.0125))m_Camera.RecalculateViewAndDirections();
    ImGui::DragFloat("Shift Speed", &m_Camera.GetShiftSpeed(),0.25f);
    ImGui::DragFloat("Normal Speed", &m_Camera.GetNormalSpeed(), 0.25f);
    ImGui::DragFloat("Control Speed", &m_Camera.GetControlSpeed(), 0.25f);
    ImGui::SeparatorText("Scene");
    ImGui::End();
}