#include "vtpch.h"
#include "OrthographicCameraController.h"

#include "Vortex/Core/Input.h"
#include "Vortex/Core/KeyCodes.h"

namespace Vortex
{
    // aspectRadio gives the radio of screen's width and height
    // zoomLevel gives the top of orthographic camera
    OrthographicCameraController::OrthographicCameraController(float aspectRatio,
                                                               bool rotation)
        : m_AspectRatio(aspectRatio),
          m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
                   -m_ZoomLevel, m_ZoomLevel),
          m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        // position transform, move in world space
        if (Input::IsKeyPressed(VT_KEY_A)) {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
        } else if (Input::IsKeyPressed(VT_KEY_D)) {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
        }

        if (Input::IsKeyPressed(VT_KEY_W)) {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
        } else if (Input::IsKeyPressed(VT_KEY_S)) {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) *
                                  m_CameraTranslationSpeed * ts;
        }

        // rotation transform
        if (m_Rotation) {
            if (Input::IsKeyPressed(VT_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(VT_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;

            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);

        // the bigger your camera rect is, the faster you move the camera
        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    // dispatch concrete event to function to handle
    void OrthographicCameraController::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(
            VT_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(
            VT_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent &e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
                               m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                               m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent &e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
                               m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                               m_ZoomLevel);
        return false;
    }
}
