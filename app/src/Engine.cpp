#include "Engine.h"

void LightningEngine::Init()
{

}

void LightningEngine::Destroy()
{
    m_Renderer.Destroy();
}

void LightningEngine::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    //ImGui::Text("%d,%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);
    m_Renderer.OnResize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    m_Renderer.Render();

    ImGui::Image(m_Renderer.GetFinalImage()->GetImage(), { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y});
    
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::Begin("Settings");
    ImGui::Text("FPS: %f (%f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
    ImGui::End();
}