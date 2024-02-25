#pragma once
#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);   //overlay（叠加层）是什么意思呢
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;   //一个Layer指针组成的数组
		std::vector<Layer*>::iterator m_LayerInsert;   //看上去是指向当前的第一个空位的       感觉是layer之间进行分组的，不同的layer分别组成不同的组，然后m_LayerInsert是处理分组的
	};

}