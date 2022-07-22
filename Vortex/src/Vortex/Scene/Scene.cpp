#include "vtpch.h"
#include "Scene.h"
#include "Vortex/ImGui/ViewportWindow.h"
#include "Vortex/Renderer/Camera.h"

namespace Vortex
{
	Scene::Scene()
		:m_ViewportWindow(nullptr)
	{
	}

	Scene::Scene(ViewportWindow* vWin)
		:m_ViewportWindow(vWin)
	{
	}

	void Scene::Init()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{
		for (auto& e : m_Entities)
		{
			e->OnUpdate(ts);
		}
	}

	void Scene::PostUpdate(Timestep ts)
	{
		for (auto& e : m_Entities)
		{
			e->PostUpdate(ts);
		}
	}

	Entity* Scene::AddEntity(const std::string& name)
	{
		Ref<Entity> e = CreateRef<Entity>(name);
		e->SetScene(this);
		e->Init();

		m_Entities.emplace_back(e);
		return e.get();
	}

	Entity* Scene::FindFirstEntityByName(const std::string& name)
	{
		for (auto& e : m_Entities)
		{
			if (e->GetName() == name)
			{
				return e.get();
			}
		}
		return nullptr;
	}

	Camera* Scene::GetInnerCamera()
	{
		return m_ViewportWindow->GetCamera();
	}
}

