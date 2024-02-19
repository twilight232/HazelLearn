#pragma once

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	//Windows操作系统窗口类
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		//没有覆写Create函数，那就是沿用基类的
	private:
		//这两个函数是派生类自己的
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:

		GLFWwindow* m_Window;  //这个指针指向一个GLFW窗口对象

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;  //是否垂直同步

			EventCallbackFn EventCallback;  //捕获事件，做出响应
		};

		WindowData m_Data;
	};

}