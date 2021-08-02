#pragma once
//#include"vtpch.h"

//for use by vortex application
#include"Vortex/Core/Application.h"
#include"Vortex/ImGui/ImGuiLayer.h"
#include"Vortex/Core/Log.h"

#include "Vortex/Core/Timestep.h"

#include "Vortex/Core/Input.h"
#include "Vortex/Core/KeyCodes.h"
#include "Vortex/Core/MouseButtonCodes.h"
#include "Vortex/Renderer/OrthographicCameraController.h"

// --- Renderer -------------------------
#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/RenderCommand.h"

#include "Vortex/Renderer/Buffer.h"
#include "Vortex/Renderer/Shader.h"
#include "Vortex/Renderer/Texture.h"
#include "Vortex/Renderer/VertexArray.h"

#include "Vortex/Renderer/OrthographicCamera.h"
// --------------------------------------

/*
tips:
this head file should put in the last, 
it will reference the content of above head files.
*/
// --- Entry Point ----------------------
#include"Vortex/Core/EntryPoint.h"
// --------------------------------------
