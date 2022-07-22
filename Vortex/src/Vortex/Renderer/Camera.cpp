#include "vtpch.h"

#include "Camera.h"
#include <imgui.h>

#include "Vortex/Core/Core.h"
#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/KeyEvent.h"
#include "Vortex/Geo/Ray.h"
#include "Vortex/Core/Input.h"
#include "Vortex/Core/KeyCodes.h"
#include "Vortex/Core/MouseButtonCodes.h"

namespace Vortex
{
	void Camera::Init()
	{
		m_WorldUp = glm::vec3(0, 1.0f, 0);
		m_Yaw = -90.0f;
		m_Pitch = 0.0f;
		ReCalculateCameraVectors();
		m_MovementSpeed = 2.5f;
		m_MoveSensitivity = 0.1f;
		m_ZoomSensitivity = 0.5f;
	}

	Camera::Camera(PerspParam pram, glm::vec3 pos, float aspect, float zn, float zf)
		:m_PerspParam(pram), m_Position(pos), m_ZNear(zn), m_ZFar(zf), m_Aspect(aspect)
	{
		m_ProjMode = CameraProjMode::Perspective;
		Init();
	}
	Camera::Camera(OrthoParam pram, glm::vec3 pos, float aspect, float zn, float zf)
		: m_OrthoParam(pram), m_Position(pos), m_ZNear(zn), m_ZFar(zf), m_Aspect(aspect)
	{
		m_ProjMode = CameraProjMode::Orthographic;
		Init();
	}

	glm::mat4 Camera::GetProjMatrix() const
	{
		if (m_ProjMode == CameraProjMode::Perspective)
			return glm::perspective(glm::radians(m_PerspParam.degFOV), m_Aspect, m_ZNear, m_ZFar);
		if (m_ProjMode == CameraProjMode::Orthographic)
		{
			float zl = m_OrthoParam.zoomLevel;
			float hWidth = m_Aspect * zl;
			float hHeight = zl;
			return glm::ortho(-hWidth, hWidth, -hHeight, hHeight, m_ZNear, m_ZFar);
		}
		VT_ASSERT(0, "unknown projection mode!");
		return glm::mat4();
	}

	void Camera::CastRay(const glm::vec2& normPos, Ray& r)
	{
		if (CameraProjMode::Orthographic == m_ProjMode)
		{
			float camHalfW = m_Aspect * m_OrthoParam.zoomLevel;
			float camHalfH = m_OrthoParam.zoomLevel;
			r.origin = glm::vec3((normPos.x * 2 - 1) * camHalfW, ((1 - normPos.y) * 2 - 1) * camHalfH, m_ZNear);
			r.origin += m_Position;
			r.direction = m_Front;
		}
	}

	void Camera::ReCalculateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}

	void Camera::PerspectiveModeHandleInput(Timestep ts) 
	{
		float deltaPos = m_MovementSpeed * ts;
		if (Input::IsKeyPressed(VT_KEY_W))
		{
			m_Position += m_Front * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_S))
		{
			m_Position -= m_Front * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_A))
		{
			m_Position -= m_Right * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_D))
		{
			m_Position += m_Right * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_SPACE))
		{
			m_Position += m_WorldUp * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_LEFT_SHIFT))
		{
			m_Position -= m_WorldUp * deltaPos;
		}

		if (m_IsRotating)
		{
			float x = Input::GetMouseX();
			float y = Input::GetMouseY();

			float offsetX = x - m_LastMouseX;
			float offsetY = m_LastMouseY - y;

			m_Yaw += offsetX * m_MoveSensitivity;
			m_Pitch += offsetY * m_MoveSensitivity;
			m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

			m_LastMouseX = x;
			m_LastMouseY = y;
		}
	}

	void Camera::OrthoModeHandleInput(Timestep ts)
	{
		float deltaPos = m_MovementSpeed * ts;
		if (Input::IsKeyPressed(VT_KEY_W))
		{
			m_Position += m_WorldUp * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_S))
		{
			m_Position -= m_WorldUp * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_A))
		{
			m_Position -= m_Right * deltaPos;
		}
		if (Input::IsKeyPressed(VT_KEY_D))
		{
			m_Position += m_Right * deltaPos;
		}
	}

	void Camera::OnUpdate(Timestep ts)
	{
		if (m_ProjMode == CameraProjMode::Perspective)
			PerspectiveModeHandleInput(ts);
		if (m_ProjMode == CameraProjMode::Orthographic)
			OrthoModeHandleInput(ts);
		
		ReCalculateCameraVectors();
	}

	void Camera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(
			VT_BIND_EVENT_FN(Camera::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(
			VT_BIND_EVENT_FN(Camera::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(
			VT_BIND_EVENT_FN(Camera::OnMouseButtonReleased));
	}

	bool Camera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		if (m_ProjMode == CameraProjMode::Perspective)
		{
			m_PerspParam.degFOV -= e.GetYOffset() * m_ZoomSensitivity * 2;
			m_PerspParam.degFOV = glm::clamp(m_PerspParam.degFOV, 0.05f, 89.0f);
		}
		if (m_ProjMode == CameraProjMode::Orthographic)
		{
			m_OrthoParam.zoomLevel -= e.GetYOffset() * m_ZoomSensitivity * 0.25f;
			m_OrthoParam.zoomLevel = glm::clamp(m_OrthoParam.zoomLevel, 0.1f, 100.0f);
		}
		return false;
	}

	// OBSOLETE
	bool Camera::OnWindowResized(WindowResizeEvent& e)
	{
		m_Aspect = static_cast<float>(e.GetWidth()) / e.GetHeight();;
		return false;
	}

	bool Camera::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == VT_MOUSE_BUTTON_2)
		{
			if (!m_IsRotating)
			{
				m_LastMouseX = Input::GetMouseX();
				m_LastMouseY = Input::GetMouseY();
			}
			m_IsRotating = true;
		}
		return false;
	}

	bool Camera::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == VT_MOUSE_BUTTON_2)
		{
			m_IsRotating = false;
		}
		return false;
	}

	bool Camera::OnKeyPressed(KeyPressedEvent& e)
	{
		// WARNING: can't use this to handle input,
		// the speed is too slow!!
		// VT_CORE_INFO("Camera: Key Pressed");
		//int keycode = e.GetKeyCode();
		//if (keycode == VT_KEY_W)
		//{
		//	m_Position += m_Front * deltaPos;
		//}
		//if (keycode == VT_KEY_S)
		//{
		//	m_Position -= m_Front * deltaPos;
		//}
		//if (keycode == VT_KEY_A)
		//{
		//	m_Position -= m_Right * deltaPos;
		//}
		//if (keycode == VT_KEY_D)
		//{
		//	m_Position += m_Right * deltaPos;
		//}
		//if (keycode == VT_KEY_SPACE)
		//{
		//	m_Position += m_WorldUp * deltaPos;
		//}
		//if (keycode == VT_KEY_LEFT_SHIFT)
		//{
		//	m_Position -= m_WorldUp * deltaPos;
		//}
		return false;
	}

	void Camera::RenderConfigGUI()
	{
		Vortex::Camera& cam = *this;
		std::string mode = cam.m_ProjMode == Vortex::CameraProjMode::Perspective ? "Perspective" : "Orthographic";
		
		ImGui::Text("Camera Config");
		ImGui::Text("MODE: %s", mode.c_str());
		if (ImGui::Button("Persp"))
		{
			cam.m_ProjMode = Vortex::CameraProjMode::Perspective;
		}
		if (ImGui::Button("Ortho"))
		{
			cam.m_ProjMode = Vortex::CameraProjMode::Orthographic;
		}
		ImGui::DragFloat3("pos", glm::value_ptr(cam.m_Position));
		ImGui::DragFloat3("front", glm::value_ptr(cam.m_Front));
		ImGui::DragFloat3("right", glm::value_ptr(cam.m_Right));
		ImGui::DragFloat3("up", glm::value_ptr(cam.m_Up));
		ImGui::DragFloat3("world up", glm::value_ptr(cam.m_WorldUp));

		ImGui::SliderFloat("zoomLv", &cam.m_OrthoParam.zoomLevel, 0.1f, 100.0f);

		ImGui::DragFloat("fov",   &cam.m_PerspParam.degFOV);
		ImGui::DragFloat("yaw",   &cam.m_Yaw);
		ImGui::DragFloat("pitch", &cam.m_Pitch);
		ImGui::DragFloat("zNear", &cam.m_ZNear);
		ImGui::DragFloat("zFar ", &cam.m_ZFar);
		ImGui::DragFloat("aspect", &cam.m_Aspect);
		ImGui::DragFloat("speed", &cam.m_MovementSpeed);
		ImGui::DragFloat("moveSen", &cam.m_MoveSensitivity);
		ImGui::DragFloat("zoomSen", &cam.m_ZoomSensitivity);
	}
}
