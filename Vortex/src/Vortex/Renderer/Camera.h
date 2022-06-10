#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Vortex/Core/Timestep.h"

#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/KeyEvent.h"
#include "Vortex/Geo/Ray.h"

namespace Vortex
{
	enum class CameraProjMode 
	{
		Perspective, Orthographic
	};

	struct PerspParam
	{
		PerspParam() {};
		PerspParam(float _degFov) :degFOV(_degFov) {}
		float degFOV = 45.0f;
	};

	struct OrthoParam
	{
		OrthoParam() {}
		OrthoParam(float _zoomLevel) : zoomLevel(_zoomLevel) {}
		float zoomLevel = 5.0f;
	};

	class Camera
	{
		friend class ViewportWindow;
	public:
		Camera(PerspParam pram, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float aspect=1.0f, float zn = 0.1f, float zf = 100.0f);
		Camera(OrthoParam pram, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float aspect=1.0f, float zn = 0.1f, float zf = 100.0f);
		~Camera() = default;

		inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
		glm::mat4 GetProjMatrix() const;
		inline glm::mat4 GetViewProjMatrix() const { return GetProjMatrix() * GetViewMatrix(); }

		void CastRay(const glm::vec2& normPos, Ray& r);
		void ReCalculateCameraVectors();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void RenderConfigGUI();
	private:
		void Init();
		void PerspectiveModeHandleInput(Timestep ts);
		void OrthoModeHandleInput(Timestep ts);

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	private:
		CameraProjMode			m_ProjMode;
		glm::vec3				m_Position;
		glm::vec3				m_Front;
		glm::vec3				m_Up;
		glm::vec3				m_Right;
		glm::vec3				m_WorldUp;
		float					m_Yaw;
		float					m_Pitch;
		float					m_MovementSpeed;
		float					m_MoveSensitivity;
		float					m_ZoomSensitivity;
		float					m_ZNear;
		float					m_ZFar;
		float					m_Aspect;
		PerspParam				m_PerspParam;
		OrthoParam				m_OrthoParam;
		// rotate
		bool					m_IsRotating = false;
		float					m_LastMouseX = 0;
		float					m_LastMouseY = 0;
	};

}
