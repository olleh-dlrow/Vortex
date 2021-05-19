#pragma once

#include "Event.h"

namespace Vortex {
    class VORTEX_API KeyEvent: public Event {
    public:
        int GetKeyCode() const {return m_KeyCode;}

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keycode): m_KeyCode(keycode) {}
        int m_KeyCode;
    };

    class VORTEX_API KeyPressedEvent: public KeyEvent 
    {
    public:
        KeyPressedEvent(const int keycode, int repeatCount): 
            KeyEvent(keycode), m_RepeatCount(repeatCount)
        {
            
        }

        int GetRepeatCount() const {return m_RepeatCount;}

        std::string ToString() const override 
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

	class VORTEX_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class VORTEX_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}