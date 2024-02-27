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
	

	//���һ��ImGui��
	void ImGuiLayer::OnAttach() 
	{
		ImGui::CreateContext();  //����һ�� ImGui �������Ķ������ǹ��� ImGui ��״̬�����ݵĺ��Ķ������㿪ʼʹ�� ImGui ���� UI Ԫ��֮ǰ����Ҫ�ȴ���һ�������Ķ���һ�������Ķ��󱻴�������Ϳ��Կ�ʼ��Ӵ��ڡ���ť���ı���� UI Ԫ�أ������佻����
		ImGui::StyleColorsDark(); //�������� ImGui �Ľ�����Ϊ��ɫ���⡣

		ImGuiIO& io = ImGui::GetIO();  //��ṹ�廹ͦ��
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; //��������깦��
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  //�������λ�ù���    //Ҳ��λ���㣬����˼

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
	
	//ÿһ֡��Imgui���и�����  ÿһ֡��
	void ImGuiLayer::OnUpdate()
	{
		//IO����
		ImGuiIO& io = ImGui::GetIO();  
		Application& app = Application::Get();  //ָ��ǰΨһ�����ָ��
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());


		//ʱ�䲿��
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);   //��һ֡DeltaTime��1/60.0,�ڶ�֡DeltaTime�͸����ˣ��� (time - ��һ֡��time)
		m_Time = time;


		//��Ⱦ����

		/*ͨ������£�������Ⱦѭ���Ŀ�ͷ����������������
		Ȼ������Ⱦѭ�����м������� ImGui UI Ԫ�أ��������Ⱦѭ���Ľ�β���� ImGui::Render() ����� UI ����Ⱦ��*/
		ImGui_ImplOpenGL3_NewFrame(); //�������ͨ����������Ⱦѭ����ʼʱ���ã�������ʼ�� ImGui �ڵ�ǰͼ���������е�һЩ״̬
		ImGui::NewFrame(); //���� ImGui ���һ����Ҫ���������ڿ�ʼ�µ�֡��frame�����ڵ����������֮��������ڵ�ǰ֡����� ImGui �� UI Ԫ�أ��簴ť���ı���ȡ�

		static bool show = true;
		ImGui::ShowDemoWindow(&show);  //�������չʾ�������Ǹ�imgui����һ��Demo������ImGui::Button()������Imgui�¼��������ǻ�û��ʹ�ã�����û�н���
		//��ImGui::Render();֮ǰ������Ҫ����Լ���GUI�����߽���DemoҲ�У�������Ū������UIԪ�ص�

		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  //��ȡ���Ƶ����ݣ���ʵ��
	}

	//�õ�����¼�������¼�����ʱ   ����ᱻApplication::OnEvent���ã�����GLFW�����¼�ʱ��event����GLFW�����¼����ƺ�ֻ����Щ��GLFWע��ĺ��������
	void ImGuiLayer::OnEvent(Event& event)
	{

	}

}