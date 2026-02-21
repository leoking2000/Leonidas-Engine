#pragma once
#include <LEO/Utilities/LeoTypes.h>
#include "Event.h"

namespace leo
{
	class Layer
	{
	public:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
	public:
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(leo::f32 dt) {}
	public:
		virtual ~Layer() = default;
	};
}
