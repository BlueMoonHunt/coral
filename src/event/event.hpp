#include "base.hpp"
#include "keyCodes.hpp"

namespace coral {

    class Event {
    public:
        virtual ~Event() = default;

        bool handled = false;
        FLAG flag = CoralEventFlag_None;
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(int width, int height)
            : size({ width,height }) {
            flag |= CoralEventFlag_WindowResize;
        }

        i32vec2_t getSize() const { return size; }

    private:
        i32vec2_t size;
    };

    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {
            flag |= CoralEventFlag_WindowClose;
        }
    };

    class WindowFocusEvent : public Event {
    public:
        WindowFocusEvent() {
            flag |= CoralEventFlag_WindowFocus;
        }
    };

    class WindowLostFocusEvent : public Event {
    public:
        WindowLostFocusEvent() {
            flag |= CoralEventFlag_WindowLostFocus;
        }
    };

    class KeyEvent : public Event {
    public:
        KeyEvent(KeyCode _key)
            : key(_key) {
        }
        KeyCode getKey() { return key; }
    private:
        KeyCode key;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(KeyCode key, bool repeating = false)
            : KeyEvent(key) {
            flag |= CoralEventFlag_KeyPressed;
            if (repeating)
                flag |= CoralEventFlag_KeyRepeat;
        }
        bool isRepeating() const { return flag & CoralEventFlag_KeyRepeat; }
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(KeyCode key)
            : KeyEvent(key) {
            flag |= CoralEventFlag_KeyReleased;
        }
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(KeyCode key)
            : KeyEvent(key) {
            flag |= CoralEventFlag_KeyTyped;
        }
    };

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(const float x, const float y)
            : pos({ x,y }) {
            flag |= CoralEventFlag_MouseMove;
        }
        vec2_t getPos() const { return pos; }
    private:
        vec2_t pos;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset)
            : offset({ xOffset, yOffset }) {
            flag |= CoralEventFlag_MouseScroll;
        }

        vec2_t getOffset() const { return offset; }

    private:
        vec2_t offset;
    };

    class MouseButtonEvent : public Event {
    public:
        MouseButton getMouseButton() const {
            return m_Button;
        }

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
            flag |= CoralEventFlag_MouseButtonPressed;
        }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(const MouseButton button)
            : MouseButtonEvent(button) {
            flag |= CoralEventFlag_MouseButtonReleased;
        }
    };

    class InputEvent : public Event {
    public:
        InputEvent() {
            flag |= CoralEventFlag_Input;
        }
    };
} // namespace coral