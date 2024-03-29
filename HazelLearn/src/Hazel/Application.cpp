#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"
#include "../Platform/Windows/WindowsInput.h"


#include "../../Glad/include/glad/glad.h"

#include "glm.hpp"
#include "../vendor/imgui/imgui.h"





namespace Hazel {

	//绑定事件
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x是Application的成员函数，this是当前对象的指针，第三个是指成员函数传入参数的第一个
	//感觉bind这个，说是绑定，其实是又加了层封装，用处确实有，可以调整函数的接口，控制传入参数数量和使用，让函数接口匹配

	

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //如果s_Instance不为空指针，就终止程序
		s_Instance = this;  //这是个静态成员，让它指向当前对象  意思是每新建立一个Application对象，都会让s_Instance指向当前对象，配合HZ_CORE_ASSERT，确保整个程序中都只会有一个Application实例

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)本身是一个函数对象

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		//渲染开始
		m_VertexArray.reset(VertexArray::Create());//生成一个VAO，

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //生成一个VBO对象，数据已经传入缓冲区

		
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },  //单提出来是调用了BufferElement的构造函数，这种传参就算默认调用构造函数
			{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);  //设置layout，以后就按照layout读取缓冲区数据
		m_VertexArray->AddVertexBuffer(vertexBuffer); //VAO添加对VBO的引用

		
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))); //创建EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);  //VAO添加对EBO的引用



		m_SquareVA.reset(VertexArray::Create()); //生成一个VAO
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));  //生成一个VBO
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB); //VAO添加对VBO的引用，此时VAO会换绑，换成m_SquareVA这个VAO


		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t))); 
		m_SquareVA->SetIndexBuffer(squareIB); //VAO添加对EBO的引用



		//人工制作Shader，本来应该是读取Shader文件的，先凑合吧
		//location=0，对应第一个顶点数组
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
	//这个是为了过dll生成这一关，其实这个函数是不会被使用的，使用的依旧是SandboxApp里实现的CreateApplication() 
	/*Hazel::Application* Hazel::CreateApplication()          //没有中间那个Application时这里一直报错，说Hazel命名空间下没有CreateApplication()
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

			//轮询不同层发生的事件
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //目前只有IO轮询



			//渲染ImGui
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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));    //当传入事件e==WindowCloseEvent时，dispatcher调用BIND_EVENT_FN(OnWindowClose)
		//BIND_EVENT_FN(OnWindowClose)是一个函数对象，这个函数对象

		//从后往前走的迭代器   发生事件后，调用每一个层的OnEvent函数
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);  //先--，再*
			if (e.Handled)     //如果已经有一个事件被处理了，那就break，，这是因为默认事件从前往后处理吗？  当检测到已经处理的事件后，就默认前面的事件都处理完了
				break;
		}
	}

	//传入一个事件，返回一个bool值，倒是匹配
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}