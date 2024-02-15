#pragma once
#include "Log.h"


#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();   //����һ���ⲿ�ĺ���������ȥ��һ���麯����׼������ģ������Ӧ����Щ����ֵֹĵ�������ĳ���

int main(int argc,char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Lod!");
	int a = 9;
	HZ_INFO("Hello! Var={0}", a);


	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif


