#pragma once
#include "Log.h"
#include "Application.h"



#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();   

int main(int argc,char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	
	

	
	auto app = Hazel::CreateApplication();  //调用的是Hazel空间下的CreateApplication函数
	app->Run();
	delete app;
	return 0;
}

#endif


