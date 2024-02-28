#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

	//��Щ�����������������ת��λ��֮��Ĳ������ж�  �����ת������Ҽ������������������֣����벻ͬ�Ĳ���ģʽ
	Input* Input::s_Instance = new WindowsInput();
	
	//�����Ƿ���
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);  //�������ֵ����������SandboxApp��OnUpdate��������  ��ʾ������Ƿ񱻰��·���Press��Release
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	//����Ƿ���
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	//���λ��
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
	//���xλ��
	float WindowsInput::GetMouseXImpl()
	{
		std::pair<float, float> position = GetMousePositionImpl();
		return position.first;
	}
	//���yλ��
	float WindowsInput::GetMouseYImpl()
	{
		std::pair<float, float> position = GetMousePositionImpl();
		return position.second;
	}

}