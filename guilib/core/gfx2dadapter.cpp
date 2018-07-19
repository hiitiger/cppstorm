#include "stable.h"
#include "renderer_d2d.h"
#include "gfx2dadapter.h"

namespace Storm
{
    Direct2DAdpater::~Direct2DAdpater()
    {
        direct2DFactory_ = nullptr;
        directWriteFactory_ = nullptr;
    }

    bool Direct2DAdpater::setup()
    {
        return setupFactory();
    }

    GuiCore::Gfx2DRenderType Direct2DAdpater::type() const
    {
        return GuiCore::Direct2DRenderer;
    }

    std::shared_ptr<Gfx2DRenderer> Direct2DAdpater::createWindow2DRenderer(Window* window)
    {
        auto gfx = std::make_shared<Gfx2DRendererD2D>(window);
        if (gfx->setup())
        {
            return gfx;
        }
        else
            return nullptr;
    }

    Windows::ComPtr<ID2D1Factory> Direct2DAdpater::getD2D1Factory() const
    {
        return direct2DFactory_;
    }

    Windows::ComPtr<IDWriteFactory> Direct2DAdpater::getDWriteFactory() const
    {
        return directWriteFactory_;
    }


    bool Direct2DAdpater::setupFactory()
    {
        if (!direct2DFactory_)
        {
            if (FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED,
                direct2DFactory_.resetAndGetPointerAddress()
                )))
            {
                return false;
            }
        }

        if (!directWriteFactory_)
        {
            if (FAILED(DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_ISOLATED,
                __uuidof(directWriteFactory_),
                reinterpret_cast<IUnknown **>(directWriteFactory_.resetAndGetPointerAddress())
                )))
            {
                return false;
            }
        }
        return true;
    }


    //////////////////////////////////////////////////////////////////////////

    Direct2DHWNDAdpater::~Direct2DHWNDAdpater()
    {

    }

    GuiCore::Gfx2DRenderType Direct2DHWNDAdpater::type() const
    {
        return GuiCore::Direct2DRendererHWND;
    }
    
    std::shared_ptr<Gfx2DRenderer> Direct2DHWNDAdpater::createWindow2DRenderer(Window* window)
    {
        auto gfx = std::make_shared<Gfx2DRendererD2DHWND>(window);
        if (gfx->setup())
        {
            return gfx;
        }
        else
            return nullptr;
    }
}