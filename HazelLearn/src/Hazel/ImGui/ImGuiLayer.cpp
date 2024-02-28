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
	
	//ɾ��һ����
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
		//��Ⱦ��ʼ��
		ImGui_ImplOpenGL3_NewFrame(); //�������ͨ����������Ⱦѭ����ʼʱ���ã�������ʼ�� ImGui �ڵ�ǰͼ���������е�һЩ״̬
		ImGui::NewFrame(); //���� ImGui ���һ����Ҫ���������ڿ�ʼ�µ�֡��frame�����ڵ����������֮��������ڵ�ǰ֡����� ImGui �� UI Ԫ�أ��簴ť���ı���ȡ�

		//���UIԪ��
		static bool show = true;
		ImGui::ShowDemoWindow(&show);  //�������չʾ�������Ǹ�imgui����һ��Demo������ImGui::Button()������Imgui�¼��������ǻ�û��ʹ�ã�����û�н���
		//��ImGui::Render();֮ǰ������Ҫ����Լ���GUI�����߽���DemoҲ�У�������Ū������UIԪ�ص�

		
		//��ʼ��Ⱦ
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());  //��ȡ���Ƶ����ݣ���ʵ��
	}

	//�õ�����¼�������¼�����ʱ   ����ᱻApplication::OnEvent���ã�����GLFW�����¼�ʱ��event����GLFW�����¼����ƺ�ֻ����Щ��GLFWע��ĺ��������
	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		//��GLFW�Ǳߴ������¼��ַ���imgui
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));


	}
	
	
	
	
	//����Ϣ���ݸ�Imgui
	
	//�����
	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}
	//����ͷ�
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}
	//����ƶ�
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}
	//������
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}
	//�����û�
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		//KeysDown������һ��bool���飬�����ǰ�glfw�Ǳ߼�⵽�ļ��̰�����Ϣ����������⵽���±�Ϊ�棬  ��˵��һ��������update�����ĳ�ʼ������г�ʼ����
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true; 

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}
	//�����ͷ�
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}
	//����ֵ
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)  //��AddInputCharacter�����ķ�Χһ����ʹ��unsigned short
			io.AddInputCharacter((unsigned short)keycode);  //��ӵ� Dear ImGui �������ַ�������

		return false;
	}
	//���ڱ仯
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}


}