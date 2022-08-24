#include <Vortex.h>
#include <Vortex/Core/EntryPoint.h>

//#include "TemplateLayer.h"
//#include "RayCastTest.h"
//#include "BatchRenderingTest.h"
//#include "EditorLayer.h"
//#include "DrawLineTest.h"
//#include "SceneTest.h"
//#include "BezierTest.h"
//#include "CubicSplinesTest.h"
//#include "ModelImportTest.h"
//#include "BlingPhongTest.h"
//#include "HDRTest.h"
//#include "CubemapTest.h"
//#include "PBRLightTest.h"
//#include "IBLTest.h"
#include "ShadowTest.h"
/*
tips:
To copy the dll automatically, you can 
add below command to sandbox build events, pre-build event:
xcopy "$(SolutionDir)bin\$(Configuration)-$(Platform)\Vortex\Vortex.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox" /y
*/

class Sandbox: public Vortex::Application {
public:
    Sandbox() {
        PushLayer(new ShadowTest());
    }
    ~Sandbox() {
    
    }
};

Vortex::Application* Vortex::CreateApplication() {
    VT_INFO("Create application");
    return new Sandbox();
}