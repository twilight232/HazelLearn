#include "hzpch.h"
#include "ImGuiLayer.h"

#include "../../HazelLearn/vendor/imgui/imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Hazel/Application.h"

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}
	

	//添加一个ImGui层
	void ImGuiLayer::OnAttach() 
	{
		ImGui::CreateContext();  //创建一个 ImGui 的上下文对象，它是管理 ImGui 库状态和数据的核心对象。在你开始使用 ImGui 创建 UI 元素之前，需要先创建一个上下文对象。一旦上下文对象被创建，你就可以开始添加窗口、按钮、文本框等 UI 元素，并与其交互。
		ImGui::StyleColorsDark(); //用于设置 ImGui 的界面风格为暗色主题。

		ImGuiIO& io = ImGui::GetIO();  //这结构体还挺大
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; //启用鼠标光标功能
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  //启用鼠标位置功能    //也是位运算，有意思

		// TEMPORARY: should eventually use Hazel key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	

	void ImGuiLayer::OnDetach()
	{
	
	}
	
	//每一帧对Imgui进行更新吗？  每一帧？
	void ImGuiLayer::OnUpdate()
	{
		//IO部分
		ImGuiIO& io = ImGui::GetIO();  
		Application& app = Application::Get();  //指向当前唯一对象的指针
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());


		//时间部分
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);   //第一帧DeltaTime是1/60.0,第二帧DeltaTime就浮动了，是 (time - 上一帧的time)
		m_Time = time;


		//渲染部分

		/*通常情况下，你在渲染循环的开头调用这两个函数，
		然后在渲染循环的中间添加你的 ImGui UI 元素，最后在渲染循环的结尾调用 ImGui::Render() 来完成 UI 的渲染。*/
		ImGui_ImplOpenGL3_NewFrame(); //这个函数通常用于在渲染循环开始时调用，用来初始化 ImGui 在当前图形上下文中的一些状态
		ImGui::NewFrame(); //这是 ImGui 库的一个主要函数，用于开始新的帧（frame）。在调用这个函数之后，你可以在当前帧中添加 ImGui 的 UI 元素，如按钮、文本框等。

		static bool show = true;
		ImGui::ShowDemoWindow(&show);  //这个就是展示出来的那个imgui，是一个Demo，不过ImGui::Button()这样的Imgui事件函数我们还没有使用，所以没有交互
		//在ImGui::Render();之前，我们要设计自己的GUI，或者借助Demo也行，这里是弄出各种UI元素的

		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  //获取绘制的数据，再实现
	}

	//用当点击事件，鼠标事件出现时   这里会被Application::OnEvent调用，即当GLFW发生事件时，event就是GLFW给的事件，似乎只有那些在GLFW注册的函数会过来
	void ImGuiLayer::OnEvent(Event& event)
	{

	}

}