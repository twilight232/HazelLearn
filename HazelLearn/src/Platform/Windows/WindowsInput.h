#pragma once

#include "../../Hazel/Input.h"

namespace Hazel {

	class WindowsInput : public Input
	{


	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
	//继承了s_Instance那个静态实例，这里调用WindowsInput::s_Instance就是调用Input::s_Instance  一个东西
	//话说没有自定义构造函数，让检测到新的实例生成就终止，感觉应该这么做的

}