#pragma once

#include <vector>
#include <string>
#include <map>
#include "Entity.h"
#include "Vortex/Core/Core.h"
#include "Vortex/Core/Timestep.h"

namespace Vortex
{
	class ViewportWindow;
	class Camera;

	class Scene
	{
	public:
		Scene();
		Scene(ViewportWindow* vWin);

		void Init();

		void OnUpdate(Timestep ts);

		void PostUpdate(Timestep ts);

		Entity* AddEntity(const std::string& name);

		Entity* FindFirstEntityByName(const std::string& name);

		Camera* GetInnerCamera();
	private:
		// TODO: use multimap to implement the execution order
		std::vector< Ref<Entity> >		m_Entities;
		ViewportWindow*					m_ViewportWindow;
	};
}
