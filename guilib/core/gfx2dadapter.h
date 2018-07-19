#pragma once
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <d3d11_2.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <Dwrite.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"Dwrite.lib")


namespace Storm
{
    class Direct2DAdpater : public Gfx2DAdapter
    {
    public:
        ~Direct2DAdpater();

        virtual bool setup();

        virtual GuiCore::Gfx2DRenderType type() const;

        virtual std::shared_ptr<Gfx2DRenderer> createWindow2DRenderer(Window* window);

        Windows::ComPtr<ID2D1Factory> getD2D1Factory() const;

        Windows::ComPtr<IDWriteFactory> getDWriteFactory() const;

    private:

        bool setupFactory();

    private:
        Windows::ComPtr<ID2D1Factory> direct2DFactory_;
        Windows::ComPtr<IDWriteFactory> directWriteFactory_;
    };


    class Direct2DHWNDAdpater : public Direct2DAdpater
    {
    public:
        ~Direct2DHWNDAdpater();

        virtual GuiCore::Gfx2DRenderType type() const;

        virtual std::shared_ptr<Gfx2DRenderer> createWindow2DRenderer(Window* window);

    };
}