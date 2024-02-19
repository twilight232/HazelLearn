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

	//窗口初始化
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//如果GLFW未初始化，则执行if内部语句
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");  //判断初始化是否成功，如果初始化未成功，直接报错，终止程序

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);  //看了一下glfw3里面这个函数的实现，合情合理啊
		glfwMakeContextCurrent(m_Window);  //将当前OpenGL上下文设置位指定窗口的上下文，让OpenGL后续的渲染操作在该窗口上下文执行
		glfwSetWindowUserPointer(m_Window, &m_Data);  //将用户自定义指针与窗口关联起来，指针可以是任意类型的指针，以便在后续的事件处理中可以轻松访问这些数据
		//通常情况下，这个指针指向一个结构体或者类的实例，包含了窗口相关的一些状态或者数据。
		SetVSync(true);
	}

	//关闭窗口
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window); //摧毁窗口
	}

	//更新窗口
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();  //处理窗口和输入事件，并触发对应回调函数
		glfwSwapBuffers(m_Window);  //交换前后缓冲区，更新图像
	}

	//设置垂直同步
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);  //启动垂直同步，等待垂直空白间隔后再进行帧缓冲交换
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	//垂直同步
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}