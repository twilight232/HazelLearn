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

	

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");  //如果s_Instance不为空指针，就终止程序
		s_Instance = this;  //这是个静态成员，让它指向当前对象  意思是每新建立一个Application对象，都会让s_Instance指向当前对象，配合HZ_CORE_ASSERT，确保整个程序中都只会有一个Application实例

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));   //BIND_EVENT_FN(OnEvent)本身是一个函数对象

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);


		//OpenGL绘制三角形，倒也没什么特殊的，蛮熟悉的
		glGenVertexArrays(1, &m_VertexArray); //VAO  顶点数组对象
		glBindVertexArray(m_VertexArray);

		

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));  //生成一个VBO




		glEnableVertexAttribArray(0); //启用索引为0的顶点数组对象，其实就是第一个顶点数组对象。目前就一个顶点数组对象
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); // 设置顶点属性指针，这个数据是这么读的。 
		//第一个0是和顶点着色器对应的，这里对应location=0
		//第二个参数指明一个属性是由几个变量组成，这里是vec3
		//第三个参数指明是float，那就是vec3f
		//第四个参数是表示是否标准化，如果设置GL_TURE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
		//第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。**由于下个组位置数据在3个`float`之后**，我们把步长设置为`3 * sizeof(float)`。    并不是所有数据都是紧密排列的
		// 第六个参数，它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是nullptr。
		


		

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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();  //目前只有IO轮询




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