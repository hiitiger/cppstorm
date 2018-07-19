#pragma once

namespace Storm
{

    struct DisplayParam
    {
        int width = 800;
        int height = 600;
        bool windowed = true;
        std::wstring title = L"Hello World!";

        DisplayParam() = default;
        DisplayParam(int w, int h, bool win, const std::wstring& t)
            : width(w), height(h), windowed(win), title(t)
        {
            
        }
    };

    class Window
    {
        STORM_NONCOPYABLE(Window);
    public:
        Window();
        virtual ~Window();

        bool create(const DisplayParam& param = DisplayParam());

        void show();
        void hide();
        void close();
        void move(const Storm::PointI& pos);
        void resize(const Storm::SizeI& sz);

        void setTitle(const std::wstring&);
        const std::wstring& title() const;

        bool isWindowed() const;
        bool isVisible() const;
        bool isActive() const;

        RectI clientRect() const;
        int clientWidth() const;
        int clientHeight() const;
        int frameWidth() const;
        int frameHeight() const;

        HWND windowId() const { return windowHandle_; }

        Event<void(Window*, bool)>&                                             showHideEvent        (){ return showHideEvent_; }
        Event<void(Window*, bool)>&                                             activeEvent          (){ return activeEvent_; }
        Event<void(Window*)>&                                                   destroyEvent         (){ return destroyEvent_; }
        Event<void(Window*, const Storm::SizeI&)>&                              resizeEvent          (){ return resizeEvent_; }
        Event<void(Window*, const Storm::PointI&)>&                             moveEvent            (){ return moveEvent_; }
        Event<void(Window*)>&                                                   paintEvent           (){ return paintEvent_; }
        Event<void(Window*, const Storm::PointI&)>&                             cursorEnterEvent     (){ return cursorEnterEvent_; }
        Event<void(Window*, const Storm::PointI&)>&                             cursorMoveEvent      (){ return cursorMoveEvent_; }
        Event<void(Window*, const Storm::PointI&)>&                             cursorLeaveEvent     (){ return cursorLeaveEvent_; }
        Event<void(Window*, MouseButton, const Storm::PointI&, ModifierKey)>&   mousePressEvent      (){ return mousePressEvent_; }
        Event<void(Window*, MouseButton, const Storm::PointI&, ModifierKey)>&   mouseReleaseEvent    (){ return mouseReleaseEvent_; }
        Event<void(Window*, MouseButton, const Storm::PointI&, ModifierKey)>&   mouseDoubleClickEvent(){ return mouseDoubleClickEvent_; }
        Event<void(Window*, double)>&                                           wheelEvent           (){ return wheelEvent_; }
        Event<void(Window*, InputKey, ModifierKey)>&                            keyDownEvent         (){ return keyDownEvent_; }
        Event<void(Window*, InputKey, ModifierKey)>&                            keyUpEvent           (){ return keyUpEvent_; }
        Event<void(Window*, wchar_t, ModifierKey)>&                             charEvent            (){ return charEvent_; }

    protected:
        void destroy();
        LRESULT windowProc(UINT message, WPARAM wParam, LPARAM lParam);

    private:
        Event<void(Window*, bool)> showHideEvent_;
        Event<void(Window*, bool)> activeEvent_;
        Event<void(Window*)> destroyEvent_;
        Event<void(Window*, const Storm::SizeI& )> resizeEvent_;
        Event<void(Window*, const Storm::PointI&)> moveEvent_;
        Event<void(Window*)> paintEvent_;
        Event<void(Window*, const Storm::PointI&)> cursorEnterEvent_;
        Event<void(Window*, const Storm::PointI&)> cursorMoveEvent_;
        Event<void(Window*, const Storm::PointI&)> cursorLeaveEvent_;
        Event<void(Window*, MouseButton, const Storm::PointI&, ModifierKey)> mousePressEvent_;
        Event<void(Window*, MouseButton, const Storm::PointI&, ModifierKey)> mouseReleaseEvent_;
        Event<void(Window*, MouseButton, const Storm::PointI&, ModifierKey)> mouseDoubleClickEvent_;
        Event<void(Window*, double)> wheelEvent_;
        Event<void(Window*, InputKey, ModifierKey)> keyDownEvent_;
        Event<void(Window*, InputKey, ModifierKey)> keyUpEvent_;
        Event<void(Window*, wchar_t, ModifierKey)> charEvent_;

    protected:
        std::wstring title_;
        RectI clientRect_;
        RectI frameRect_;

        unsigned int style_ = 0;
        unsigned int styleEx_ = 0;

        HWND windowHandle_ = nullptr;
        bool windowed_ = true;
        bool visible_ = false;
        bool active_ = false;
        bool mouseOver_ = false;
        bool mousePressing_ = false;

        friend class DummyWindow;
    };
}
