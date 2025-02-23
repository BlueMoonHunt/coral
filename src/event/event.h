#pragma once
#include "keyCodes.h"

// #define CORAL_BIND_EVENT_FN(fn) [](auto&&... args)->decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }


//TODO: focus and lostFocus
namespace coral {
	enum class EventType {
		None = 0,
		WindowResize, WindowClose, WindowFocus, WindowLostFocus,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseScrolled, MouseMoved, MouseDragged
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4,
	};

	class Event {
	public:
		virtual ~Event() = default;
		virtual EventType getEventType() = 0;
		virtual int getCategoryFlags() = 0;

		virtual inline bool isInCategory(EventCategory category) {
			return getCategoryFlags() & category;
		}

		bool handled = false;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& e)
			: m_Event(e) {
		}

		template<typename T, typename F>
		bool dispatch(const F& func) {
			if (m_Event.getEventType() == T::getStaticEventType()) {
				m_Event.handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {
		}

		uint32_t getWidth() const { return m_Width; }
		uint32_t getHeight() const { return m_Height; }

		static EventType getStaticEventType() { return EventType::WindowResize; }
		virtual EventType getEventType() override { return EventType::WindowResize; }
		virtual int getCategoryFlags() override { return EventCategoryApplication; }
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		static EventType getStaticEventType() { return EventType::WindowClose; }
		virtual EventType getEventType() override { return EventType::WindowClose; }
		virtual int getCategoryFlags() override { return EventCategoryApplication; }
	};

	class KeyEvent : public Event {
	public:
		KeyEvent(KeyCode _key)
			: key(_key) {
		}

		virtual int getCategoryFlags() override { return EventCategoryKeyboard; }
		KeyCode getKey() { return key; }
	private:
		KeyCode key;
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {
		}

		static EventType getStaticEventType() { return EventType::KeyPressed; }
		virtual EventType getEventType() override { return EventType::KeyPressed; }
		bool IsRepeat() const { return m_IsRepeat; }
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {
		}
		static EventType getStaticEventType() { return EventType::KeyReleased; }
		virtual EventType getEventType() override { return EventType::KeyReleased; }
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {
		}

		static EventType getStaticEventType() { return EventType::KeyTyped; }
		virtual EventType getEventType() override { return EventType::KeyTyped; }
		virtual int getCategoryFlags() override { return EventCategoryKeyboard | EventCategoryInput; }
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y) {
		}

		static EventType getStaticEventType() { return EventType::MouseMoved; }
		virtual EventType getEventType() override { return EventType::MouseMoved; }
		virtual int getCategoryFlags() override { return EventCategoryMouse | EventCategoryInput; }

		float getX() const { return m_MouseX; }
		float getY() const { return m_MouseY; }
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {
		}

		static EventType getStaticEventType() { return EventType::MouseScrolled; }
		virtual EventType getEventType() override { return EventType::MouseScrolled; }
		virtual int getCategoryFlags() override { return EventCategoryMouse | EventCategoryInput; }

		float getXOffset() const { return m_XOffset; }
		float getYOffset() const { return m_YOffset; }

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseButton getMouseButton() const {
			return m_Button;
		}

		virtual int getCategoryFlags() override { return EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton; }
	protected:
		MouseButtonEvent(const MouseButton button)
			: m_Button(button) {
		}

		MouseButton m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const MouseButton button)
			: MouseButtonEvent(button) {
		}

		static EventType getStaticEventType() { return EventType::MouseButtonPressed; }
		virtual EventType getEventType() override { return EventType::MouseButtonPressed; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseButton button)
			: MouseButtonEvent(button) {
		}

		static EventType getStaticEventType() { return EventType::MouseButtonReleased; }
		virtual EventType getEventType() override { return EventType::MouseButtonReleased; }
	};

	class MouseDraggedEvent : public MouseButtonEvent {
	public:
		MouseDraggedEvent(float x, float y, MouseButton button)
			: MouseButtonEvent(button), m_MouseX(x), m_MouseY(y) {
		}

		static EventType getStaticEventType() { return EventType::MouseDragged; }
		virtual EventType getEventType() override { return EventType::MouseDragged; }

		float getX() const { return m_MouseX; }
		float getY() const { return m_MouseY; }

	private:
		float m_MouseX, m_MouseY;
	};

}
