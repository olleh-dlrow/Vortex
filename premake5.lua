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

include "Vortex/vendor/GLFW"
include "Vortex/vendor/Glad"
include "Vortex/vendor/imgui"

project "Vortex"
    location "Vortex"
    kind "SharedLib"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "vtpch.h"
    pchsource "Vortex/src/vtpch.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
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
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "VT_PLATFORM_WINDOWS",
            "VT_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
        
    filter "configurations:Debug"
        defines "VT_DEBUG"
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "VT_RELEASE"
        buildoptions "/MD"
        symbols "On"    
        
    filter "configurations:Dist"
        defines "VT_DIST"
        buildoptions "/MD"
        symbols "On"       



project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    
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
        "Vortex/src"
    }
    
    links
    {
        "Vortex"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        
        defines
        {
            "VT_PLATFORM_WINDOWS"
        }
        
    filter "configurations:Debug"
        defines "VT_DEBUG"
        buildoptions "/MDd"
        symbols "On"
        
    filter "configurations:Release"
        defines "VT_RELEASE"
        buildoptions "/MD"
        symbols "On"    
        
    filter "configurations:Dist"
        defines "VT_DIST"
        buildoptions "/MD"
        symbols "On"           
 