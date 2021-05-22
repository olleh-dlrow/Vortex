workspace "Vortex"
    architecture "x64"
    startproject "Sandbox"
    
    -- Dist: distination
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Vortex/vendor/GLFW/include"
IncludeDir["Glad"] = "Vortex/vendor/Glad/include"
IncludeDir["ImGui"] = "Vortex/vendor/imgui"
IncludeDir["glm"] = "Vortex/vendor/glm"

include "Vortex/vendor/GLFW"
include "Vortex/vendor/Glad"
include "Vortex/vendor/imgui"

project "Vortex"
    location "Vortex"
    kind "SharedLib"
    language "C++"
    staticruntime "off"   -- off means the runtime libary use MDd(dll) not Md!!
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "vtpch.h"
    pchsource "Vortex/src/vtpch.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }
    
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }
    
    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
            "VT_PLATFORM_WINDOWS",
            "VT_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }
        
    filter "configurations:Debug"
        defines "VT_DEBUG"
        runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines "VT_RELEASE"
        runtime "Release"
        optimize "On"    
        
    filter "configurations:Dist"
        defines "VT_DIST"
        runtime "Release"
        optimize "On"       



project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "Vortex/vendor/spdlog/include",
        "Vortex/src",
        "%{IncludeDir.glm}"
    }
    
    links
    {
        "Vortex"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        
        defines
        {
            "VT_PLATFORM_WINDOWS"
        }
        
    filter "configurations:Debug"
        defines "VT_DEBUG"
        runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        defines "VT_RELEASE"
        runtime "Release"
        optimize "On"    
        
    filter "configurations:Dist"
        defines "VT_DIST"
        runtime "Release"
        optimize "On"           
 