#include <Vortex.h>
#include <Vortex/Core/EntryPoint.h>


#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TemplateLayer.h"
#include "ExampleLayer.h"
//#include "CameraTest.h"
#include "RayCastTest.h"

/*
tips:
To copy the dll automatically, you can 
add below command to sandbox build events, pre-build event:
xcopy "$(SolutionDir)bin\$(Configuration)-$(Platform)\Vortex\Vortex.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox" /y
*/

class Sandbox: public Vortex::Application {
public:
    Sandbox() {
        PushLayer(new RayCastTest());
    }
    ~Sandbox() {
    
    }
};

Vortex::Application* Vortex::CreateApplication() {
    VT_INFO("Create application");
    return new Sandbox();
}