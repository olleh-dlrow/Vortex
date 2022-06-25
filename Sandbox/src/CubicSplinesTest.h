#pragma once

#include "EditorLayer.h"
#include "Eigen/Dense"
#include <Vortex/Scene/PointRendererComponent.h>
#include <Vortex/Scene/LineRendererComponent.h>

using std::vector;

class CubicSplinesTest : public EditorLayer
{
    Vortex::PointRendererComponent* pr;
    Vortex::LineRendererComponent* lr;

    glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);
    const glm::vec4 pointColor1 = glm::vec4(0.2, 0.8, 0.6, 1);
    const glm::vec4 pointColor2 = glm::vec4(0.2, 0.3, 0.9, 1);
    ImVec2 worldPos;
    vector<glm::vec3> positions;
    vector<glm::vec4> colors;

    const int MAX_CNT_IN_A_SEG = 1000;
    vector<glm::vec3> linePoints;

    // add point state
    bool clicked = false;

    // drag point state
    bool isDragging = false;
    bool mouseInPoint = false;
    int curChosenIndex = 0;

    // mouse in window state
    bool mouseInWindow = false;
    
    // draw lines state
    bool drawNaturalSplines = false;


public:
    // the second derivative in two ends is 0.
    // 
    // you can use chasing method to solve the linear equations.
    // if one point moved, all the other points in line will move, the further the distance,
    // the less the movement of other points.
    void NatualSplines(const vector<glm::vec3>& input, vector<glm::vec3>& output)
    {
        int n = input.size();
        VT_ASSERT(output.size() == (n - 1) * MAX_CNT_IN_A_SEG, "output length is wrong!");
        // AM = B
        Eigen::MatrixXf A = Eigen::MatrixXf::Zero(n, n);
        Eigen::VectorXf B = Eigen::VectorXf::Zero(n);
        // construct Matrix A
        A(0, 0) = 1; A(n - 1, n - 1) = 1;
        // hi = xi+1 - xi
        for (int i = 0; i < n - 2; i++)
        {
            float hi = input[i + 1].x - input[i].x;
            float hi1 = input[i + 2].x - input[i + 1].x;
            A(i + 1, i) = hi;
            A(i + 1, i + 1) = 2 * (hi + hi1);
            A(i + 1, i + 2) = hi1;
        }

        // construct Vector B
        B(0) = 0; B(n - 1) = 0;
        for (int i = 0; i < n - 2; i++)
        {
            float hi = input[i + 1].x - input[i].x;
            float hi1 = input[i + 2].x - input[i + 1].x;
            B(i + 1) = 6 * ((input[i + 2].y - input[i + 1].y) / hi1 - (input[i + 1].y - input[i].y) / hi);
        }
        Eigen::VectorXf M;
        M = A.inverse()* B;
        
        // for n-1 lines
        int outputIdx = 0;
        for (int i = 0; i < n - 1; i++)
        {
            float yi = input[i].y, yi1 = input[i + 1].y, xi = input[i].x, xi1 = input[i + 1].x;
            float mi = M(i), mi1 = M(i + 1);
            float ai = yi;
            float hi = xi1 - xi;
            float bi = (yi1 - yi) / hi - mi * hi / 2 - (mi1 - mi) * hi / 6;
            float ci = mi / 2;
            float di = (mi1 - mi) / (6 * hi);

            // generate x
            float eps = (xi1 - xi) / MAX_CNT_IN_A_SEG;
            float x = xi;
            for (int xIdx = 0; xIdx < MAX_CNT_IN_A_SEG; xIdx++, x = std::min(x + eps, xi1))
            {
                float diffX = x - xi;
                float y = ai + bi * diffX + ci * diffX * diffX + di * diffX * diffX * diffX;
                output[outputIdx++] = glm::vec3(x, y, 0);
            }
        }
    }

    CubicSplinesTest()
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
            if (mouseInWindow && 
                m_ViewportWindow->IsFocused() && 
                !clicked && 
                Vortex::Input::IsMouseButtonPressed(VT_MOUSE_BUTTON_1))
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

        // sort input positions
        vector<glm::vec3> sortedPositions(positions);
        std::sort(sortedPositions.begin(), sortedPositions.end(),
            [](const glm::vec3& v1, const glm::vec3& v2) {
                return v1.x < v2.x;
            });

        if (positions.size() > 1)
        {
            int curLinePointsSize = (positions.size() - 1) * MAX_CNT_IN_A_SEG;
            if (curLinePointsSize != linePoints.size())
                linePoints.resize(curLinePointsSize);

            if (drawNaturalSplines)
            {
                NatualSplines(sortedPositions, linePoints);
                lr->DrawLines(linePoints);
            }

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
        ImGui::Checkbox("Natural Spline", &drawNaturalSplines);
        if (ImGui::Button("Clear"))
        {
            positions.swap(vector<glm::vec3>());
        }
        ImGui::End();
    }
};
