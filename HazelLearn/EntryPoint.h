#pragma once
#include "Log.h"


#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();   //声明一个外部的函数，看上去像一个虚函数，准备调用模板类来应付那些奇奇怪怪的调用引擎的程序

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


