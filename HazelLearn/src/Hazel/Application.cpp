#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"
#include "../Platform/Windows/WindowsInput.h"


#include "../../Glad/include/glad/glad.h"

#include "glm/glm.hpp"
#include "../vendor/imgui/imgui.h"


namespace Hazel {

	//绑定事件
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x是Application的成员函数，this是当前对象的指针，第三个是指成员函数传入参数的第一个
	//感觉bind这个，说是绑定，其实是又加了层封装，用处确实有，可以调整函数的接口，控制传入参数数量和使用，让函数接口匹配

	

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //如果s_Instance不为空指针，就终止程序
		s_Instance = this;  //这是个静态成员，让它指向当前对象  意思是每新建立一个Application对象，都会让s_Instance指向当前对象，配合HZ_CORE_ASSERT，确保整个程序中都只会有一个Application实例

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)本身是一个函数对象

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		

	}
	Application::~Application()
	{

	}
	//这个是为了过dll生成这一关，其实这个函数是不会被使用的，使用的依旧是SandboxApp里实现的CreateApplication() 
	Hazel::Application* Hazel::CreateApplication()          //没有中间那个Application时这里一直报错，说Hazel命名空间下没有CreateApplication()
	{
		return new Application();             
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //目前只有IO轮询




			m_ImGuiLayer->Begin();
			

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();  

			m_ImGuiLayer->End();

			
			m_Window->OnUpdate();
		}

		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));    //当传入事件e==WindowCloseEvent时，dispatcher调用BIND_EVENT_FN(OnWindowClose)
		//BIND_EVENT_FN(OnWindowClose)是一个函数对象，这个函数对象

		//从后往前走的迭代器   发生事件后，调用每一个层的OnEvent函数
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);  //先--，再*
			if (e.Handled)     //如果已经有一个事件被处理了，那就break，，这是因为默认事件从前往后处理吗？  当检测到已经处理的事件后，就默认前面的事件都处理完了
				break;
		}
	}

	//传入一个事件，返回一个bool值，倒是匹配
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}