#include <Hazel.h>


class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{

	};
	~Sandbox()
	{

	};




private:

};

Hazel::Application* Hazel::CreateApplication()          //没有中间那个Application时这里一直报错，说Hazel命名空间下没有CreateApplication()
{
	return new Sandbox();
}
