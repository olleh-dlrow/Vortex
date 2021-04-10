#include "vtpch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"

#include "GLFW/glfw3.h"

namespace Vortex {

    void Application::Run() {
        WindowResizeEvent e(1200, 800);
        VT_TRACE(e);
        VT_INFO("hello, now I can load Vortex.dll automatically!!");
        //std::cout << e << std::endl;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWwindow *window = glfwCreateWindow(800, 600, "Vortex", NULL, NULL);
        glfwMakeContextCurrent(window);
        while(!glfwWindowShouldClose(window)) {
            glClearColor(0.1f, 0.3f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        //while(true);
    }
}
