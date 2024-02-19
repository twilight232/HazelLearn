#pragma once


#include "../src/Hazel/Core.h"


#include "../hzpch.h"

namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	//这个事件系统是Imemediate事件系统，当事件发生时，立即被派发并进行处理，并阻塞其他正在处理的事件
	/*如一个炸弹引发其他炸弹爆炸，事件产生的速度很快，并且相互关联
	如果是立即执行的，可能执行到一半就去执行另外一个，，，，然后另外一个还不见得能很快做完，会阻塞线程，要别的事件等（此时帧率会急速下降）
	造成阻塞，然后还是环环嵌套，形成一个很深的栈*/

	//这种设计的事件系统，并行化支持不好，目前的商业引擎都没有用这种

	//现代游戏引擎更多还是为事件系统形成一个Event Queue，将事件排在队列中，并且进行序列化和反序列化，来加速读取，并且使用Ring buffer
	

	//枚举事件类型啊
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,  //窗口事件
		AppTick, AppUpdate, AppRender,   //应用事件，游戏运行时间被分为一个个Tick，
		KeyPressed, KeyReleased,        //键盘事件
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled       //鼠标事件
	};


	enum EventCategory
	{
		//1的二进制补码为0000 0000 0000 0000 0000 0000 0000 0001

		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	//第一个是拼成EventType::type;   第三个是将枚举变量变量名的字符串形式返回回去  
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	//这个类里的成员函数就是上面宏所定义的那些
	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;   //纯虚函数啊
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }  //这个const代表这个函数不会造成对象状态的改变，很好的编程习惯

		inline bool IsInCategory(EventCategory category)  //事件是否在队列category中
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;  //事件是否解决
	};


	//事件派发类，事件类的友元类啊    让事件派发类可以访问事件类的私有和保护成员。。。不干涉继承关系     方便做一些事情，
	//用于快速过滤掉某些事件
	class EventDispatcher
	{
		//这里的T，可以是任何事件类型
		template<typename T>
		using EventFn = std::function<bool(T&)>;  //取了个别名EventFn，为一个传入T&，返回bool值的函数
	public:
		EventDispatcher(Event& event): m_Event(event)   //构造函数，赋个值而已，将event赋值给m_Event
		{

		}

		template<typename T>
		bool Dispatch(EventFn<T> func)  //传入一个函数对象
		{
			if (m_Event.GetEventType() == T::GetStaticType())   //如果事件类型匹配，则调用传入的函数对象；即如果m_Event和T的类型符合，下面直接强转指针类型
			{
				m_Event.m_Handled = func(*(T*)&m_Event);  //强转指针类型，然后调用函数，返回事件是否解决的bool值
				return true;
			}
			//事件类型不匹配，返回false
			return false;
		}
	private:
		Event& m_Event;
	};

	//重载运算符，允许我们用ostream直接输出事件e，不过输出的是事件的文本表示格式，不是真的事件。    ostream是输出流
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();    
	}
}