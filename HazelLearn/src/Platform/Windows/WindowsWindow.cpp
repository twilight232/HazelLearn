#include "hzpch.h"
#include "WindowsWindow.h"
#include "Hazel.h"

namespace Hazel {

	static bool s_GLFWInitialized = false;  

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

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

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);  //����һ��glfw3�������������ʵ�֣��������
		glfwMakeContextCurrent(m_Window);  //����ǰOpenGL����������λָ�����ڵ������ģ���OpenGL��������Ⱦ�����ڸô���������ִ��
		glfwSetWindowUserPointer(m_Window, &m_Data);  //���û��Զ���ָ���봰�ڹ���������ָ��������������͵�ָ�룬�Ա��ں������¼������п������ɷ�����Щ����
		//ͨ������£����ָ��ָ��һ���ṹ��������ʵ���������˴�����ص�һЩ״̬�������ݡ�
		SetVSync(true);
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