#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"
#include "../Platform/Windows/WindowsInput.h"


#include "../../Glad/include/glad/glad.h"

#include "glm/glm.hpp"
#include "../vendor/imgui/imgui.h"




namespace Hazel {

	//���¼�
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x��Application�ĳ�Ա������this�ǵ�ǰ�����ָ�룬��������ָ��Ա������������ĵ�һ��
	//�о�bind�����˵�ǰ󶨣���ʵ���ּ��˲��װ���ô�ȷʵ�У����Ե��������Ľӿڣ����ƴ������������ʹ�ã��ú����ӿ�ƥ��

	

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //���s_Instance��Ϊ��ָ�룬����ֹ����
		s_Instance = this;  //���Ǹ���̬��Ա������ָ��ǰ����  ��˼��ÿ�½���һ��Application���󣬶�����s_Instanceָ��ǰ�������HZ_CORE_ASSERT��ȷ�����������ж�ֻ����һ��Applicationʵ��

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)������һ����������

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		//OpenGL���������Σ���Ҳûʲô����ģ�����Ϥ��
		glGenVertexArrays(1, &m_VertexArray); //VAO  �����������
		glBindVertexArray(m_VertexArray);

		

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //����һ��VBO




		glEnableVertexAttribArray(0); //��������Ϊ0�Ķ������������ʵ���ǵ�һ�������������Ŀǰ��һ�������������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); // ���ö�������ָ�룬�����������ô���ġ� 
		//��һ��0�ǺͶ�����ɫ����Ӧ�ģ������Ӧlocation=0
		//�ڶ�������ָ��һ���������ɼ���������ɣ�������vec3
		//����������ָ����float���Ǿ���vec3f
		//���ĸ������Ǳ�ʾ�Ƿ��׼�����������GL_TURE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣
		//�����������������(Stride)�������������������Ķ���������֮��ļ����**�����¸���λ��������3��`float`֮��**�����ǰѲ�������Ϊ`3 * sizeof(float)`��    �������������ݶ��ǽ������е�
		// ����������������ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)������λ������������Ŀ�ͷ������������nullptr��
		


		

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))); //����EBO
		//��OpenGL����һ���װ������Ŀǰ���ԣ���Ⱦ��ʵ�ֻ��ǻ���OpenGL��˼·�����ǲ����ģ�����������Ҫ������������ݣ��������OpenGL����ʲô��ȾAPIʵ��


		//�˹�����Shader������Ӧ���Ƕ�ȡShader�ļ��ģ��ȴպϰ�
		//location=0����Ӧ��һ����������
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		

	}
	Application::~Application()
	{

	}
	//�����Ϊ�˹�dll������һ�أ���ʵ��������ǲ��ᱻʹ�õģ�ʹ�õ�������SandboxApp��ʵ�ֵ�CreateApplication() 
	/*Hazel::Application* Hazel::CreateApplication()          //û���м��Ǹ�Applicationʱ����һֱ����˵Hazel�����ռ���û��CreateApplication()
	{
		return new Application();             
	}*/

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //Ŀǰֻ��IO��ѯ




			m_ImGuiLayer->Begin();
			

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();  

			m_ImGuiLayer->End();

			
			m_Window->OnUpdate();
		}

		
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));    //�������¼�e==WindowCloseEventʱ��dispatcher����BIND_EVENT_FN(OnWindowClose)
		//BIND_EVENT_FN(OnWindowClose)��һ���������������������

		//�Ӻ���ǰ�ߵĵ�����   �����¼��󣬵���ÿһ�����OnEvent����
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);  //��--����*
			if (e.Handled)     //����Ѿ���һ���¼��������ˣ��Ǿ�break����������ΪĬ���¼���ǰ��������  ����⵽�Ѿ�������¼��󣬾�Ĭ��ǰ����¼�����������
				break;
		}
	}

	//����һ���¼�������һ��boolֵ������ƥ��
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}