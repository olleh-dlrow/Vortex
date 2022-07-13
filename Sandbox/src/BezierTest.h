#pragma once

#include "EditorLayer.h"
#include <igl/bezier.h>
#include <Vortex/Scene/PointRendererComponent.h>
#include <Vortex/Scene/LineRendererComponent.h>

using std::vector;

class BezierTest : public EditorLayer
{
    Vortex::PointRendererComponent* pr;
    Vortex::LineRendererComponent* lr;

    glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);
    const glm::vec4 pointColor1 = glm::vec4(0.2, 0.8, 0.6, 1);
    const glm::vec4 pointColor2 = glm::vec4(0.2, 0.3, 0.9, 1);
    ImVec2 worldPos;
    vector<glm::vec3> positions;
    vector<glm::vec4> colors;

    vector<glm::vec3> linePoints;

    // add point state
    bool clicked = false;

    // drag point state
    bool isDragging = false;
    bool mouseInPoint = false;
    int curChosenIndex = 0;

    // mouse in window state
    bool mouseInWindow = false;
    bool drawLines = false;

public:
    BezierTest()
    {
        Vortex::Scene* scene = GetEditorScene();
        auto e1 = scene->AddEntity("e1");
        pr = e1->AddComponent<Vortex::PointRendererComponent>();
        lr = e1->AddComponent<Vortex::LineRendererComponent>();
    }

    inline void OnUpdate(Vortex::Timestep ts) override
    {
        Vortex::Camera& cam = GetCamera();
        // cast ray
        ImVec2 scrPos = ImGui::GetMousePos();
        mouseInWindow = m_ViewportWindow->IsInside(scrPos);

        ImVec2 normPos = m_ViewportWindow->ConvertToNormalizedPos(scrPos);
        Vortex::Ray r;
        cam.CastRay(glm::vec2(normPos.x, normPos.y), r);
        worldPos = ImVec2(r.origin.x, r.origin.y);

        if (!mouseInPoint)
        {
            if (mouseInWindow && !clicked && Vortex::Input::IsMouseButtonPressed(VT_MOUSE_BUTTON_1))
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
            vMin = glm::translate(I, pos) * glm::scale(I, { size, size, 1.0f }) * glm::vec4(vMin, 1);
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

        ////////////////////////////////////////////
        // START FROM HERE
        ////////////////////////////////////////////

        // paint control points
        pr->DrawPoints(positions, 1.0f, colors);

        if (drawLines && positions.size() > 2)
        {
            Eigen::MatrixXf C = Eigen::MatrixXf(positions.size(), 2);
            for (int i = 0; i < positions.size(); i++)
            {
                C(i, 0) = positions[i].x;
                C(i, 1) = positions[i].y;
            }

            Eigen::VectorXf T = Eigen::VectorXf(1000, 1);
            T(0, 0) = 0.0f;
            float eps = 1.0f / 1000.0f;
            for (int i = 1; i < 1000; i++)
            {
                T(i, 0) = glm::clamp(T(i - 1, 0) + eps, 0.0f, 1.0f);
            }

            Eigen::MatrixXf P;
            igl::bezier(C, T, P);

            int row = P.rows();
            if(row != linePoints.size())
                linePoints.resize(row);
            for (int i = 0; i < P.rows(); i++)
            {
                linePoints[i] = glm::vec3(P(i, 0), P(i, 1), 0.0f);
            }
            // glLineWidth(2.0f):
            // OPENGL error: API_ID_LINE_WIDTH deprecated behavior warning has been generated.
            // it seems that the opengl of higher version has deprecated this method, who 
            // encourages us to achieve our own line :(
            lr->m_Width = 1.0f;
            lr->DrawLines(linePoints);
        }
    }

    inline virtual void OnImGuiRender() override
    {
        ImGui::Begin("Debug");

        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 winPos = m_ViewportWindow->ConvertToWinPos(scrPos);
        ImGui::DragFloat2("InnerWinPos", (float*)&winPos);
        ImGui::DragFloat2("WorldPos", (float*)&worldPos);
        ImGui::Text("MouseInWindow: %s", mouseInWindow ? "yes" : "no");
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.2f, 1.0f), "Press E to delete Point");
        if (ImGui::Button("Draw or Not"))
        {
            drawLines = !drawLines;
        }
        if (ImGui::Button("Clear"))
        {
            positions.swap(vector<glm::vec3>());
        }
        ImGui::End();
    }
};
