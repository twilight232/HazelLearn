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

Hazel::Application* Hazel::CreateApplication()          //û���м��Ǹ�Applicationʱ����һֱ����˵Hazel�����ռ���û��CreateApplication()
{
	return new Sandbox();
}
