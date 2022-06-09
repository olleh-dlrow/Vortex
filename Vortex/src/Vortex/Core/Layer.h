#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/Core/Timestep.h"
#include "Vortex/Events/Event.h"

namespace Vortex {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}    //tips: set the env of layer
		virtual void OnDetach() {}    //tips: cancel the env of layer
		virtual void PreUpdate(Timestep ts) {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void AfterUpdate(Timestep ts) {}

		virtual void PreImGuiRender() {}
		virtual void OnImGuiRender() {}
		virtual void AfterImGuiRender() {}

		virtual void PreEvent(Event& event) {}
		virtual void OnEvent(Event& event) {}
		virtual void AfterEvent(Event& event) {}
		

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}
