#include <Vortex.h>

/*
tips:
To copy the dll automatically, you can 
add below command to sandbox build events, pre-build event:
xcopy "$(SolutionDir)bin\$(Configuration)-$(Platform)\Vortex\Vortex.dll" "$(SolutionDir)bin\$(Configuration)-$(Platform)\Sandbox" /y
*/

class ExampleLayer: public Vortex::Layer {
public:
    ExampleLayer(): Layer("Example"){}

    void OnUpdate() override
    {
        //VT_TRACE("ExampleLayer::Update");
    }

    void OnEvent(Vortex::Event& event) override
    {
        VT_TRACE("{0}", event);   
    }
};

class Sandbox: public Vortex::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        PushOverlay(new Vortex::ImGuiLayer());
    }
    ~Sandbox() {
    
    }
};

Vortex::Application* Vortex::CreateApplication() {
    VT_INFO("Create application");
    return new Sandbox();
}