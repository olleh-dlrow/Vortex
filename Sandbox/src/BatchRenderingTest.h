#pragma once

#include <imgui.h>
#include <Vortex.h>

using Vortex::Ref;
using Vortex::CreateRef;
using Vortex::ViewportWindow;
using Vortex::VertexArray;
using Vortex::VertexBuffer;
using Vortex::BufferLayout;
using Vortex::Camera;
using Vortex::OrthoParam;
using Vortex::Renderer;
using Vortex::DrawPointConfig;
using Vortex::DrawLineConfig;
using Vortex::IndexBuffer;
using Vortex::Ray;
using Vortex::Shader;

using Vortex::Batch;
using Vortex::Quad1;

using std::vector;

static glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);
const static glm::vec4 pointColor1 = glm::vec4(0.2, 0.8, 0.6, 1);
const static glm::vec4 pointColor2 = glm::vec4(0.2, 0.3, 0.9, 1);
static ImVec2 worldPos;
static vector<glm::vec3> positions;
static vector<glm::vec4> colors;
static bool clicked = false;
static bool mouseInPoint = false;
static bool isDragging = false;
static int curChosenIndex = 0;

class BatchRenderingTest : public Vortex::Layer
{
public:
    BatchRenderingTest()
    {
        srand(time(NULL));

        Ref<Camera> cam = CreateRef<Camera>(OrthoParam(), glm::vec3(0, 0, 5.0f));
        m_ViewportWindow = CreateRef<ViewportWindow>("Viewport", cam);
    }

    virtual ~BatchRenderingTest() = default;

    inline virtual void OnAttach() override
    {

    }
    inline virtual void OnDetach() override
    {

    }


    inline void OnUpdate(Vortex::Timestep ts) override
    {
        m_ViewportWindow->OnUpdate(ts);

        // background
        Vortex::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        // clear glfw background and imgui 
        Vortex::Renderer::Clear();
        // start render in viewport window
        m_ViewportWindow->Begin();

        Vortex::Camera& cam = *m_ViewportWindow->GetCamera();

        // cast ray
        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 normPos = m_ViewportWindow->ConvertToNormalizedPos(scrPos);
        Ray r;
        cam.CastRay(glm::vec2(normPos.x, normPos.y), r);
        worldPos = ImVec2(r.origin.x, r.origin.y);
        if (!mouseInPoint)
        {
            if (!clicked && Vortex::Input::IsMouseButtonPressed(VT_MOUSE_BUTTON_1))
            {
                positions.emplace_back(glm::vec3(worldPos.x, worldPos.y, 0));
                colors.emplace_back(pointColor1);
                clicked = true;
            }
            if (clicked && Vortex::Input::IsMouseButtonReleased(VT_MOUSE_BUTTON_1))
            {
                clicked = false;
            }
        }
        else
        {
            if (!isDragging && Vortex::Input::IsKeyPressed(VT_KEY_E))
            {
                positions.erase(positions.begin() + curChosenIndex);
            }
            if (Vortex::Input::IsMouseButtonPressed(VT_MOUSE_BUTTON_1))
            {
                isDragging = true;
            }
        }
        if (isDragging)
        {
            positions[curChosenIndex] = glm::vec3(worldPos.x, worldPos.y, 0);
            if (Vortex::Input::IsMouseButtonReleased(VT_MOUSE_BUTTON_1))
            {
                isDragging = false;
            }
        }

        mouseInPoint = false;
        float size = 0.1f;
        glm::mat4 I = glm::identity<glm::mat4>();
        for (int i = 0; i < positions.size(); i++)
        {
            glm::vec3& pos = positions[i];

            glm::vec3 vMin = glm::vec3(-0.5, -0.5, 0);
            glm::vec3 vMax = glm::vec3(0.5, 0.5, 0);
            vMin = glm::translate(I, pos) * glm::scale(I, {size, size, 1.0f}) * glm::vec4(vMin, 1);
            vMax = glm::translate(I, pos) * glm::scale(I, { size, size, 1.0f }) * glm::vec4(vMax, 1);
            if (worldPos.x >= vMin.x && worldPos.x <= vMax.x && worldPos.y >= vMin.y &&
                worldPos.y <= vMax.y)
            {
                mouseInPoint = true;
                if (!isDragging)
                {
                    colors[i] = pointColor2;
                    curChosenIndex = i;
                }
            }
            else
            {
                colors[i] = pointColor1;
            }
        }

        Vortex::Renderer::BeginScene(cam);
        Renderer::DrawPoints(positions, 1.0f, colors);

        if (positions.size() > 1)
        {
            float width = 2.0f;
            Renderer::DrawLines(positions, width);
        }

        Vortex::Renderer::EndScene();
        m_ViewportWindow->End();
    }

    inline virtual void OnImGuiRender() override
    {
        m_ViewportWindow->OnImGuiRender();
        m_ViewportWindow->GetCamera()->RenderConfigGUI();
        ImGui::ShowDemoWindow();

        ImGui::Begin("Debug");

        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 winPos = m_ViewportWindow->ConvertToWinPos(scrPos);
        ImGui::DragFloat2("WinPos", (float*)&winPos);
        ImGui::DragFloat2("WorldPos", (float*)&worldPos);
        ImGui::ColorEdit3("Line Color", glm::value_ptr(color));

        ImGui::End();
    }
    inline void OnEvent(Vortex::Event& e) override
    {
        m_ViewportWindow->OnEvent(e);
    }
private:
    Ref<ViewportWindow> m_ViewportWindow;
};

