#pragma once

#include "vtpch.h"

#include "Vortex/Core/Core.h"
#include "Vortex/Events/Event.h"

namespace Vortex {
    
    struct WindowProps {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Vortex Engine",
                    unsigned int width = 1680,
                    unsigned int height = 960)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    //Interface representing a desktop system based window
    class Window {
    public:
        /*
        tips:
        using equal to typedef but looks more clearly
        */
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int Width() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        // set vertical sync
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
}