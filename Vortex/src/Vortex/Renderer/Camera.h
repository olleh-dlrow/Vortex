#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Vortex/Core/Timestep.h"

#include "Vortex/Events/ApplicationEvent.h"
#include "Vortex/Events/MouseEvent.h"
#include "Vortex/Events/KeyEvent.h"

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
		float zoomLevel = 5.0f;
	};

	class Camera
	{
	public:
		Camera(PerspParam pram, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float aspect=1.0f, float zn = 0.1f, float zf = 100.0f);
		Camera(OrthoParam pram, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float aspect=1.0f, float zn = 0.1f, float zf = 100.0f);
		~Camera() = default;

		inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
		inline glm::mat4 GetProjMatrix() const 
		{
			if(m_ProjMode == CameraProjMode::Perspective)
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

		void ReCalculateCameraVectors();
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void CameraDebug();
	private:
		void Init();
		void PerspectiveModeHandleInput(Timestep ts);
		void OrthoModeHandleInput(Timestep ts);

		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	public:
		CameraProjMode m_ProjMode;
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		float m_Yaw;
		float m_Pitch;
		float m_MovementSpeed;
		float m_MoveSensitivity;
		float m_ZoomSensitivity;
		float m_ZNear;
		float m_ZFar;
		float m_Aspect;
		PerspParam m_PerspParam;
		OrthoParam m_OrthoParam;
	};

}
