#include "hzpch.h"
#include "WindowsWindow.h"
#include "Hazel.h"


namespace Hazel {

	static bool s_GLFWInitialized = false;  

	static void GLFWErrorCallback(int error,const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}):{1}", error, description);
	}

	//创建窗口
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	//构造函数
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	//析构函数
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
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);  //看了一下glfw3里面这个函数的实现，合情合理啊
		glfwMakeContextCurrent(m_Window);  //将当前OpenGL上下文设置位指定窗口的上下文，让OpenGL后续的渲染操作在该窗口上下文执行
		glfwSetWindowUserPointer(m_Window, &m_Data);  //让&m_Data成为m_Window指向窗口的用户指针，可以通过glfwGetWindowUserPointer来获得窗口用户指针&m_Data
		SetVSync(true);

		//注册GLFW的回调函数，利用lambda表达式，现场定义出回调函数
		// 这里是调用了GLFW的glfwSetWindowSizeCallback函数，然后lambda表达式作为其中一个参数
		//窗口大小改变事件
		
		//EventCallback值是OnEvent函数，这是在Application中的Init函数中赋值的
		//名为注册，实际上就是调用glfwSetWindowSizeCallback函数，传入m_Window和一个lambda表达式，具体glfwSetWindowSizeCallback函数实现在glfw3.lib里，
		//然后glfw那边掌握了你传入的那个lambda表达式作为函数对象，一旦发生相应事件，就调用这个lambda表示式对象
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			//width，height才是最先的数据。 下面几行只是将数据更新，让我们知道
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);  
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});
		//窗口关闭事件
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);   //这里使用static_cast 是否会更好一点？
			WindowCloseEvent event;
			data.EventCallback(event);
		});
		//键盘事件
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
		//鼠标按键事件
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
		//鼠标滚轮事件
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
		//鼠标移动事件
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);  //获取了m_Data  

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);  //传入鼠标移动事件到m_Data的EventCallback
		});
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