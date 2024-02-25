#pragma once

#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/EventAPI.h"

#include "Window.h"
#include"Hazel/LayerStack.h"


namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		 virtual ~Application();

		void Run();
		
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);


	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	



	};

	 Application* CreateApplication();  //����������������ռ������ģ������������������ģ���˵��ô��ô�౨����˵�����ռ�û�����������ԭ��������Ĳ��㰡
	//Ҳ�ǣ�������ĺ�����������Ǿ�̬�Ļ�����Ҫ�ٸ��һ�����������

}