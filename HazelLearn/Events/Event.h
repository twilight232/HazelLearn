#pragma once


#include "../src/Hazel/Core.h"


#include "../hzpch.h"

namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	//����¼�ϵͳ��Imemediate�¼�ϵͳ�����¼�����ʱ���������ɷ������д����������������ڴ�����¼�
	/*��һ��ը����������ը����ը���¼��������ٶȺܿ죬�����໥����
	���������ִ�еģ�����ִ�е�һ���ȥִ������һ����������Ȼ������һ�����������ܺܿ����꣬�������̣߳�Ҫ����¼��ȣ���ʱ֡�ʻἱ���½���
	���������Ȼ���ǻ���Ƕ�ף��γ�һ�������ջ*/

	//������Ƶ��¼�ϵͳ�����л�֧�ֲ��ã�Ŀǰ����ҵ���涼û��������

	//�ִ���Ϸ������໹��Ϊ�¼�ϵͳ�γ�һ��Event Queue�����¼����ڶ����У����ҽ������л��ͷ����л��������ٶ�ȡ������ʹ��Ring buffer
	

	//ö���¼����Ͱ�
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,  //�����¼�
		AppTick, AppUpdate, AppRender,   //Ӧ���¼�����Ϸ����ʱ�䱻��Ϊһ����Tick��
		KeyPressed, KeyReleased,        //�����¼�
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled       //����¼�
	};


	enum EventCategory
	{
		//1�Ķ����Ʋ���Ϊ0000 0000 0000 0000 0000 0000 0000 0001

		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	//��һ����ƴ��EventType::type;   �������ǽ�ö�ٱ������������ַ�����ʽ���ػ�ȥ  
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	//�������ĳ�Ա����������������������Щ
	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;   //���麯����
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }  //���const�����������������ɶ���״̬�ĸı䣬�ܺõı��ϰ��

		inline bool IsInCategory(EventCategory category)  //�¼��Ƿ��ڶ���category��
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;  //�¼��Ƿ���
	};


	//�¼��ɷ��࣬�¼������Ԫ�డ    ���¼��ɷ�����Է����¼����˽�кͱ�����Ա������������̳й�ϵ     ������һЩ���飬
	//���ڿ��ٹ��˵�ĳЩ�¼�
	class EventDispatcher
	{
		//�����T���������κ��¼�����
		template<typename T>
		using EventFn = std::function<bool(T&)>;  //ȡ�˸�����EventFn��Ϊһ������T&������boolֵ�ĺ���
	public:
		EventDispatcher(Event& event): m_Event(event)   //���캯��������ֵ���ѣ���event��ֵ��m_Event
		{

		}

		template<typename T>
		bool Dispatch(EventFn<T> func)  //����һ����������
		{
			if (m_Event.GetEventType() == T::GetStaticType())   //����¼�����ƥ�䣬����ô���ĺ������󣻼����m_Event��T�����ͷ��ϣ�����ֱ��ǿתָ������
			{
				m_Event.m_Handled = func(*(T*)&m_Event);  //ǿתָ�����ͣ�Ȼ����ú����������¼��Ƿ�����boolֵ
				return true;
			}
			//�¼����Ͳ�ƥ�䣬����false
			return false;
		}
	private:
		Event& m_Event;
	};

	//���������������������ostreamֱ������¼�e��������������¼����ı���ʾ��ʽ����������¼���    ostream�������
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();    
	}
}