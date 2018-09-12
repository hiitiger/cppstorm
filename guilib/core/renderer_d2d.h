#pragma once
#include "gfx2dadapter.h"

namespace Storm
{

    inline D2D1::ColorF toD2D1ColorF(Color const& color)
    {
        return D2D1::ColorF(static_cast<float>(color.red()) / 255.f, static_cast<float>(color.green()) / 255.f, static_cast<float>(color.blue()) / 255.f, static_cast<float>(color.alpha()) / 255.f);
    }

    inline Color fromD2D1ColorF(D2D1::ColorF const& color)
    {
        return Color(static_cast<int>(color.r * 255.0), static_cast<int>(color.g * 255.0), static_cast<int>(color.b * 255.0), static_cast<int>(color.a * 255.0));
    }

    inline Color fromD2D1ColorF(D2D1_COLOR_F const& color)
    {
        return Color(static_cast<int>(color.r * 255.0), static_cast<int>(color.g * 255.0), static_cast<int>(color.b * 255.0), static_cast<int>(color.a * 255.0));
    }

    inline D2D1_POINT_2F toD2D1PointF(PointI const& pt)
    {
        return D2D1::Point2F(static_cast<float>(pt.x()), static_cast<float>(pt.y()));
    }

    inline D2D1_POINT_2F toD2D1PointF(PointF const& pt)
    {
        return D2D1::Point2F(pt.x(), pt.y());
    }

    inline D2D1_RECT_F toD2D1RectF(RectI const& rc)
    {
        return D2D1::RectF(static_cast<float>(rc.left()), static_cast<float>(rc.top()), static_cast<float>(rc.right()), static_cast<float>(rc.bottom()));
    }

    inline D2D1_RECT_F toD2D1RectF(RectF const& rc)
    {
        return D2D1::RectF(rc.left(), rc.top(), rc.right(), rc.bottom());
    }

    inline D2D1_SIZE_U toD2D1SizeU(SizeI const& size)
    {
        return D2D1::SizeU(size.width(), size.height());
    }
#define USE_BITMAPTARGET 1

    class Gfx2DRendererD2D : public Gfx2DRenderer
    {
    public:
        Gfx2DRendererD2D(Window* window)
            : window_(window)
        {
        }

        ~Gfx2DRendererD2D() {  }

        virtual bool setup()
        {
            if (!setupRenderTarget())
            {
                return false;
            }

            if (!setupDirect2DResource())
            {
                return false;
            }

            if (!setupFont())
            {
                return false;
            }

            return true;
        }

        virtual Windows::ComPtr<ID2D1RenderTarget> renderTarget()
        {
#if USE_BITMAPTARGET
            Windows::ComPtr<ID2D1RenderTarget> rt;
            d2d1Context_.QueryInterface(rt);
            return rt;
#else
            return d2d1RenderTarget_;
#endif
            
        }

        virtual bool begin()
        {
            if (!renderTarget())
            {
                if (!setup())
                {
                    return false;
                }
            }
            renderTarget()->BeginDraw();

            return true;
        }

        virtual void end()
        {
            renderTarget()->EndDraw();
            dxgiSwapChain_->Present (0, DXGI_PRESENT_DO_NOT_WAIT);
        }

        virtual void resize(const Storm::SizeI& sz)
        {
            disposeResource();
            resizeSwapChain(sz);
#if USE_BITMAPTARGET
            setupD2D1TargetBitmap();
#else 
            setupDXGISurfaceTarget();
#endif
            setupDirect2DResource();
        }

        virtual void clear(const Storm::Color& color)
        {
            renderTarget()->Clear(toD2D1ColorF(color));
        }

        virtual void setBrushColor(const Storm::Color& color)
        {
            colorBrush_->SetColor(toD2D1ColorF(color));
        }

        virtual Storm::Color brushColor() const
        {
            return fromD2D1ColorF(colorBrush_->GetColor());
        }

        virtual void drawRect(const Storm::RectF& rect)
        {
            RectF rcAdjust(rect.left() + 0.5f, rect.top() + 0.5f, rect.right() - 0.5f, rect.bottom() - 0.5f);
            renderTarget()->DrawRectangle(toD2D1RectF(rcAdjust), colorBrush_, 1.0f);
        }

        virtual void drawRect(const Storm::RectF& rect, const Storm::Color& color)
        {
            D2D1_COLOR_F savedColor = colorBrush_->GetColor();
            colorBrush_->SetColor(toD2D1ColorF(color));
            drawRect(rect);
            colorBrush_->SetColor(savedColor);
        }

        virtual void fillRect(const Storm::RectF& rect)
        {
            RectF rcAdjust(rect.left() + 0.5f, rect.top() + 0.5f, rect.right() - 0.5f, rect.bottom() - 0.5f);
            renderTarget()->FillRectangle(toD2D1RectF(rcAdjust), colorBrush_);
        }

        virtual void fillRect(const Storm::RectF& rect, const Storm::Color& color)
        {
            D2D1_COLOR_F savedColor = colorBrush_->GetColor();
            colorBrush_->SetColor(toD2D1ColorF(color));
            fillRect(rect);
            colorBrush_->SetColor(savedColor);
        }

        virtual void drawImage()
        {

        }

        virtual void drawText(const std::wstring& text, const Storm::RectF& rect)
        {
            renderTarget()->DrawTextW(text.c_str(), static_cast<uint32_t>(text.length()), textFormat_, toD2D1RectF(rect), colorBrush_, D2D1_DRAW_TEXT_OPTIONS_CLIP);
        }

        virtual void drawText(const std::wstring& text, const Storm::RectF& rect, const Storm::Font& font)
        {
            Windows::ComPtr<IDWriteTextFormat> textFormat;

            HRESULT hr = static_cast<Direct2DAdpater*>(GuiCore::gfxAdpater())->getDWriteFactory()->CreateTextFormat(
                font.font_.c_str(),
                NULL,
                font.bold_ ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
                font.italic_ ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                font.size_,
                L"",
                textFormat.resetAndGetPointerAddress()
                );

            if (SUCCEEDED(hr))
            {
                textFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

                Windows::ComPtr<IDWriteTextLayout> textLayout;

                hr = static_cast<Direct2DAdpater*>(GuiCore::gfxAdpater())->getDWriteFactory()->CreateTextLayout(
                    text.c_str(),
                    text.length(),
                    textFormat,
                    0,
                    0,
                    textLayout.resetAndGetPointerAddress());

                if (SUCCEEDED(hr))
                {
                    if (font.underline_)
                    {
                        DWRITE_TEXT_RANGE textRange;
                        textRange.startPosition = 0;
                        textRange.length = (int)text.length();
                        textLayout->SetUnderline(TRUE, textRange);
                    }

                    if (font.strikeline_)
                    {
                        DWRITE_TEXT_RANGE textRange;
                        textRange.startPosition = 0;
                        textRange.length = (int)text.length();
                        textLayout->SetStrikethrough(TRUE, textRange);
                    }

                    textLayout->SetMaxHeight((float)rect.height());
                    textLayout->SetMaxWidth((float)rect.width());

                    renderTarget()->DrawTextLayout(
                        toD2D1PointF(rect.topLeft()),
                        textLayout,
                        colorBrush_,
                        D2D1_DRAW_TEXT_OPTIONS_NO_SNAP | D2D1_DRAW_TEXT_OPTIONS_CLIP
                        );
                }
            }
        }

    protected:

        virtual void disposeResource()
        {
            colorBrush_ = nullptr;
            d2dTargetBitmap_ = nullptr;
            d2d1Context_ = nullptr;
            d2d1Device_ = nullptr;
            d2d1RenderTarget_ = nullptr;
        }

        virtual bool setupRenderTarget()
        {
            if (createD3D11Device())
            {
                if (getDXGIDevice())
                {
                    if (setupSwapChain())
                    {
#if USE_BITMAPTARGET
                        if (setupD2D1TargetBitmap())
#else
                        if (setupDXGISurfaceTarget())
#endif
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        bool setupD2D1TargetBitmap()
        {
            Windows::ComPtr<ID2D1Factory1> d2d1Factory1;
            static_cast<Direct2DAdpater*>(GuiCore::gfxAdpater())->getD2D1Factory().QueryInterface(d2d1Factory1);

            FLOAT dpiX;
            FLOAT dpiY;
            d2d1Factory1->GetDesktopDpi(&dpiX, &dpiY);


            Windows::ComPtr<IDXGISurface> dxgiBackBuffer;
            HRESULT hr = dxgiSwapChain_->GetBuffer(0, IID_PPV_ARGS(dxgiBackBuffer.resetAndGetPointerAddress()));
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            hr = d2d1Factory1->CreateDevice(dxgiDevice_, d2d1Device_.resetAndGetPointerAddress());
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            hr = d2d1Device_->CreateDeviceContext(
                D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
                d2d1Context_.resetAndGetPointerAddress()
                );
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
                dpiX, dpiY
                );

            hr = d2d1Context_->CreateBitmapFromDxgiSurface(
                dxgiBackBuffer,
                &bitmapProperties,
                d2dTargetBitmap_.resetAndGetPointerAddress()
                );

            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            d2d1Context_->SetTarget(d2dTargetBitmap_);
            d2d1Context_->SetDpi(dpiX, dpiY);
            return true;
        }

        bool setupDXGISurfaceTarget()
        {
            Windows::ComPtr<ID2D1Factory1> d2d1Factory1;
            static_cast<Direct2DAdpater*>(GuiCore::gfxAdpater())->getD2D1Factory().QueryInterface(d2d1Factory1);

            Windows::ComPtr<IDXGISurface> dxgiBackBuffer;
            HRESULT hr = dxgiSwapChain_->GetBuffer(0, IID_PPV_ARGS(dxgiBackBuffer.resetAndGetPointerAddress()));
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            FLOAT dpiX;
            FLOAT dpiY;
            d2d1Factory1->GetDesktopDpi(&dpiX, &dpiY);

            D2D1_RENDER_TARGET_PROPERTIES props =
                D2D1::RenderTargetProperties(
                    D2D1_RENDER_TARGET_TYPE_DEFAULT,
                    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
                    dpiX,dpiY
                    );

            hr = d2d1Factory1->CreateDxgiSurfaceRenderTarget(
                dxgiBackBuffer,
                &props,
                d2d1RenderTarget_.resetAndGetPointerAddress()
                );
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            return true;
        }


        Windows::ComPtr<ID3D11Device> createD3D11Device()
        {
            UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
            flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

            D3D_FEATURE_LEVEL featureLevels[] =
            {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1
            };

            HRESULT hr = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                flags,
                featureLevels,
                sizeof(featureLevels) / sizeof(*featureLevels),
                D3D11_SDK_VERSION,
                d3d11Device_.resetAndGetPointerAddress(),
                nullptr,
                nullptr);
            if (FAILED(hr))
                DAssert_M(false, "D3D11CreateDevice");
            return d3d11Device_;
        }

        bool setupSwapChain()
        {
            Windows::ComPtr<IDXGIAdapter> adapter;
            HRESULT hr = dxgiDevice_->GetAdapter(adapter.resetAndGetPointerAddress());
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            Windows::ComPtr<IDXGIFactory2> dxgiFactory;
            hr = adapter->GetParent(__uuidof(IDXGIFactory2), (void**)dxgiFactory.resetAndGetPointerAddress());
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }


            int width = window_->clientWidth();
            int height = window_->clientHeight();
            if (width <= 0) width = 1;
            if (height <= 0) height = 1;

            DXGI_SWAP_CHAIN_DESC1 swapDesc;
            ::ZeroMemory(&swapDesc, sizeof(swapDesc));
            swapDesc.Width = width;
            swapDesc.Height = height;
            swapDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // this is the most common swapchain format
            swapDesc.Stereo = false;
            swapDesc.SampleDesc.Count = 1;                // don't use multi-sampling
            swapDesc.SampleDesc.Quality = 0;
            swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapDesc.BufferCount = 2;
            swapDesc.Scaling = DXGI_SCALING_NONE;
            swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL ;
            swapDesc.Flags = 0;

            hr = dxgiFactory->CreateSwapChainForHwnd(d3d11Device_, window_->windowId(), &swapDesc, nullptr, nullptr, dxgiSwapChain_.resetAndGetPointerAddress());
            if (!SUCCEEDED(hr)) return 0;
            if (FAILED(hr)) {
                DAssert(0);
                return false;
            }

            return true;
        }

        bool resizeSwapChain(const Storm::SizeI& sz)
        {
            int width = sz.width();
            int height = sz.height();
            if (width <= 0) width = 1;
            if (height <= 0) height = 1;
            dxgiSwapChain_->ResizeBuffers(0, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
            return true;
        }

        Windows::ComPtr<IDXGIDevice> getDXGIDevice()
        {
            HRESULT hr = d3d11Device_.QueryInterface(dxgiDevice_);
            if (FAILED(hr))
                DAssert_M(false, "GetDXGIDevice");
            return dxgiDevice_;
        }

        virtual bool setupDirect2DResource()
        {
            if (FAILED(renderTarget()->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::Black)),
                colorBrush_.resetAndGetPointerAddress()
                )))
            {
                DAssert(0);
                return false;
            }

            return true;
        }

        bool setupFont()
        {
            if (FAILED(static_cast<Direct2DAdpater*>(GuiCore::gfxAdpater())->getDWriteFactory()->CreateTextFormat(
                L"Microsoft YaHei UI",
                NULL,
                DWRITE_FONT_WEIGHT_NORMAL,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                16.0f,
                L"",
                textFormat_.resetAndGetPointerAddress()
                )))
            {
                DAssert(0);
                return false;
            }
            return true;
        }

    protected:

        Window* window_;

        Windows::ComPtr<ID3D11Device> d3d11Device_;
        Windows::ComPtr<IDXGIDevice> dxgiDevice_;
        Windows::ComPtr<IDXGISwapChain1> dxgiSwapChain_;
        Windows::ComPtr<ID2D1RenderTarget> d2d1RenderTarget_;
        Windows::ComPtr<ID2D1Device>  d2d1Device_;
        Windows::ComPtr<ID2D1DeviceContext> d2d1Context_;
        Windows::ComPtr<ID2D1Bitmap1> d2dTargetBitmap_;
        Windows::ComPtr<ID2D1SolidColorBrush> colorBrush_;
        Windows::ComPtr<IDWriteTextFormat> textFormat_;
    };

    class Gfx2DRendererD2DHWND : public Gfx2DRendererD2D
    {
    public:
        Gfx2DRendererD2DHWND(Window* window)
            : Gfx2DRendererD2D(window)
        {

        }

        ~Gfx2DRendererD2DHWND() { ; }

        virtual bool setupRenderTarget()
        {
            if (FAILED(static_cast<Direct2DAdpater*>(GuiCore::gfxAdpater())->getD2D1Factory()->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties((HWND)(window_->windowId()), toD2D1SizeU(window_->clientRect().size()), D2D1_PRESENT_OPTIONS_IMMEDIATELY),
                m_renderTarget.resetAndGetPointerAddress()
                )))
            {
                return false;
            }

            return true;
        }

        virtual bool begin()
        {
            if (!renderTarget())
            {
                return false;
            }

            renderTarget()->BeginDraw();
            return true;
        }

        virtual void end()
        {
            renderTarget()->EndDraw();
        }

        virtual Windows::ComPtr<ID2D1RenderTarget> renderTarget()
        {
            return Windows::ComPtr<ID2D1RenderTarget>(m_renderTarget.get());
        }

        virtual void resize(const Storm::SizeI& sz)
        {
            m_renderTarget->Resize(toD2D1SizeU(sz));
        }

        virtual void disposeResource()
        {
            __super::disposeResource();
            m_renderTarget = nullptr;
        }

    protected:
        Windows::ComPtr<ID2D1HwndRenderTarget> m_renderTarget;

    };
}
