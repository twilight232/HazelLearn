#pragma once

#include "Hazel/Window.h"

#include "../vendor/GLFW/glfw3.h"


namespace Hazel {

	//Windows操作系统窗口类
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		//更新
		void OnUpdate() override;
		//宽高
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// 设置回调事件
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }  //在这里定义了函数，没有交给cpp文件去做了
		//设置垂直同步
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }  

		//没有覆写Create函数，那就是沿用基类的


	private:
		//这两个函数是派生类自己的
		//初始化和关闭窗口
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();




	private:

		GLFWwindow* m_Window;  //这个指针指向一个GLFW窗口对象
		//窗口基本数据
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;  //是否垂直同步

			EventCallbackFn EventCallback;  //回调函数，使用SetEventCallback进行注册
		};

		WindowData m_Data;
	};

}