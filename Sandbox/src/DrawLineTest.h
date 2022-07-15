#pragma once

#include "EditorLayer.h"
#include <igl/bezier.h>
#include <Vortex/Scene/PointRendererComponent.h>
#include <Vortex/Scene/LineRendererComponent.h>

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

using std::vector;

static Vortex::PointRendererComponent* pr;
static Vortex::LineRendererComponent* lr;

static glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);
const static glm::vec4 pointColor1 = glm::vec4(0.2, 0.8, 0.6, 1);
const static glm::vec4 pointColor2 = glm::vec4(0.2, 0.3, 0.9, 1);
static ImVec2 worldPos;
static vector<glm::vec3> positions;
static vector<glm::vec4> colors;

// add point state
static bool clicked = false;

// drag point state
static bool isDragging = false; 
static bool mouseInPoint = false;
static int curChosenIndex = 0;

// mouse in window state
static bool mouseInWindow = false;

static ImVec2 vMin, vMax;

static bool drawLines = false;

std::vector<float> lagrange(const std::vector<glm::vec3>& input_points,
                            const std::vector<float>& xs)
{
    int length = input_points.size();
    std::vector<float> ys;
    if (length >= 2)
    {
        for (auto x : xs)
        {
            float y = 0.0f;
            for (int i = 0; i < length; i++)
            {
                float temp = input_points[i][1];
                for (int j = 0; j < length; j++)
                {
                    if (i != j)
                    {
                        temp *= x - input_points[j][0];
                        temp *= 1 / (input_points[i][0] - input_points[j][0]);
                    }
                }
                y += temp;
            }
            ys.push_back(y);
        }
    }
    return ys;
}



class DrawLineTest : public EditorLayer
{
public:
	DrawLineTest()
	{
        const Eigen::MatrixXd C =
            (Eigen::MatrixXd(4, 2) << 0, 0, 0.5, 0, 0.5, 1, 1, 1).finished();
        const Eigen::VectorXd T =
            (Eigen::VectorXd(11, 1) << 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1).finished();
        Eigen::MatrixXd P;
        igl::bezier(C, T, P);

        Vortex::Scene* scene = GetEditorScene();
        auto e1 = scene->AddEntity("e1");
        pr = e1->AddComponent<Vortex::PointRendererComponent>();
        lr = e1->AddComponent<Vortex::LineRendererComponent>();
	}

	inline void OnUpdate(Vortex::Timestep ts) override
	{
        EditorLayer::OnUpdate(ts);

		Vortex::Camera& cam = GetCamera();
        // cast ray
        ImVec2 scrPos = ImGui::GetMousePos();
        mouseInWindow = m_ViewportWindow->IsInside(scrPos);

        ImVec2 normPos = m_ViewportWindow->ConvertToNormalizedPos(scrPos);
        Ray r;
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

        pr->DrawPoints(positions, 1.0f, colors);

        if (drawLines && positions.size() > 1)
        {
            vector<glm::vec3> sortedPositions(positions.begin(), positions.end());
            std::sort(sortedPositions.begin(), sortedPositions.end(), [](glm::vec3 v1, glm::vec3 v2) {
                return v1.x < v2.x;
                });

            float xStart = sortedPositions[0].x;
            float xEnd = sortedPositions[sortedPositions.size() - 1].x;
            int cnt = 1000;
            float eps = (xEnd - xStart) / cnt;
            vector<float> xs(cnt + 1);
            float cur = xStart;
            for (int i = 0; i <= cnt; i++)
            {
                xs[i] = cur;
                cur += eps;
            }

            auto ys = lagrange(sortedPositions, xs);
            vector<glm::vec3> points(cnt + 1);
            for (int i = 0; i < xs.size(); i++)
            {
                points[i] = glm::vec3(xs[i], ys[i], 0);
            }
            lr->m_Width = 2.0f;
            lr->DrawLines(points);
        }
	}

    inline virtual void OnImGuiRender() override
    {
        ImGui::Begin("Debug");

        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 winPos = m_ViewportWindow->ConvertToWinPos(scrPos);
        ImGui::DragFloat2("WinPos", (float*)&winPos);
        ImGui::DragFloat2("WorldPos", (float*)&worldPos);
        ImGui::DragFloat2("vMin", (float*)&vMin);
        ImGui::DragFloat2("vMax", (float*)&vMax);
        ImGui::DragInt("in window", (int*)&mouseInWindow);
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
