#pragma once

#include "Core.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		 virtual ~Application();


		void Run();

		//To be define in Client           ��������Application.cpp��ʵ�֣������ڰ������ͷ�ļ���������Ӱ������Ŀͻ��˴����н���ʵ��
		




	};

	Application* CreateApplication();  //����������������ռ������ģ������������������ģ���˵��ô��ô�౨����˵�����ռ�û�����������ԭ��������Ĳ��㰡
	//Ҳ�ǣ�������ĺ�����������Ǿ�̬�Ļ�����Ҫ�ٸ��һ�����������

}