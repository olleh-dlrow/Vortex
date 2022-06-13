#include "vtpch.h"
#include "Scene.h"

Vortex::Scene::Scene()
	:m_ViewportWindow(nullptr)
{
}

Vortex::Scene::Scene(ViewportWindow* vWin)
	:m_ViewportWindow(vWin)
{
}

void Vortex::Scene::Init()
{

}

void Vortex::Scene::OnUpdate(Timestep ts)
{
	for (auto& e : m_Entities)
	{
		e->OnUpdate(ts);
	}
}

void Vortex::Scene::AfterUpdate(Timestep ts)
{
	for (auto& e : m_Entities)
	{
		e->AfterUpdate(ts);
	}
}

Vortex::Entity* Vortex::Scene::AddEntity(const std::string& name)
{
	Ref<Entity> e = CreateRef<Entity>(name);
	e->SetScene(this);
	e->Init();

	m_Entities.emplace_back(e);
	return e.get();
}

Vortex::Entity* Vortex::Scene::FindFirstEntityByName(const std::string& name)
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
