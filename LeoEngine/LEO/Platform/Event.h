#pragma once
#include <string>
#include "../Utilities/Types.h"
#include "Keys.h"

namespace LEO
{
	enum class EventType
	{
		KeyEvent, 
		MouseEvent,
		WindowResizeEvent,
		ApplicationEvent
	};

	class Event
	{
	public:
		virtual	~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const = 0;
	};

#define EVENT_CAST(e, type) reinterpret_cast<const type&>(e)

#define EVENT_CLASS_METHODS(type)  EventType GetEventType() const override { return type; }\
								   const char* GetName() const override { return #type; }\
								   std::string ToString() const override { return GetName(); }

#define EVENT_CLASS(name, type, ...)\
	class name : public Event\
	{\
	public:\
	name() = default;\
	EVENT_CLASS_METHODS(type)\
	__VA_ARGS__\
	}

	EVENT_CLASS(KeyEvent, EventType::KeyEvent, i32 keyCode; i32 action;);

	EVENT_CLASS(MouseEvent, EventType::MouseEvent, i32 keycode; i32 action; f32 x; f32 y;);

	EVENT_CLASS(WindowReseizeEvent, EventType::WindowResizeEvent, u32 width; u32 height;);

	enum class ApplicationEventAction
	{
		CLOSE_APP,
		TRANSITION_TO_SCENE,
		ADD_SCENE,
		REMOVE_SCENE
	};

	EVENT_CLASS(ApplicationEvent, EventType::ApplicationEvent, ApplicationEventAction action; u32 scene_id;);
}