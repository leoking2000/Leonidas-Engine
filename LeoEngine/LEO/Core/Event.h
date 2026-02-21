#pragma once
#include <glm/glm.hpp>
#include <string>
#include <LEO/Utilities/LeoTypes.h>

namespace leo
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};

	class Event final
	{
	public:
	private:
		EventType m_type;
		union EventData
		{
			glm::ivec2 int_data;
			glm::vec2 float_data;
		};
	};



}


