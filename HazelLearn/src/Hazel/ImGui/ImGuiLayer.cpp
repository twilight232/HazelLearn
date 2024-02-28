#include "hzpch.h"
#include "ImGuiLayer.h"

#include "../../HazelLearn/vendor/imgui/imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include "../../Glad/include/glad/glad.h"

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
	
	//删除一个层
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
		//渲染初始化
		ImGui_ImplOpenGL3_NewFrame(); //这个函数通常用于在渲染循环开始时调用，用来初始化 ImGui 在当前图形上下文中的一些状态
		ImGui::NewFrame(); //这是 ImGui 库的一个主要函数，用于开始新的帧（frame）。在调用这个函数之后，你可以在当前帧中添加 ImGui 的 UI 元素，如按钮、文本框等。

		//添加UI元素
		static bool show = true;
		ImGui::ShowDemoWindow(&show);  //这个就是展示出来的那个imgui，是一个Demo，不过ImGui::Button()这样的Imgui事件函数我们还没有使用，所以没有交互
		//在ImGui::Render();之前，我们要设计自己的GUI，或者借助Demo也行，这里是弄出各种UI元素的

		
		//开始渲染
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  //获取绘制的数据，再实现
	}

	//用当点击事件，鼠标事件出现时   这里会被Application::OnEvent调用，即当GLFW发生事件时，event就是GLFW给的事件，似乎只有那些在GLFW注册的函数会过来
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		//将GLFW那边传来的事件分发给imgui
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));


	}
	
	
	
	
	//将信息传递给Imgui
	
	//鼠标点击
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}
	//鼠标释放
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}
	//鼠标移动
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}
	//鼠标滚轮
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}
	//键盘敲击
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		//KeysDown数组是一个bool数组，这里是把glfw那边检测到的键盘按下信息传过来，检测到按下标为真，  话说这一部分是在update函数的初始化里进行初始化吗？
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true; 

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}
	//键盘释放
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}
	//键盘值
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)  //和AddInputCharacter函数的范围一样，使用unsigned short
			io.AddInputCharacter((unsigned short)keycode);  //添加到 Dear ImGui 的输入字符队列中

		return false;
	}
	//窗口变化
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}


}