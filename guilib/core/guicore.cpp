#include "stable.h"
#include "guicore.h"
#include "renderer_d2d.h"
#include "gfx2dadapter.h"
#include "guiapp.h"

namespace Storm
{
    Storm::TinyMap<Window*, GuiWindow*> GuiCore::windowMap_;

    std::unique_ptr<Storm::Gfx2DAdapter> GuiCore::gfxAdpater_;

    void GuiCore::addGuiWindow(Storm::Window* window, GuiWindow* guiWindow)
    {
        windowMap_.insert(window, guiWindow);
    }

    void GuiCore::removeGuiWindow(Storm::Window* window)
    {
        windowMap_.remove(window);
        if (windowMap_.size() == 0)
        {
            GuiApp::instance()->quit();
        }
    }

    void GuiCore::setupGfxAdpater(Gfx2DRenderType renderType)
    {
        if (renderType == Direct2DRenderer)
        {
            std::unique_ptr<Direct2DAdpater> gfx(new Direct2DAdpater);
            if (gfx->setup())
            {
                gfxAdpater_.reset(gfx.release());
            }
        }
        else if(renderType == Direct2DRendererHWND)
        {
            std::unique_ptr<Direct2DHWNDAdpater> gfx(new Direct2DHWNDAdpater);
            if (gfx->setup())
            {
                gfxAdpater_.reset(gfx.release());
            }
        }
    }

    void GuiCore::releaseGfxAdapter()
    {
        gfxAdpater_.reset(nullptr);
    }

    Storm::Gfx2DAdapter* GuiCore::gfxAdpater()
    {
        return gfxAdpater_.get();
    }

}
