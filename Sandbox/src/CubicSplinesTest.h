#pragma once

#include "EditorLayer.h"
#include "Eigen/Dense"
#include <Vortex/Scene/PointRendererComponent.h>
#include <Vortex/Scene/LineRendererComponent.h>

using std::vector;

class CubicSplinesTest : public EditorLayer
{
    class ControlPoint
    {
    public:
        // tangent mode
        constexpr static int TAN_NONE = 0;
        constexpr static int TAN_G0 = 1;
        constexpr static int TAN_G1 = 2;
        constexpr static int TAN_G2 = 3;

        ControlPoint() {}
        ControlPoint(const glm::vec2& pos):f(pos){}
        glm::vec2 f;            // f(t), equal position
        glm::vec2 Df_iMinusOne;
        glm::vec2 Df_i;
        float t;
        int tangentMode = TAN_G2;
    };

    class CubicSplineFunction
    {
    public:
        float ti = 0;
        float ti1 = 0;
        glm::vec2 ai;
        glm::vec2 bi;
        glm::vec2 ci;
        glm::vec2 di;

        glm::vec2 F(float t)
        {
            t = t - ti;
            return ai + bi * t + ci * t * t + di * t * t * t;
        }

        glm::vec2 DF(float t)
        {
            t = t - ti;
            return bi + 2.0f * ci * t + 3.0f * di * t * t * t;
        }

        glm::vec2 DDF(float t)
        {
            t = t - ti;
            return 2.0f * ci + 6.0f * di * t * t;
        }
    };

    Vortex::PointRendererComponent* pr;
    Vortex::LineRendererComponent* lr;

    Vortex::Ref<Vortex::Shader> screenShader;

    const glm::vec3 INVALID_POINT = glm::vec3(std::numeric_limits<float>::max());

    //glm::vec3 color = glm::vec3(0.3f, 0.8f, 0.2f);
    glm::vec4 pointColor1 = glm::vec4(0.2, 0.8, 0.6, 1);  // normal point
    glm::vec4 pointColor2 = glm::vec4(0.2, 0.3, 0.9, 1);  // mouse in point
    glm::vec4 pointColor3 = glm::vec4(0.9, 0.1, 0.1, 1);  // point in edit
    glm::vec4 pointColor4 = glm::vec4(1.0, 1.0, 1.0, 1);  // control tangent point

    glm::vec4 lineColor1 = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f); // normal line
    glm::vec4 lineColor2 = glm::vec4(1.0, 1.0, 1.0, 1);   // control tangent

    float lineWidth = 1.0f;

    ImVec2 worldPos;
    //vector<glm::vec3> positions;
    //vector<glm::vec4> colors;

    //vector<ControlPoint> controlPoints;

    // multiple lines
    using PosList = vector<glm::vec3>;
    using ColList = vector<glm::vec4>;
    using CtlList = vector<ControlPoint>;

    vector<PosList> posLists;
    vector<ColList> colorLists;
    vector<CtlList> ctlPointLists;
    vector<bool>    curvesInitStates;
    int curLineIndex = -1;

    // add, finish, delete line state
    bool isAddingLine;

    int MAX_LINE_CNT_IN_A_SEG = 20;
    vector<glm::vec3> linePoints;

    // debug for second derivative
    int DDfIdx = 1;
    glm::vec2 leftDDf = INVALID_POINT;
    glm::vec2 rightDDf = INVALID_POINT;

    // draw control points state
    bool hideControlPoints = false;

    // edit point state
    int curEditPointIndex = -1;

    // add point state
    bool clicked = false;

    // drag point state
    bool isDragging = false;
    bool mouseInPoint = false;
    int curChosenIndex = 0;

    // mouse in window state
    bool mouseInWindow = false;
    
    // control tangent state
    glm::vec3 leftTangentPoint = INVALID_POINT;
    glm::vec3 rightTangentPoint = INVALID_POINT;
    bool mouseInLeftTangentPoint = false;
    bool mouseInRightTangentPoint = false;
    
    bool isDragTangentPoint = false;
    bool isDragLeftTangentPoint = false;
    bool isDragRightTangentPoint = false;

    // draw lines state
    bool curveInitialized = false;
    bool canDrawLines = false;
public:
    // the second derivative in two ends is 0.
    // 
    // you can use chasing method to solve the linear equations.
    // if one point moved, all the other points in line will move, the further the distance,
    // the less the movement of other points.
    void NatualSplines(const vector<glm::vec3>& input, vector<glm::vec3>& output)
    {
        auto& positions = posLists[curLineIndex];
        auto& colors = colorLists[curLineIndex];
        auto& controlPoints = ctlPointLists[curLineIndex];

        int n = input.size();
        int neededSize = std::max(0, (n - 1) * (MAX_LINE_CNT_IN_A_SEG) + 1);
        if (neededSize != output.size())
            output.resize(neededSize);
        // AM = B
        Eigen::MatrixXf A = Eigen::MatrixXf::Zero(n, n);
        Eigen::VectorXf B = Eigen::VectorXf::Zero(n);
        // construct Matrix A
        A(0, 0) = 1; A(n - 1, n - 1) = 1;
        // hi = xi+1 - xi
        for (int i = 0; i < n - 2; i++)
        {
            float ti = i, ti1 = i + 1, ti2 = i + 2;
            float hi = ti1 - ti;
            float hi1 = ti2 - ti1;
            A(i + 1, i) = hi;
            A(i + 1, i + 1) = 2 * (hi + hi1);
            A(i + 1, i + 2) = hi1;
        }

        // construct Vector B
        // ftIdx: x->0, y->1, z->2
        auto calB = [&](int ftIdx) 
        {
            B(0) = 0; B(n - 1) = 0;
            for (int i = 0; i < n - 2; i++)
            {
                float ti = i, ti1 = i + 1, ti2 = i + 2;
                float hi = ti1 - ti;
                float hi1 = ti2 - ti1;
                float fti = input[i][ftIdx], fti1 = input[i + 1][ftIdx], fti2 = input[i + 2][ftIdx];
                B(i + 1) = 6 * ((fti2 - fti1) / hi1 - (fti1 - fti) / hi);
            }
        };

        Eigen::MatrixXf invA = A.inverse();
        Eigen::VectorXf M;

        auto calOutput = [&](int ftIdx)
        {
            // for n-1 lines
            int outputIdx = 0;
            for (int i = 0; i < n - 1; i++)
            {
                float fti = input[i][ftIdx], fti1 = input[i + 1][ftIdx];
                float ti = i, ti1 = i + 1;

                float mi = M(i), mi1 = M(i + 1);
                float ai = fti;
                float hi = ti1 - ti;
                float bi = (fti1 - fti) / hi - mi * hi / 2 - (mi1 - mi) * hi / 6;
                float ci = mi / 2;
                float di = (mi1 - mi) / (6 * hi);

                controlPoints[i].t = ti;
                controlPoints[i + 1].t = ti1;

                controlPoints[i + 1].Df_iMinusOne[ftIdx] = bi + 2 * ci * hi * hi + 3 * di * hi * hi * hi;
                controlPoints[i].Df_i[ftIdx] = bi;

                // generate x
                float eps = (ti1 - ti) / MAX_LINE_CNT_IN_A_SEG;
                float t = ti;
                for (int tIdx = 0; tIdx < MAX_LINE_CNT_IN_A_SEG; tIdx++, t = std::min(t + eps, ti1))
                {
                    float dt = t - ti;
                    output[outputIdx++][ftIdx] = ai + bi * dt + ci * dt * dt + di * dt * dt * dt;
                }
            }
            output[outputIdx] = glm::vec3(controlPoints[n - 1].f, 0);
        };

        // X(t)
        calB(0);
        M = invA * B;
        calOutput(0);

        // Y(t)
        calB(1);
        M = invA * B;
        calOutput(1);
    }

    CubicSplineFunction CalculateSpline(const ControlPoint& pi, const ControlPoint& pi1)
    {
        CubicSplineFunction func;
        float ti = 0;
        float ti1 = 0;
        glm::vec2 ai;
        glm::vec2 bi;
        glm::vec2 ci;
        glm::vec2 di;

        ti = pi.t;
        ti1 = pi1.t;
        float hi = ti1 - ti;
        glm::vec2 fi = pi.f;
        glm::vec2 fi1 = pi1.f;
        glm::vec2 Dfi = pi.Df_i;
        glm::vec2 Dfi1 = pi1.Df_iMinusOne;

        ai = fi;
        bi = Dfi;
        di = (2.0f * fi + Dfi * hi + Dfi1 * hi - 2.0f * fi1) / (hi * hi * hi);
        ci = (Dfi1 - Dfi - 3.0f * di * hi * hi) / (2 * hi);

        func.ai = ai;
        func.bi = bi;
        func.ci = ci;
        func.di = di;
        func.ti = ti;
        func.ti1 = ti1;
        return func;
    }

    void CalculateSplinePointsWithTangent(const vector<ControlPoint>& controlPoints, vector<glm::vec3>& output)
    {
        int n = controlPoints.size();
        // seg count * line cnt in one seg + final point
        int neededSize = std::max(0, (n - 1) * (MAX_LINE_CNT_IN_A_SEG) + 1);
        if (neededSize != output.size())
            output.resize(neededSize);
        // for n-1 lines
        int outputIdx = 0;
        for (int i = 0; i < n - 1; i++)
        {
            const ControlPoint& pi  = controlPoints[i];
            const ControlPoint& pi1 = controlPoints[i + 1];
            CubicSplineFunction spline = CalculateSpline(pi, pi1);

            float ti = spline.ti;
            float ti1 = spline.ti1;
            float eps = (ti1 - ti) / MAX_LINE_CNT_IN_A_SEG;
            
            float t = ti;
            for (int tIdx = 0; tIdx < MAX_LINE_CNT_IN_A_SEG; tIdx++, t = std::min(t + eps, ti1))
            {
                output[outputIdx++] = glm::vec3(spline.F(t), 0);
            }
        }
        output[outputIdx] = glm::vec3(controlPoints[n - 1].f, 0);
    }

    void SetCurrentLineIndex(int idx)
    {
        if (curLineIndex != -1)
            curvesInitStates[curLineIndex] = curveInitialized;
        curLineIndex = idx;
        curveInitialized = curvesInitStates[idx];
    }

    void AddLine()
    {
        posLists.push_back(PosList());
        colorLists.push_back(ColList());
        ctlPointLists.push_back(CtlList());
        curvesInitStates.push_back(false);
        SetCurrentLineIndex(posLists.size() - 1);
        curEditPointIndex = -1;
    }

    CubicSplinesTest()
    {
        Vortex::Scene* scene = GetEditorScene();
        auto e1 = scene->AddEntity("e1");
        pr = e1->AddComponent<Vortex::PointRendererComponent>();
        lr = e1->AddComponent<Vortex::LineRendererComponent>();

        // add first line
        AddLine();

        // postprocess
        screenShader = Vortex::Shader::Create("assets/shaders/Screen.glsl");
    }

    float offset = 1.0f / 300.0f;

    virtual void OnPostProcess(Vortex::Texture2D& renderTexture)
    {
        screenShader->Bind();
        screenShader->SetFloat("offset", offset);
        screenShader->SetInt("screenTexture", 0);
        renderTexture.Bind();
    }

    // https://blog.mapbox.com/drawing-antialiased-lines-with-opengl-8766f34192dc
    void DrawLinesTest(const PosList& positions)
    {
        uint32_t len = positions.size();
        uint32_t vbSize = len * 2 * sizeof(glm::vec3);
        // calculate draw positions
        PosList drawPositions(len * 2);
        float width = lineWidth * 0.01f;
        for (int i = 0; i < len; i++)
        {
            glm::vec2 lastNorm(0), nextNorm(0);
            if(i != 0)
            {
                glm::vec2 dir = positions[i] - positions[i - 1];
                lastNorm = glm::vec2(-dir.y, dir.x);
                lastNorm = glm::normalize(lastNorm);
            }
            if (i != len - 1)
            {
                glm::vec2 dir = positions[i + 1] - positions[i];
                nextNorm = glm::vec2(-dir.y, dir.x);
                nextNorm = glm::normalize(nextNorm);
            }
            glm::vec2 avgNorm = glm::normalize(lastNorm + nextNorm) * width;
            drawPositions[i * 2] = positions[i] + glm::vec3(avgNorm, 0);
            drawPositions[i * 2 + 1] = positions[i] + glm::vec3(-avgNorm, 0);
        }
        Vortex::Ref<Vortex::VertexArray> VA = Vortex::VertexArray::Create();
        Vortex::Ref<Vortex::VertexBuffer> VB = Vortex::VertexBuffer::Create((float*)&drawPositions[0], vbSize);
        Vortex::BufferLayout layout = {
            {Vortex::ShaderDataType::Float3, "a_Position"}
        };
        
        VB->SetLayout(layout);
        VA->Bind();
        VA->AddVertexBuffer(VB);
        Vortex::Ref<Vortex::Shader> shader = Vortex::Shader::Create("assets/shaders/Line.glsl");
        shader->Bind();
        shader->SetFloat4("u_Color", lineColor1);
        shader->SetMat4("u_ViewProjection", GetCamera().GetViewProjMatrix());
        // Vortex::DrawTriangleConfig attr(len * 2, GL_TRIANGLE_STRIP);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, len * 2);
    }

    inline void OnUpdate(Vortex::Timestep ts) override
    {
        EditorLayer::OnUpdate(ts);

        auto& positions = posLists[curLineIndex];
        auto& colors = colorLists[curLineIndex];
        auto& controlPoints = ctlPointLists[curLineIndex];

        Vortex::Camera& cam = GetCamera();
        // cast ray
        ImVec2 scrPos = ImGui::GetMousePos();
        mouseInWindow = m_ViewportWindow->IsInside(scrPos);

        ImVec2 normPos = m_ViewportWindow->ConvertToNormalizedPos(scrPos);
        Vortex::Ray r;
        cam.CastRay(glm::vec2(normPos.x, normPos.y), r);
        worldPos = ImVec2(r.origin.x, r.origin.y);

        // put all if statements in the first level
        //
        // handle input event
        if (Vortex::Input::IsMouseButtonPressed(VT_MOUSE_BUTTON_1))
        {
            // add point
            bool canAddPoint = !mouseInPoint &&
                !mouseInLeftTangentPoint &&
                !mouseInRightTangentPoint &&
                mouseInWindow &&
                m_ViewportWindow->IsFocused() &&
                !clicked;
            if (canAddPoint)
            {
                glm::vec2 newPos = glm::vec2(worldPos.x, worldPos.y);
                positions.emplace_back(glm::vec3(newPos, 0));
                controlPoints.emplace_back(newPos);

                colors.emplace_back(pointColor1);
            }
            
            if (canAddPoint && curveInitialized)
            {
                // use ti's first derivative and second derivative to calculate cubic spline
                // often appear very large derivative for new point
                auto algo1 = [&]() {
                    int curPointIdx = controlPoints.size() - 1;
                    int lastPointIdx = curPointIdx - 1;
                    int i = lastPointIdx;
                    ControlPoint& pi = controlPoints[i];
                    ControlPoint& pi1 = controlPoints[i + 1];
                    // necessary
                    pi1.t = i + 1;
                    CubicSplineFunction func_iMinusOne = CalculateSpline(controlPoints[i - 1], controlPoints[i]);
                    CubicSplineFunction func_i;
                    float hi = pi1.t - pi.t;
                    glm::vec2 fi = pi.f;
                    glm::vec2 fi1 = pi1.f;
                    glm::vec2 Dfi = func_iMinusOne.DF(pi.t);
                    pi.Df_i = pi.Df_iMinusOne;
                    glm::vec2 DDfi = func_iMinusOne.DDF(pi.t);
                    func_i.ai = fi;
                    func_i.bi = Dfi;
                    func_i.ci = DDfi / 2.0f;
                    func_i.di = (fi1 - func_i.ai - func_i.bi * hi - func_i.ci * hi * hi) / (hi * hi * hi);
                    func_i.ti = pi.t;
                    func_i.ti1 = pi1.t;
                    pi1.Df_iMinusOne = func_i.DF(pi1.t);
                    // CalculateSplinePointsWithTangent(controlPoints, linePoints);
                };

                // set ti+1's second derivative to zero to calculate cubic spline
                // the effect is better
                auto algo2 = [&]() {
                    int curPointIdx = controlPoints.size() - 1;
                    int lastPointIdx = curPointIdx - 1;
                    int i = lastPointIdx;
                    ControlPoint& pi = controlPoints[i];
                    ControlPoint& pi1 = controlPoints[i + 1];
                    // necessary
                    pi1.t = i + 1;
                    CubicSplineFunction func_iMinusOne = CalculateSpline(controlPoints[i - 1], controlPoints[i]);
                    CubicSplineFunction func_i;
                    float hi = pi1.t - pi.t;
                    glm::vec2 fi = pi.f;
                    glm::vec2 fi1 = pi1.f;
                    glm::vec2 Dfi = func_iMinusOne.DF(pi.t);
                    pi.Df_i = pi.Df_iMinusOne;
                    glm::vec2 DDfi = func_iMinusOne.DDF(pi.t);
                    func_i.ai = fi;
                    func_i.bi = Dfi;
                    func_i.di = (func_i.ai + func_i.bi * hi - fi1) / (2.0f * hi * hi * hi);
                    func_i.ci = -3.0f * func_i.di * hi;
                    func_i.ti = pi.t;
                    func_i.ti1 = pi1.t;
                    pi1.Df_iMinusOne = func_i.DF(pi1.t);
                    // CalculateSplinePointsWithTangent(controlPoints, linePoints);
                };

                algo2();
            }

            // drag tangent point
            if (mouseInLeftTangentPoint &&
                !clicked &&
                !isDragLeftTangentPoint &&
                !isDragRightTangentPoint
                )
            {
                isDragLeftTangentPoint = true;
            }
            if (mouseInRightTangentPoint &&
                !clicked &&
                !isDragLeftTangentPoint &&
                !isDragRightTangentPoint
                )
            {
                isDragRightTangentPoint = true;
            }

            // drag this point, this should put before edit this point
            if (mouseInPoint &&
                !mouseInLeftTangentPoint &&
                !mouseInRightTangentPoint &&
                !clicked &&
                !isDragging &&
                curEditPointIndex == curChosenIndex
                )
            {
                isDragging = true;
            }
            // edit this point
            if (mouseInPoint && // mouse in point
                !mouseInLeftTangentPoint &&
                !mouseInRightTangentPoint &&
                !clicked &&     // first click
                curEditPointIndex != curChosenIndex // this point is not editting
                )
            {
                curEditPointIndex = curChosenIndex;
                colors[curEditPointIndex] = pointColor3;
            }
            // this button has clicked, enter repeat state
            if (!clicked)
            {
                clicked = true;
            }
        }
        if (Vortex::Input::IsMouseButtonReleased(VT_MOUSE_BUTTON_1))
        {
            if (clicked)
            {
                clicked = false;
            }
            if (isDragging)
            {
                isDragging = false;
            }
            if (isDragLeftTangentPoint)
            {
                isDragLeftTangentPoint = false;
            }
            if (isDragRightTangentPoint)
            {
                isDragRightTangentPoint = false;
            }
        }

        // erase point
        if (Vortex::Input::IsKeyPressed(VT_KEY_E))
        {
            if (mouseInPoint && !isDragging)
            {
                positions.erase(positions.begin() + curChosenIndex);
                colors.erase(colors.begin() + curChosenIndex);
                controlPoints.erase(controlPoints.begin() + curChosenIndex);
                // reset t
                for (int i = 0; i < controlPoints.size(); i++)
                {
                    controlPoints[i].t = i;
                }
                // CalculateSplinePointsWithTangent(controlPoints, linePoints);
            }
            if (mouseInPoint &&
                !isDragging &&
                curEditPointIndex == curChosenIndex // this point is editting
                )
            {
                curEditPointIndex = -1;
            }
            // check cur point size
            if(positions.size() == 0)
            {
                curveInitialized = false;
            }
        }

        if (isDragging)
        {
            glm::vec2 newPos = glm::vec2(worldPos.x, worldPos.y);
            positions[curChosenIndex] = glm::vec3(newPos, 0);
            controlPoints[curChosenIndex].f = newPos;
            // CalculateSplinePointsWithTangent(controlPoints, linePoints);
        }

        // traverse all points, find current chosen point
        mouseInPoint = false; 
        auto checkMouseInPoint = [&](const glm::vec3& pos)
        {
            constexpr float size = Vortex::PointRendererComponent::UNIT_POINT_SIZE;
            glm::mat4 I = glm::identity<glm::mat4>();
            glm::vec3 vMin = glm::vec3(-0.5, -0.5, 0);
            glm::vec3 vMax = glm::vec3(0.5, 0.5, 0);
            // multiply model matrix, convert to world coordinate
            vMin = glm::translate(I, pos) * glm::scale(I, { size, size, 1.0f }) * glm::vec4(vMin, 1);
            vMax = glm::translate(I, pos) * glm::scale(I, { size, size, 1.0f }) * glm::vec4(vMax, 1);
            // mouse in point i
            return worldPos.x >= vMin.x && worldPos.x <= vMax.x && worldPos.y >= vMin.y &&
                worldPos.y <= vMax.y;
        };
       
        glm::mat4 I = glm::identity<glm::mat4>();
        for (int i = 0; i < positions.size(); i++)
        {
            glm::vec3& pos = positions[i];
            bool inPointI = checkMouseInPoint(pos);

            if (inPointI)
            {
                mouseInPoint = true;
            }
            if (inPointI && !isDragging)
            {
                curChosenIndex = i;
            }
            if (inPointI && curChosenIndex != curEditPointIndex)
            {
                colors[i] = pointColor2;
            }
            if (!inPointI &&
                curEditPointIndex != i // this point is not editting
                )
            {
                colors[i] = pointColor1;
            }
        }

        ////////////////////////////////////////////
        // START FROM HERE
        ////////////////////////////////////////////
        lr->m_Width = lineWidth;

        // paint control points
        if (!hideControlPoints)
        {
            pr->DrawPoints(positions, 1.0f, colors);
        }

        canDrawLines = positions.size() > 1;
        // draw tangent control point
        bool canDrawTangent = canDrawLines &&
            curEditPointIndex != -1;

        // draw left tangent
        if (canDrawTangent && curEditPointIndex != 0)
        {
            glm::vec2 tangent = controlPoints[curEditPointIndex].Df_iMinusOne;
            glm::vec3 rightPoint = positions[curEditPointIndex];
            glm::vec3 leftPoint = rightPoint - glm::vec3(tangent, 0);
            lr->m_Color = lineColor2;
            lr->DrawLines({ leftPoint, rightPoint });
            pr->DrawPoints({ leftPoint }, 1.0f, pointColor4);

            leftTangentPoint = leftPoint;
            mouseInLeftTangentPoint = checkMouseInPoint(leftTangentPoint);
        }
        if (curEditPointIndex == 0)
        {
            leftTangentPoint = INVALID_POINT;
        }

        // draw right tangent
        if (canDrawTangent && curEditPointIndex != positions.size() - 1)
        {
            glm::vec2 tangent = controlPoints[curEditPointIndex].Df_i;
            glm::vec3 leftPoint = positions[curEditPointIndex];
            glm::vec3 rightPoint = leftPoint + glm::vec3(tangent, 0);
            lr->m_Color = lineColor2;
            lr->DrawLines({ leftPoint, rightPoint });
            pr->DrawPoints({ rightPoint }, 1.0f, pointColor4);

            rightTangentPoint = rightPoint;
            mouseInRightTangentPoint = checkMouseInPoint(rightTangentPoint);
        }
        if (curEditPointIndex == positions.size() - 1)
        {
            rightTangentPoint = INVALID_POINT;
        }

        // drag tangent point
        if (isDragLeftTangentPoint)
        {
            leftTangentPoint = glm::vec3(worldPos.x, worldPos.y, 0);
            glm::vec3& leftPoint = leftTangentPoint;
            glm::vec3& rightPoint = positions[curEditPointIndex];

            glm::vec2 tangent = rightPoint - leftPoint;
            ControlPoint& p = controlPoints[curEditPointIndex];
            p.Df_iMinusOne = tangent;

            switch (p.tangentMode)
            {
            case ControlPoint::TAN_G0:
                break;
            case ControlPoint::TAN_G1:
                // direction is same
                p.Df_i = glm::length(p.Df_i) * glm::normalize(p.Df_iMinusOne);
                break;
            case ControlPoint::TAN_G2:
                // value is same
                p.Df_i = p.Df_iMinusOne;
                break;
            default:
                break;
            }

            // CalculateSplinePointsWithTangent(controlPoints, linePoints);
        }
        if (isDragRightTangentPoint)
        {
            rightTangentPoint = glm::vec3(worldPos.x, worldPos.y, 0);
            glm::vec3& rightPoint = rightTangentPoint;
            glm::vec3& leftPoint = positions[curEditPointIndex];

            glm::vec2 tangent = rightPoint - leftPoint;
            ControlPoint& p = controlPoints[curEditPointIndex];
            p.Df_i = tangent;

            switch (p.tangentMode)
            {
            case ControlPoint::TAN_G0:
                break;
            case ControlPoint::TAN_G1:
                // direction is same
                p.Df_iMinusOne = glm::length(p.Df_iMinusOne) * glm::normalize(p.Df_i);
                break;
            case ControlPoint::TAN_G2:
                // value is same
                p.Df_iMinusOne = p.Df_i;
                break;
            default:
                break;
            }

            // CalculateSplinePointsWithTangent(controlPoints, linePoints);
        }

        // calculate point[DDfIdx] DDf
        if (positions.size() >= 3)
        {
            CubicSplineFunction leftFunc = CalculateSpline(controlPoints[DDfIdx - 1], controlPoints[DDfIdx]);
            CubicSplineFunction rightFunc = CalculateSpline(controlPoints[DDfIdx], controlPoints[DDfIdx + 1]);

            leftDDf = leftFunc.DDF(DDfIdx);
            rightDDf = rightFunc.DDF(DDfIdx);
        }

        for (int i = 0; i < ctlPointLists.size(); i++)
        {
            lr->m_Color = lineColor1;
            // not current line
            if (i != curLineIndex || (i == curLineIndex && curveInitialized))
            {
                CalculateSplinePointsWithTangent(ctlPointLists[i], linePoints);
                //lr->DrawLines(linePoints);
                //pr->DrawPoints(linePoints, lineWidth, lineColor1);
                DrawLinesTest(linePoints);
            }
        }
    }

    inline virtual void OnImGuiRender() override
    {
        auto& positions = posLists[curLineIndex];
        auto& colors = colorLists[curLineIndex];
        auto& controlPoints = ctlPointLists[curLineIndex];

        ImGui::Begin("Debug");

        ImVec2 scrPos = ImGui::GetMousePos();
        ImVec2 winPos = m_ViewportWindow->ConvertToWinPos(scrPos);
        ImGui::DragFloat2("InnerWinPos", (float*)&winPos);
        ImGui::DragFloat2("WorldPos", (float*)&worldPos);
        ImGui::Text("MouseInWindow: %s", mouseInWindow ? "yes" : "no");
        ImGui::Text("clicked: %s", clicked ? "yes" : "no");
        ImGui::Text("MouseInPoint: %s", mouseInPoint ? "yes" : "no");
        ImGui::DragInt("MaxPointCnt", &MAX_LINE_CNT_IN_A_SEG, 10, 10, 100000);
        ImGui::DragInt("DDfIdx", &DDfIdx, 1, 1, positions.size() - 2);
        ImGui::DragFloat2("LeftDDF", (float*)&leftDDf);
        ImGui::DragFloat2("RightDDF", (float*)&rightDDf);
        ImGui::DragFloat("LineWidth", &lineWidth);
        ImGui::ColorEdit4("LineColor", (float*)&lineColor1);
        ImGui::DragInt("SegLinePointCnt", &MAX_LINE_CNT_IN_A_SEG);
        ImGui::DragFloat("Offset", &offset, 0.001f);

        // operation
        ImGui::TextColored(ImVec4(0.8f, 0.3f, 0.2f, 1.0f), "Press E to delete Point");
        if (!curveInitialized && canDrawLines && ImGui::Button("DrawSpline"))
        {
            NatualSplines(positions, linePoints);
            curveInitialized = true;
        }

        // add line
        if (/*curveInitialized &&*/ ImGui::Button("Add Line"))
        {
            AddLine();
            curveInitialized = false;
        }

        // chose current edit line
        if (posLists.size() > 0)
        {
            const char** items = new const char*[posLists.size()];
            for (int i = 0; i < posLists.size(); i++)
            {
                char* item = new char[10];
                sprintf(item, "Line%d", i + 1);
                items[i] = item;
            }
            int idx = curLineIndex;
            if (ImGui::ListBox("Lines", &idx, items, posLists.size()))
            {
                // cancel edit
                curEditPointIndex = -1;
                SetCurrentLineIndex(idx);
            }
            for (int i = 0; i < posLists.size(); i++)
            {
                delete[] items[i];
            }
            delete[] items;
        }
        

        ImGui::Checkbox("HideControlPoint", &hideControlPoints);

        if (curEditPointIndex != -1 && ImGui::Button("CancelEdit"))
        {
            curEditPointIndex = -1;
        }

        bool isEditMiddlePoint = curEditPointIndex != -1 &&
            curEditPointIndex != 0 &&
            curEditPointIndex != controlPoints.size() - 1;

        if (isEditMiddlePoint)
        {
            ControlPoint& p = controlPoints[curEditPointIndex];
            const char* items[] = {"G0", "G1", "G2"};
            int curItem = p.tangentMode - 1;
            bool selectChanged = ImGui::ListBox("TangentMode", &curItem, items, IM_ARRAYSIZE(items));
            p.tangentMode = curItem + 1;

            if (selectChanged)
            {
                switch (p.tangentMode)
                {
                case ControlPoint::TAN_G0:
                    break;
                case ControlPoint::TAN_G1:
                    // direction is same
                    p.Df_i = glm::length(p.Df_i) * glm::normalize(p.Df_iMinusOne);
                    // CalculateSplinePointsWithTangent(controlPoints, linePoints);
                    break;
                case ControlPoint::TAN_G2:
                    // value is same
                    p.Df_i = p.Df_iMinusOne;
                    // CalculateSplinePointsWithTangent(controlPoints, linePoints);
                    break;
                default:
                    break;
                }
            }
        }

        if (ImGui::Button("Clear Current Line"))
        {
            positions.swap(vector<glm::vec3>());
            controlPoints.swap(vector<ControlPoint>());
            linePoints.swap(vector<glm::vec3>());
            curEditPointIndex = -1;
            curveInitialized = false;
        }

        ImGui::End();
    }
};
