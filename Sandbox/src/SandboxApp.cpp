#include <Vortex.h>
#include <Vortex/Core/EntryPoint.h>


#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "TemplateLayer.h"
//#include "ExampleLayer.h"
//#include "RayCastTest.h"
//#include "BatchRenderingTest.h"
//#include "EditorLayer.h"
//#include "DrawLineTest.h"
//#include "SceneTest.h"
//#include "BezierTest.h"
#include "CubicSplinesTest.h"
//#include "ModelImportTest.h"

/*
tips:
To copy the dll automatically, you can 
add below command to sandbox build events, pre-build event:
xcopy "$(SolutionDir)bin\$(Configuration)-$(Platform)\Vortex\Vortex.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox" /y
*/

class Sandbox: public Vortex::Application {
public:
    Sandbox() {
        PushLayer(new CubicSplinesTest());
    }
    ~Sandbox() {
    
    }
};

Vortex::Application* Vortex::CreateApplication() {
    VT_INFO("Create application");
    return new Sandbox();
}