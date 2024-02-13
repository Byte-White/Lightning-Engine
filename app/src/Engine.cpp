#include "Engine.h"

void LightningEngine::Init()
{

}



void LightningEngine::Render()
{
    ImGui::Begin("Viewport");
    //ImGui::Text("%d,%d", (int)ImGui::GetContentRegionAvail().x, (int)ImGui::GetContentRegionAvail().y);
    m_Renderer.OnResize(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
    m_Renderer.Render();
    ImGui::Image(m_Renderer.GetFinalImage()->GetImage(), { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y});
    ImGui::End();
    ImGui::Begin("Settings");
    ImGui::End();
}