#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"
#include "../Platform/Windows/WindowsInput.h"


#include "../../Glad/include/glad/glad.h"

#include "glm/glm.hpp"
#include "../vendor/imgui/imgui.h"


namespace Hazel {

	//���¼�
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x��Application�ĳ�Ա������this�ǵ�ǰ�����ָ�룬��������ָ��Ա������������ĵ�һ��
	//�о�bind�����˵�ǰ󶨣���ʵ���ּ��˲��װ���ô�ȷʵ�У����Ե��������Ľӿڣ����ƴ������������ʹ�ã��ú����ӿ�ƥ��

	

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //���s_Instance��Ϊ��ָ�룬����ֹ����
		s_Instance = this;  //���Ǹ���̬��Ա������ָ��ǰ����  ��˼��ÿ�½���һ��Application���󣬶�����s_Instanceָ��ǰ�������HZ_CORE_ASSERT��ȷ�����������ж�ֻ����һ��Applicationʵ��

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)������һ����������

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		

	}
	Application::~Application()
	{

	}
	//�����Ϊ�˹�dll������һ�أ���ʵ��������ǲ��ᱻʹ�õģ�ʹ�õ�������SandboxApp��ʵ�ֵ�CreateApplication() 
	Hazel::Application* Hazel::CreateApplication()          //û���м��Ǹ�Applicationʱ����һֱ����˵Hazel�����ռ���û��CreateApplication()
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
				layer->OnUpdate();  //Ŀǰֻ��IO��ѯ




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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));    //�������¼�e==WindowCloseEventʱ��dispatcher����BIND_EVENT_FN(OnWindowClose)
		//BIND_EVENT_FN(OnWindowClose)��һ���������������������

		//�Ӻ���ǰ�ߵĵ�����   �����¼��󣬵���ÿһ�����OnEvent����
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);  //��--����*
			if (e.Handled)     //����Ѿ���һ���¼��������ˣ��Ǿ�break����������ΪĬ���¼���ǰ��������  ����⵽�Ѿ�������¼��󣬾�Ĭ��ǰ����¼�����������
				break;
		}
	}

	//����һ���¼�������һ��boolֵ������ƥ��
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}