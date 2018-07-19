#include "stable.h"
#include "window.h"

namespace Storm 
{
    class DummyWindow
    {
    public:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            Window *pThis = nullptr;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (Window*)pCreate->lpCreateParams;
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
                pThis->windowHandle_ = hwnd;
            }
            else
            {
                pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }
            if (pThis)
            {
                return pThis->windowProc(uMsg, wParam, lParam);
            }
            else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }
    };
   

    class  WindowClassRegister
    {
    public:
        WindowClassRegister() { ; }
        ~WindowClassRegister()
        {
            for (std::vector<std::wstring>::iterator i = m_windowClass.begin(); i != m_windowClass.end(); ++i)
            {
                ::UnregisterClassW(i->c_str(), nullptr);
            }
        }

        static WindowClassRegister& instance()
        {
            static WindowClassRegister rr;
            return rr;
        }

        bool registerWindowClass(const std::wstring& cls)
        {
            WNDCLASSEX wc = { 0 };
            wc.cbSize = sizeof(WNDCLASSEX);
            wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            wc.lpfnWndProc = DummyWindow::WindowProc;
            wc.hInstance = GetModuleHandle(nullptr);
            wc.lpszClassName = cls.c_str();
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

            ATOM r = RegisterClassEx(&wc);
            DAssert_M(r, "registerClass");
            return !!r;
        }

        bool registerClass(const std::wstring& cls)
        {
            if (isRegistered(cls))
            {
                return true;
            }

            if (registerWindowClass(cls))
            {
                m_windowClass.push_back(cls);
                return true;
            }
            else
                return false;
        }

        bool isRegistered(const std::wstring& cls)
        {
            return std::find(m_windowClass.begin(), m_windowClass.end(), cls) != m_windowClass.end();
        }

    private:
        std::vector<std::wstring> m_windowClass;
    };

    std::vector<MouseButton> getInputMouseButtons(WPARAM wParam)
    {
         std::vector<MouseButton> mouseButton;
        if (wParam & MK_LBUTTON)
        {
            mouseButton.push_back(E_MB_Left);
        }
        if (wParam & MK_RBUTTON)
        {
            mouseButton.push_back(E_MB_Right);
        }
        if (wParam & MK_MBUTTON)
        {
            mouseButton.push_back(E_MB_Middle);
        }
        return mouseButton;
    }

    int getInputKeyModifiers() 
    {
        int mods = 0;
        if (::GetKeyState(VK_SHIFT) & (1 << 31)) {
            mods |= E_ShiftModifier;
        }
        if (::GetKeyState(VK_CONTROL) & (1 << 31)) {
            mods |= E_ControlModifier;
        }
        if (::GetKeyState(VK_MENU) & (1 << 31)) {
            mods |= E_AltModifier;
        }
        return mods;
    }


    Window::Window()
    {
        style_ = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        styleEx_ = WS_EX_APPWINDOW;
    }

    Window::~Window()
    {
        destroy();
    }

    bool Window::create(const DisplayParam& param)
    {
        DAssert(windowHandle_ == nullptr);
        static std::wstring windowclass = L"Window";
        WindowClassRegister::instance().registerClass(windowclass);

        windowed_ = param.windowed;
        if (param.windowed)
        {
            style_ |= WS_OVERLAPPEDWINDOW;
            styleEx_ |= WS_EX_WINDOWEDGE;
        }
        else
        {
            style_ |= WS_POPUP;
        }

        RECT rc = { 0, 0, param.width, param.height };
        AdjustWindowRectEx(&rc, style_, FALSE, styleEx_);

        int frameWidth = rc.right - rc.left;
        int frameHeight = rc.bottom - rc.top;

        CreateWindowExW(
            styleEx_, windowclass.c_str(), param.title.c_str(), style_, 
            CW_USEDEFAULT, CW_USEDEFAULT, frameWidth, frameHeight, 
            nullptr, nullptr, GetModuleHandle(nullptr), this);

        if (!windowHandle_)
        {
            return false;
        }

        title_ = param.title;

        GetClientRect(windowHandle_, &rc);
        clientRect_.setLeft(rc.left);
        clientRect_.setTop(rc.top);
        clientRect_.setRight(rc.right);
        clientRect_.setBottom(rc.bottom);

        GetWindowRect(windowHandle_, &rc);
        frameRect_.setLeft(rc.left);
        frameRect_.setTop(rc.top);
        frameRect_.setRight(rc.right);
        frameRect_.setBottom(rc.bottom);

        return true;
    }

    void Window::show()
    {
        DAssert(windowHandle_);
        ShowWindow(windowHandle_, SW_SHOWNORMAL);
    }

    void Window::hide()
    {
        DAssert(windowHandle_);
        ShowWindow(windowHandle_, SW_HIDE);
    }

    void Window::close()
    {
        DAssert(windowHandle_);
        CloseWindow(windowHandle_);
    }

    void Window::move(const Storm::PointI& pos)
    {
        DAssert(windowHandle_);
        ::MoveWindow(windowHandle_, pos.x(), pos.y(), frameWidth(), frameHeight(), FALSE);
    }

    void Window::resize(const Storm::SizeI& sz)
    {
        DAssert(windowHandle_);

        RECT rc = { 0, 0, sz.width(), sz.height() };
        AdjustWindowRectEx(&rc, style_, FALSE, styleEx_);
        int frameWidth = rc.right - rc.left;
        int frameHeght = rc.bottom - rc.top;
        ::MoveWindow(windowHandle_, frameRect_.left(), frameRect_.top(), frameWidth, frameHeght, FALSE);
    }

    void Window::setTitle(const std::wstring& title)
    {
        if (::IsWindow(windowHandle_))
        {
            ::SetWindowTextW(windowHandle_, title.c_str());
        }
        title_ = title;
    }

    const std::wstring& Window::title() const
    {
        return title_;
    }

    bool Window::isWindowed() const
    {
        return windowed_;
    }

    bool Window::isVisible() const
    {
        return visible_;
    }

    bool Window::isActive() const
    {
        return active_;
    }

    Storm::RectI Window::clientRect() const
    {
        return clientRect_;
    }

    int Window::clientWidth() const
    {
        return clientRect_.width();
    }

    int Window::clientHeight() const
    {
        return clientRect_.height();
    }

    int Window::frameWidth() const
    {
        return frameRect_.width();
    }

    int Window::frameHeight() const
    {
        return frameRect_.height();
    }

    void Window::destroy()
    {
        if (windowHandle_)
        {
            DestroyWindow(windowHandle_);
            windowHandle_ = nullptr;
        }
    }

    LRESULT Window::windowProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
        break;
        case WM_SHOWWINDOW:
        {
            visible_ = !!wParam;
            this->showHideEvent_(this, visible_);
        }
        break;
        case WM_ACTIVATE:
        {
            active_ = !!wParam;
            this->activeEvent_(this, active_);
        }
        break;
        case WM_CLOSE:
        {
            destroy();
            visible_ = false;
            active_ = false;
        }
            return 0;
        case WM_DESTROY:
        {
            this->destroyEvent_(this);
            windowHandle_ = nullptr;
        }
        break;
        case WM_MOVING:
        {
            RECT* rect = (RECT*)lParam;
            this->frameRect_ = RectI(rect->left, rect->top, rect->right, rect->bottom);
        }
        break;
        case WM_SIZE:
        {
            RECT rect = { 0 };
            ::GetWindowRect(windowHandle_, &rect);
            this->frameRect_ = RectI(rect.left, rect.top, rect.right, rect.bottom);
            ::GetClientRect(windowHandle_, &rect);
            this->clientRect_ = RectI(rect.left, rect.top, rect.right, rect.bottom);
            this->resizeEvent_(this, clientRect_.size());
        }
        break;
        case WM_MOVE:
        {
            RECT rect = { 0 };
            ::GetWindowRect(windowHandle_, &rect);
            this->frameRect_ = RectI(rect.left, rect.top, rect.right, rect.bottom);
            this->moveEvent_(this, frameRect_.topLeft());
        }
        break;
        case WM_ERASEBKGND:
            return TRUE;
        case WM_PAINT:
        {
            this->paintEvent_(this);
        }
        break;
        case WM_MOUSEMOVE:
        {
            POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
            PointI clientPos(pt.x, pt.y);

            if (!mouseOver_)
            {
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(tme);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = windowHandle_;
                tme.dwHoverTime = 0;
                ::TrackMouseEvent(&tme);
                mouseOver_ = true;
                this->cursorEnterEvent_(this, clientPos);
            }
            
            this->cursorMoveEvent_(this, clientPos);
        }
        break;
        case WM_MOUSELEAVE:
        {
            mouseOver_ = false;
            DWORD msgPos = ::GetMessagePos();
            POINT pt = { (int)(short)LOWORD(msgPos), (int)(short)HIWORD(msgPos) };
            PointI screenPos(pt.x, pt.y);
            ::ScreenToClient(windowHandle_, &pt);
            PointI clientPos(pt.x, pt.y);

            this->cursorLeaveEvent_(this, clientPos);
        }
        break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        {
            ::SetCapture(windowHandle_);
            mousePressing_ = true;
            int keyModifiers = getInputKeyModifiers();

            POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
            PointI clientPos(pt.x, pt.y);
            MouseButton btn = (message == WM_LBUTTONDOWN) ? E_MB_Left : (message == WM_RBUTTONDOWN) ? E_MB_Right : (message == WM_MBUTTONDOWN) ? E_MB_Middle : E_MB_None;
            this->mousePressEvent_(this, btn, clientPos, (ModifierKey)keyModifiers);
        }
        break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            ::ReleaseCapture();
            mousePressing_ = false;
            int keyModifiers = getInputKeyModifiers();

            POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
            PointI clientPos(pt.x, pt.y);
            MouseButton btn = (message == WM_LBUTTONDOWN) ? E_MB_Left : (message == WM_RBUTTONDOWN) ? E_MB_Right : (message == WM_MBUTTONDOWN) ? E_MB_Middle : E_MB_None;
            this->mouseReleaseEvent_(this, btn, clientPos, (ModifierKey)keyModifiers);
        }
        break;
        case WM_CAPTURECHANGED:
        {
            if ((HWND)lParam != windowHandle_)
            {
                if (mousePressing_)
                {
                    DWORD msgPos = ::GetMessagePos();
                    POINT pt = { (int)(short)LOWORD(msgPos), (int)(short)HIWORD(msgPos) };
                    PointI screenPos(pt.x, pt.y);
                    ::ScreenToClient(windowHandle_, &pt);
                    PointI clientPos(pt.x, pt.y);

                    this->mouseReleaseEvent_(this, E_MB_None, clientPos, E_NoModifier);
                }
                mousePressing_ = false;
            }
        }
        break;
        case WM_MOUSEWHEEL:
        {
            double step = (SHORT)HIWORD(wParam) / 120.0;
            this->wheelEvent_(this, step);
        }
        return 0;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            int keyModifiers = getInputKeyModifiers();
            int key = (int)wParam;
            this->keyDownEvent_(this, (InputKey)key, (ModifierKey)keyModifiers);
        }
        break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            int keyModifiers = getInputKeyModifiers();
            int key = (int)wParam;

            this->keyUpEvent_(this, (InputKey)key, (ModifierKey)keyModifiers);
        }
        break;
        case WM_CHAR:
        //case WM_SYSCHAR:
        {
            int keyModifiers = getInputKeyModifiers();
            this->charEvent_(this, (wchar_t)wParam, (ModifierKey)keyModifiers);
        }
        break;
        default:
            break;
        }

        return ::DefWindowProc(windowHandle_, message, wParam, lParam);
    }

}

