#include <Vortex.h>

/*
tips:
To copy the dll automatically, you can 
add below command to sandbox build events, pre-build event:
xcopy "$(SolutionDir)bin\$(Configuration)-$(Platform)\Vortex\Vortex.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox" /y
*/

class Sandbox: public Vortex::Application {
public:
    Sandbox() {
        
    }
};

Vortex::Application* Vortex::CreateApplication() {
    return new Sandbox();
}