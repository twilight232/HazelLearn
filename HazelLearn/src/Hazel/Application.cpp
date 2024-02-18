#include "hzpch.h"
#include "Application.h"
#include "../../Events/EventAPI.h"
#include "Log.h"



namespace Hazel {

	Application::Application()
	{

	}
	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		//std::cout << e.ToString();
		//HZ_TRACE(e.ToString());   //因为这里是个自定义事件，所以，你日志系统无法追踪是吗,,,要转换成字符串，日志系统才能跟踪     
		//改个代码还要实时更换dll，，，emmmm

		if (e.IsInCategory(EventCategoryKeyboard))
		{
			HZ_TRACE(e.ToString());
			HZ_TRACE("Keyboard");
		}
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE(e.ToString());
			HZ_TRACE("Application");
		}



		while (true)
		{
			;
		}

		
	}

}