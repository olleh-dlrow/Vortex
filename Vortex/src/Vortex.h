#pragma once
//#include"vtpch.h"

//for use by vortex application
#include "Vortex/Core/Core.h"

#include"Vortex/Core/Application.h"
#include"Vortex/Core/Window.h"
#include"Vortex/ImGui/ImGuiLayer.h"
#include"Vortex/Core/Log.h"

#include "Vortex/Core/Timestep.h"
#include "Vortex/Core/Time.h"

#include "Vortex/Core/Input.h"
#include "Vortex/Core/KeyCodes.h"
#include "Vortex/Core/MouseButtonCodes.h"

// some basic geometry objects
#include "Vortex/Geo/Ray.h"
#include "Vortex/Geo/DrawGeoConfig.h"
#include "Vortex/Geo/Mesh.h"
#include "Vortex/Geo/Rect.h"

// --- Renderer -------------------------
#include "Vortex/Renderer/Renderer.h"

#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/Shader.h"
#include "Vortex/Renderer/Texture.h"
#include "Vortex/Renderer/VertexArray.h"
#include "Vortex/Renderer/GraphicsContext.h"
#include "Vortex/Renderer/Batch.h"

#include "Vortex/Renderer/Camera.h"
#include "Vortex/Renderer/Material.h"
// --------------------------------------


#include "Vortex/ImGui/ViewportWindow.h"

// --- Scene -------------------------
#include "Vortex/Scene/Scene.h"
#include "Vortex/Scene/SceneImporter.h"
#include "Vortex/Scene/Entity.h"
#include "Vortex/Scene/Component.h"
/*
tips:
this head file should put in the last, 
it will reference the content of above head files.

-- has moved to SandboxApp.cpp
*/
// --- Entry Point ----------------------
// #include"Vortex/Core/EntryPoint.h"
// --------------------------------------
