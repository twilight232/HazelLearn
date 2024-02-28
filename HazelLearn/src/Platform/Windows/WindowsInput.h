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
	//�̳���s_Instance�Ǹ���̬ʵ�����������WindowsInput::s_Instance���ǵ���Input::s_Instance  һ������
	//��˵û���Զ��幹�캯�����ü�⵽�µ�ʵ�����ɾ���ֹ���о�Ӧ����ô����

}