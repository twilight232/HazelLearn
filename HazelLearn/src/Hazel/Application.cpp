#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"
#include "../Platform/Windows/WindowsInput.h"


#include "../../Glad/include/glad/glad.h"

#include "glm.hpp"
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


		//��Ⱦ��ʼ
		m_VertexArray.reset(VertexArray::Create());//����һ��VAO��

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //����һ��VBO���������Ѿ����뻺����

		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },  //��������ǵ�����BufferElement�Ĺ��캯�������ִ��ξ���Ĭ�ϵ��ù��캯��
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);  //����layout���Ժ�Ͱ���layout��ȡ����������
		m_VertexArray->AddVertexBuffer(vertexBuffer); //VAO��Ӷ�VBO������

		
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))); //����EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);  //VAO��Ӷ�EBO������



		m_SquareVA.reset(VertexArray::Create()); //����һ��VAO
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));  //����һ��VBO
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB); //VAO��Ӷ�VBO�����ã���ʱVAO�ỻ�󣬻���m_SquareVA���VAO


		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); 
		m_SquareVA->SetIndexBuffer(squareIB); //VAO��Ӷ�EBO������



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


		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

		

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

			m_BlueShader->Bind();
			m_SquareVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			//��ѯ��ͬ�㷢�����¼�
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //Ŀǰֻ��IO��ѯ



			//��ȾImGui
			/*m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();  
			m_ImGuiLayer->End();*/

			
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