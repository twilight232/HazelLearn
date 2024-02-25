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

	 Application* CreateApplication();  //这个函数是在命名空间声明的，不是在类里面声明的，我说怎么那么多报错，还说命名空间没有这个函数。原来类里面的不算啊
	//也是，类里面的函数，如果不是静态的话，还要再搞出一个对象才能用

}