#pragma once

#include "Hazel/Window.h"

#include "../vendor/GLFW/glfw3.h"


namespace Hazel {

	//Windows����ϵͳ������
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
		//����
		void OnUpdate() override;
		//���
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// ���ûص��¼�
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }  //�����ﶨ���˺�����û�н���cpp�ļ�ȥ����
		//���ô�ֱͬ��
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_Window; }  

		//û�и�дCreate�������Ǿ������û����


	private:
		//�������������������Լ���
		//��ʼ���͹رմ���
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();




	private:

		GLFWwindow* m_Window;  //���ָ��ָ��һ��GLFW���ڶ���
		//���ڻ�������
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;  //�Ƿ�ֱͬ��

			EventCallbackFn EventCallback;  //�ص�������ʹ��SetEventCallback����ע��
		};

		WindowData m_Data;
	};

}