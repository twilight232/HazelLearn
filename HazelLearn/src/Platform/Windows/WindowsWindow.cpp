#include "hzpch.h"
#include "WindowsWindow.h"
#include "Hazel.h"


namespace Hazel {

	static bool s_GLFWInitialized = false;  

	static void GLFWErrorCallback(int error,const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}):{1}", error, description);
	}

	//��������
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	//���캯��
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	//��������
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	//���ڳ�ʼ��
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//���GLFWδ��ʼ������ִ��if�ڲ����
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");  //�жϳ�ʼ���Ƿ�ɹ��������ʼ��δ�ɹ���ֱ�ӱ�����ֹ����
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);  //����һ��glfw3�������������ʵ�֣��������
		glfwMakeContextCurrent(m_Window);  //����ǰOpenGL����������λָ�����ڵ������ģ���OpenGL��������Ⱦ�����ڸô���������ִ��
		glfwSetWindowUserPointer(m_Window, &m_Data);  //��&m_Data��Ϊm_Windowָ�򴰿ڵ��û�ָ�룬����ͨ��glfwGetWindowUserPointer����ô����û�ָ��&m_Data
		SetVSync(true);

		//ע��GLFW�Ļص�����������lambda���ʽ���ֳ�������ص�����
		// �����ǵ�����GLFW��glfwSetWindowSizeCallback������Ȼ��lambda���ʽ��Ϊ����һ������
		//���ڴ�С�ı��¼�
		
		//EventCallbackֵ��OnEvent������������Application�е�Init�����и�ֵ��
		//��Ϊע�ᣬʵ���Ͼ��ǵ���glfwSetWindowSizeCallback����������m_Window��һ��lambda���ʽ������glfwSetWindowSizeCallback����ʵ����glfw3.lib�
		//Ȼ��glfw�Ǳ��������㴫����Ǹ�lambda���ʽ��Ϊ��������һ��������Ӧ�¼����͵������lambda��ʾʽ����
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			//width��height�������ȵ����ݡ� ���漸��ֻ�ǽ����ݸ��£�������֪��
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);  
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		//���ڹر��¼�
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);   //����ʹ��static_cast �Ƿ�����һ�㣿
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		//�����¼�
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
		});
		//��갴���¼�
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
		});
		//�������¼�
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
		//����ƶ��¼�
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);  //��ȡ��m_Data  

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);  //��������ƶ��¼���m_Data��EventCallback
		});
	}




	


	//�رմ���
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window); //�ݻٴ���
	}

	//���´���
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();  //�����ں������¼�����������Ӧ�ص�����
		glfwSwapBuffers(m_Window);  //����ǰ�󻺳���������ͼ��
	}

	//���ô�ֱͬ��
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);  //������ֱͬ�����ȴ���ֱ�հ׼�����ٽ���֡���彻��
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	//��ֱͬ��
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}