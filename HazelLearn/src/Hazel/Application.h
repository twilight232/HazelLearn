#pragma once

#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/EventAPI.h"

#include "Window.h"
#include"Hazel/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"
#include "../../HazelLearn/src/Hazel/Render/Shader.h"
#include "../../HazelLearn/src/Hazel/Render/Buffer.h"


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
		unsigned int m_VertexArray;//VAO


		std::unique_ptr<Shader> m_Shader;  
		std::unique_ptr<VertexBuffer> m_VertexBuffer; //VBO
		std::unique_ptr<IndexBuffer> m_IndexBuffer;  //EBO


	private:
		static Application* s_Instance;  //静态实例，确保始终只有一个实例对象
	



	};

	 Application* CreateApplication();  //这个函数是在命名空间声明的，不是在类里面声明的，我说怎么那么多报错，还说命名空间没有这个函数。原来类里面的不算啊
	//也是，类里面的函数，如果不是静态的话，还要再搞出一个对象才能用
	 //这种算全局函数

}