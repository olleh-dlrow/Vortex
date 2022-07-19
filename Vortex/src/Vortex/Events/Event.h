#pragma once
#include"Vortex/Core/Core.h"

namespace Vortex {
    enum class EventType {
        None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

		MSAAOpened, MSAAClosed, MSAANSamplesChanged,
		HDROpened, HDRClosed, HDRExposureChanged
    };

	/*
	tips:
	this can't be declared to enum class because it will operate(&) with an int value
	*/
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4),
		EventCategoryGraphics		= BIT(5),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type;}\
							   virtual EventType GetEventType() const override {return GetStaticType();}\
							   virtual const char *GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class Event {
	public:
		bool Handled = false;  //some event can be handled by other event

		virtual EventType GetEventType() const = 0;
		virtual const char *GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const {return GetName();}

		bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event): m_Event(event) {
			
		}

		// Dispatch Event of T to certain function F to handle.
		// Only T is the same type as m_Event could F handles it, 
		// otherwise IGNORE
		// If any of the function handled this Event, Dispatch return true,
		// else return false
		//T: Event class, F: function type
		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if ( m_Event.GetEventType() == T::GetStaticType()) {
				/*
				tips:
				static_cast: force type conversion(cast), can't ensure security
				*/
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}