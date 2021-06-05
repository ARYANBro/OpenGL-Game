#pragma once

#define BIND_FUNCTION(function) [this](auto&&... args)	\
{																\
	function(args...);											\
}																\

#define EVENT_CLASS(class)\
virtual EventType GetType() const noexcept override { return EventType::class; }\
static EventType GetStaticType() noexcept { return EventType:::class; }
				
enum class EventType
{
	Event, WindowResizeEvent
};

class Event
{
public:
	virtual EventType GetType() const noexcept = 0;
	static EventType GetStaticType() noexcept { return EventType::Event; }
};

class EventDispatcher
{
public:
	EventDispatcher(const Event& event) noexcept
		: m_Event(event)
	{
	}

	template<typename EventType, typename Function>
	void Dispatch(Function function) const noexcept
	{
		if (m_Event.GetType() == EventType::GetStaticType())
			function(reinterpret_cast<const EventType&>(m_Event));
	}

private:
	const Event& m_Event;
};