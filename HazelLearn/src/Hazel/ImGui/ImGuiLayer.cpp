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
	

	//���һ��ImGui��
	void ImGuiLayer::OnAttach() 
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();  //����һ�� ImGui �������Ķ������ǹ��� ImGui ��״̬�����ݵĺ��Ķ������㿪ʼʹ�� ImGui ���� UI Ԫ��֮ǰ����Ҫ�ȴ���һ�������Ķ���һ�������Ķ��󱻴�������Ϳ��Կ�ʼ��Ӵ��ڡ���ť���ı���� UI Ԫ�أ������佻����
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//����������gui������Щ�������������
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls ���̿���
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls  ��Ϸ�ֱ�����
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking  ͣ������
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows ���ӿڹ���
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;  //������������ʾ���ӿڣ����ƴ򿪶��VS��Ŀ����������Ҳֻ��һ��VSͼ��
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;�������������ӿںϲ�Ϊ��һ����
		ImGui::StyleColorsDark(); //�������� ImGui �Ľ�����Ϊ��ɫ���⡣

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		//����ʽ
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
	
	//ɾ��һ����
	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	
	//Begin+End�����˳���Ⱦ����ĳ�ʼ������β��Begin�����ǳ�ʼ����������������io֮���Ҳ��ʼ���ˣ�Begin+OnRender+End���������Ĺ���
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
	
	//ʵ������������������Ų�UI�ģ��Ƿ���Begin��End�м��
	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}
	
	
	
	
	
	
	


}