#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		
		PushLayer(new Sandbox2D()); //在这里选择启用一个SandBox2D
	}

	~Sandbox()
	{
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
