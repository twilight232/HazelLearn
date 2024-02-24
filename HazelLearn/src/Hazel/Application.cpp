#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"

#include <GLFW/glfw3.h>



namespace Hazel {

	//绑定事件
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x是Application的成员函数，this是当前对象的指针，第三个是指成员函数传入参数的第一个
	//感觉bind这个，说是绑定，其实是又加了层封装，用处确实有，可以调整函数的接口，控制传入参数数量和使用，让函数接口匹配

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)本身是一个函数对象

	}
	Application::~Application()
	{

	}

	Hazel::Application* Hazel::CreateApplication()          //没有中间那个Application时这里一直报错，说Hazel命名空间下没有CreateApplication()
	{
		return new Application();
	}

	

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}

		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));    //当传入事件e==WindowCloseEvent时，dispatcher调用BIND_EVENT_FN(OnWindowClose)
		//BIND_EVENT_FN(OnWindowClose)是一个函数对象，这个函数对象
		HZ_CORE_TRACE("{0}", e);
	}

	//传入一个事件，返回一个bool值，倒是匹配
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}