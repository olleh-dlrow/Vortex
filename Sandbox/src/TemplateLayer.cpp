#include "TemplateLayer.h"

#include <Vortex/Geo/DrawGeoConfig.h>

#include <imgui.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



TemplateLayer::TemplateLayer()
	:Layer("TemplateLayer")
{

}

void TemplateLayer::OnAttach()
{
}

void TemplateLayer::OnDetach()
{

}



void TemplateLayer::OnUpdate(Vortex::Timestep ts)
{
	//Vortex::Renderer::SetClearColor(glm::vec4(0.2f, 0.1f, 0.2f, 1.0f));
	Vortex::Renderer::Clear();
}

void TemplateLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::ShowDemoWindow();
	ImGui::End();
}

void TemplateLayer::OnEvent(Vortex::Event& e)
{
}
