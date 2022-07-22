#pragma once

#include "Vortex/Core/Core.h"

#include <vector>

namespace Vortex {

	class Layer;

	/*
		LayerStack has two parts: Overlay on the top and Layer underneath it,
		all of the layers in Overlay part are higher than layers in Layer part
	*/
	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		// The bottom of the LayerStack
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		// The top of the LayerStack
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};

}
