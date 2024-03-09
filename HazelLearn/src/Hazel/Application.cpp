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

		glGenBuffers(1, &m_VertexBuffer);   //VBO   顶点缓冲对象   但这里只是生成一个缓冲区
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);//此时选择VBO代表的缓冲区作为GL_ARRAY_BUFFER，顶点缓冲区     这里指明是顶点缓冲区，VBo才算生成完毕

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //将内存的数据传输到显存中，显存中的缓冲中，这里是进入顶点缓冲区

		glEnableVertexAttribArray(0); //启用索引为0的顶点数组对象，其实就是第一个顶点数组对象。目前就一个顶点数组对象
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); // 设置顶点属性指针，这个数据是这么读的。 
		//第一个0和顶点数组对应，它代表我们读哪一个顶点数组，这里是让我们读第一个顶点数组
		//第二个参数指明一个属性是由几个变量组成，这里是vec3
		//第三个参数指明是float，那就是vec3f
		//第四个参数是表示是否标准化，如果设置GL_TURE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
		//第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。**由于下个组位置数据在3个`float`之后**，我们把步长设置为`3 * sizeof(float)`。    并不是所有数据都是紧密排列的
		// 第六个参数，它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是nullptr。
		


		glGenBuffers(1, &m_IndexBuffer);  
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer); //EBO

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		

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
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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