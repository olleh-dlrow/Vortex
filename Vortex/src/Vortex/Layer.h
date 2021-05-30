#pragma once

#include "Vortex/Core.h"
#include "Vortex/Core/Timestep.h"
#include "Vortex/Events/Event.h"

namespace Vortex {

	class VORTEX_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}    //tips: set the env of layer
		virtual void OnDetach() {}    //tips: cancel the env of layer
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}
