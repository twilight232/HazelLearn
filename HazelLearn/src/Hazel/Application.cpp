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
		//HZ_TRACE(e.ToString());   //��Ϊ�����Ǹ��Զ����¼������ԣ�����־ϵͳ�޷�׷������,,,Ҫת�����ַ�������־ϵͳ���ܸ���     
		//�ĸ����뻹Ҫʵʱ����dll������emmmm

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