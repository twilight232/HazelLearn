#pragma once


#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();   //����һ���ⲿ�ĺ���������ȥ��һ���麯����׼������ģ������Ӧ����Щ����ֵֹĵ�������ĳ���

int main(int argc,char** argv)
{
	
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif


