#include "hzpch.h"
#include "ImGuiLayer.h"

#include "../../HazelLearn/vendor/imgui/imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "glfw3.h"
//#include "../../Glad/include/glad/glad.h"

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
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();  //创建一个 ImGui 的上下文对象，它是管理 ImGui 库状态和数据的核心对象。在你开始使用 ImGui 创建 UI 元素之前，需要先创建一个上下文对象。一旦上下文对象被创建，你就可以开始添加窗口、按钮、文本框等 UI 元素，并与其交互。
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//以下是允许gui进行哪些操作，自行添加
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls 键盘控制
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls  游戏手柄控制
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking  停靠功能
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows 多视口功能
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;  //不在任务栏显示多视口，类似打开多个VS项目，任务栏上也只有一个VS图标
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;　／／不将多视口合并为单一窗口
		ImGui::StyleColorsDark(); //用于设置 ImGui 的界面风格为暗色主题。

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		//改样式
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);

		ImGui_ImplOpenGL3_Init("#version 410");
	}
	
	//删除一个层
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	
	//Begin+End构成了除渲染完外的初始化和收尾，Begin更多是初始化，这三个函数把io之类的也初始化了，Begin+OnRender+End就是完整的过程
	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	
	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
	
	//实际上这个函数是用来排布UI的，是放在Begin和End中间的
	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}
	
	
	
	
	
	
	


}