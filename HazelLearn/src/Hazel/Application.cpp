#include "hzpch.h"
#include "Application.h"
#include "Hazel/Events/EventAPI.h"
#include "Log.h"
#include "../Platform/Windows/WindowsInput.h"


#include "../../Glad/include/glad/glad.h"

#include "glm/glm.hpp"
#include "../vendor/imgui/imgui.h"





namespace Hazel {

	//绑定事件
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)  //x是Application的成员函数，this是当前对象的指针，第三个是指成员函数传入参数的第一个
	//感觉bind这个，说是绑定，其实是又加了层封装，用处确实有，可以调整函数的接口，控制传入参数数量和使用，让函数接口匹配

	//一个Hazel空间下的函数
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
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //如果s_Instance不为空指针，就终止程序
		s_Instance = this;  //这是个静态成员，让它指向当前对象  意思是每新建立一个Application对象，都会让s_Instance指向当前对象，配合HZ_CORE_ASSERT，确保整个程序中都只会有一个Application实例

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)本身是一个函数对象

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		
		glGenVertexArrays(1, &m_VertexArray); //VAO  顶点数组对象
		glBindVertexArray(m_VertexArray);

		

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //生成一个VBO，数据已经传入缓冲区

		{ //这个花括号是单独弄出一个作用域， 为了让这个layout用完就销毁
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },  //单提出来是调用了BufferElement的构造函数，这种传参就算默认调用构造函数
				{ ShaderDataType::Float4, "a_Color" }
			};

			m_VertexBuffer->SetLayout(layout);  //设置layout，以后就按照layout读取缓冲区数据

		}



		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout(); //这个常量引用有点意思，
		//GetLayout返回的是OpenGLVertexBuffer对象的m_Layout，引用的话可以让外面这个layout直接操作对象里面的m_Layout，这样不安全，就加了个const
		//其实不用引用是可以的，但那样就要复制一个对象了，如果对象很大的话，就会影响内存和性能了
		for (const auto& element : layout.GetElements())   //之前是直接遍历layout，其实也就是默认区遍历m_Elements,,但我的m_Elements是设置为Private的，for循环能直接遍历？ 因为我定义了一个可以访问到m_Elements的Public函数，for循环会自己去调用它吧v
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,       
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;//layout是std::vector<BufferElement> m_Elements;  其中element是各不相同的，是BufferElement，可以有多种类型的,确实是对应了Shader中的layout序号
		}
		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))); //生成EBO
		//给OpenGL做了一层封装，但就目前而言，渲染的实现还是基于OpenGL的思路，这是不够的，我们最终是要做到上面给数据，下面随便OpenGL还是什么渲染API实现


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
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			//轮询不同层发生的事件
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //目前只有IO轮询



			//渲染ImGui
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