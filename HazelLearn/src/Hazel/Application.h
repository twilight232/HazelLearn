#pragma once

#include "Core.h"
#include "Hazel/Events/Event.h"

#include "Window.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		 virtual ~Application();


		void Run();

		//To be define in Client           ��������Application.cpp��ʵ�֣������ڰ������ͷ�ļ���������Ӱ������Ŀͻ��˴����н���ʵ��
		

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	



	};

	 Application* CreateApplication();  //����������������ռ������ģ������������������ģ���˵��ô��ô�౨����˵�����ռ�û�����������ԭ��������Ĳ��㰡
	//Ҳ�ǣ�������ĺ�����������Ǿ�̬�Ļ�����Ҫ�ٸ��һ�����������

}