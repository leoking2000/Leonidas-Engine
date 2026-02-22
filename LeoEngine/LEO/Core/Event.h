#pragma once
#include <string>
#include <functional>
#include <format>
#include <LEO/Utilities/LeoTypes.h>

namespace leo
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased,
		MouseMoved, MouseScrolled
	};

	class Event
	{
	public:
		bool Handled = false;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const = 0;
	public:
		virtual	~Event() = default;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType() && !m_Event.Handled)
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

#define EVENT_CLASS_METHODS(type) static  EventType GetStaticType() { return type; }\
							      virtual EventType GetEventType() const override { return GetStaticType(); }\
							      virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS(name, type, to_string_body, ...)\
	class name : public Event\
	{\
	public:\
	name() = default;\
	EVENT_CLASS_METHODS(type)\
	virtual std::string ToString() const override { return to_string_body }\
	__VA_ARGS__\
	}


EVENT_CLASS(WindowCloseEvent, EventType::WindowClose, GetName();, );
EVENT_CLASS(WindowResizeEvent, EventType::WindowResize, std::format("WindowResizeEvent: {}, {}", width, height); , u32 width; u32 height;);

EVENT_CLASS(KeyPressedEvent, EventType::KeyPressed, std::format("KeyPressedEvent: {} (repeat={})", keyCode, isRepeat); , i32 keyCode; bool isRepeat;);
EVENT_CLASS(KeyReleasedEvent, EventType::KeyReleased, std::format("KeyReleasedEvent: {}", keyCode);, i32 keyCode;);

EVENT_CLASS(MouseButtonPressedEvent, EventType::MouseButtonPressed, std::format("MouseButtonPressedEvent: {}", code);, i32 code; f32 x; f32 y;);
EVENT_CLASS(MouseButtonReleasedEvent, EventType::MouseButtonReleased, std::format("MouseButtonReleasedEvent: {}", code);, i32 code; f32 x; f32 y;);

EVENT_CLASS(MouseMovedEvent, EventType::MouseMoved, std::format("MouseMovedEvent: {}, {}", x, y);,f32 x; f32 y;);
EVENT_CLASS(MouseScrolledEvent, EventType::MouseScrolled, std::format("MouseScrolledEvent: {}, {}", offset_x, offset_y);, f32 offset_x; f32 offset_y; f32 x; f32 y;);

}


