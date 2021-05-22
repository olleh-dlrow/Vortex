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
        if ( Vortex::Input::IsKeyPressed(VT_KEY_TAB) )
        {
            VT_TRACE("Tab is pressed (poll)!");
        }
    }

    void OnEvent(Vortex::Event& event) override
    {
        if ( event.GetEventType() == Vortex::EventType::KeyPressed )
        {
            Vortex::KeyPressedEvent& e = (Vortex::KeyPressedEvent&)event;
            if ( e.GetKeyCode() == VT_KEY_TAB )
            {
                VT_TRACE("Tab is pressed (event)!");
            }
            VT_TRACE("{0}", (char)e.GetKeyCode());
        }
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