#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

	//这些函数是用来做相机旋转，位移之类的操作的判定  相机旋转按鼠标右键，缩放是鼠标滚轮这种，进入不同的操作模式
	Input* Input::s_Instance = new WindowsInput();
	
	//键盘是否按下
	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);  //这个键盘值最终是用在SandboxApp的OnUpdate函数里面  表示这个键是否被按下返回Press或Release
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	//鼠标是否按下
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}
	//鼠标位置
	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
	//鼠标x位置
	float WindowsInput::GetMouseXImpl()
	{
		std::pair<float, float> position = GetMousePositionImpl();
		return position.first;
	}
	//鼠标y位置
	float WindowsInput::GetMouseYImpl()
	{
		std::pair<float, float> position = GetMousePositionImpl();
		return position.second;
	}

}