#pragma once

#include "Core.h"

namespace Hazel {

	class HAZEL_API Application
	{
	public:
		Application();
		 virtual ~Application();


		void Run();

		//To be define in Client           它不会在Application.cpp中实现，它会在包含这个头文件（包括间接包含）的客户端代码中进行实现
		




	};

	Application* CreateApplication();  //这个函数是在命名空间声明的，不是在类里面声明的，我说怎么那么多报错，还说命名空间没有这个函数。原来类里面的不算啊
	//也是，类里面的函数，如果不是静态的话，还要再搞出一个对象才能用

}