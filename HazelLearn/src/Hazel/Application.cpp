#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"

#include <GLFW/glfw3.h>



namespace Hazel {

	//���¼�
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x��Application�ĳ�Ա������this�ǵ�ǰ�����ָ�룬��������ָ��Ա������������ĵ�һ��
	//�о�bind�����˵�ǰ󶨣���ʵ���ּ��˲��װ���ô�ȷʵ�У����Ե��������Ľӿڣ����ƴ������������ʹ�ã��ú����ӿ�ƥ��

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)������һ����������

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
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}

		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));    //�������¼�e==WindowCloseEventʱ��dispatcher����BIND_EVENT_FN(OnWindowClose)
		//BIND_EVENT_FN(OnWindowClose)��һ���������������������

		//�Ӻ���ǰ�ߵĵ�����
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