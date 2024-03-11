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

	//һ��Hazel�ռ��µĺ���
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Float:    return GL_FLOAT;
		case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
		case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
		case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
		case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
		case Hazel::ShaderDataType::Int:      return GL_INT;
		case Hazel::ShaderDataType::Int2:     return GL_INT;
		case Hazel::ShaderDataType::Int3:     return GL_INT;
		case Hazel::ShaderDataType::Int4:     return GL_INT;
		case Hazel::ShaderDataType::Bool:     return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //���s_Instance��Ϊ��ָ�룬����ֹ����
		s_Instance = this;  //���Ǹ���̬��Ա������ָ��ǰ����  ��˼��ÿ�½���һ��Application���󣬶�����s_Instanceָ��ǰ�������HZ_CORE_ASSERT��ȷ�����������ж�ֻ����һ��Applicationʵ��

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)������һ����������

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		
		glGenVertexArrays(1, &m_VertexArray); //VAO  �����������
		glBindVertexArray(m_VertexArray);

		

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //����һ��VBO�������Ѿ����뻺����

		{ //����������ǵ���Ū��һ�������� Ϊ�������layout���������
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },  //��������ǵ�����BufferElement�Ĺ��캯�������ִ��ξ���Ĭ�ϵ��ù��캯��
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);  //����layout���Ժ�Ͱ���layout��ȡ����������

		}



		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout(); //������������е���˼��
		//GetLayout���ص���OpenGLVertexBuffer�����m_Layout�����õĻ��������������layoutֱ�Ӳ������������m_Layout����������ȫ���ͼ��˸�const
		//��ʵ���������ǿ��Եģ���������Ҫ����һ�������ˣ��������ܴ�Ļ����ͻ�Ӱ���ڴ��������
		for (const auto& element : layout.GetElements())   //֮ǰ��ֱ�ӱ���layout����ʵҲ����Ĭ��������m_Elements,,���ҵ�m_Elements������ΪPrivate�ģ�forѭ����ֱ�ӱ����� ��Ϊ�Ҷ�����һ�����Է��ʵ�m_Elements��Public������forѭ�����Լ�ȥ��������v
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,       
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;//layout��std::vector<BufferElement> m_Elements;  ����element�Ǹ�����ͬ�ģ���BufferElement�������ж������͵�,ȷʵ�Ƕ�Ӧ��Shader�е�layout���
		}
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

			//��ѯ��ͬ�㷢�����¼�
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //Ŀǰֻ��IO��ѯ



			//��ȾImGui
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