#pragma once

#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/EventAPI.h"

#include "Window.h"
#include"Hazel/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "../../HazelLearn/src/Hazel/Render/Shader.h"
#include "../../HazelLearn/src/Hazel/Render/Buffer.h"
#include "../../HazelLearn/src/Hazel/Render/VertexArray.h"


namespace Hazel {

	class HAZEL_API Application
	{
	public:
		

		Application();
		 virtual ~Application();

		void Run();
		
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;



		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;//VAO

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;//VAO

	private:
		static Application* s_Instance;  //��̬ʵ����ȷ��ʼ��ֻ��һ��ʵ������
	



	};

	 Application* CreateApplication();  //����������������ռ������ģ������������������ģ���˵��ô��ô�౨����˵�����ռ�û�����������ԭ��������Ĳ��㰡
	//Ҳ�ǣ�������ĺ�����������Ǿ�̬�Ļ�����Ҫ�ٸ��һ�����������
	 //������ȫ�ֺ���

}