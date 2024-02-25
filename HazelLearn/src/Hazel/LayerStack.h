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
		void PushOverlay(Layer* overlay);   //overlay�����Ӳ㣩��ʲô��˼��
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;   //һ��Layerָ����ɵ�����
		std::vector<Layer*>::iterator m_LayerInsert;   //����ȥ��ָ��ǰ�ĵ�һ����λ��       �о���layer֮����з���ģ���ͬ��layer�ֱ���ɲ�ͬ���飬Ȼ��m_LayerInsert�Ǵ�������
	};

}