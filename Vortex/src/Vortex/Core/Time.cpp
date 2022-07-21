#include "vtpch.h"
#include "Time.h"
#include <GLFW/glfw3.h>

namespace Vortex
{
	float Time::GetSeconds()
	{
		return static_cast<float>(glfwGetTime());
	}
}

