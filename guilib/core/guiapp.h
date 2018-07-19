#pragma once

namespace Storm {

    class GuiApp : public App
    {
    public:
        GuiApp()
        {
            Storm::GuiCore::setupGfxAdpater(Storm::GuiCore::Direct2DRenderer);
        }

        ~GuiApp()
        {

        }

        static GuiApp* instance();

    protected:
        virtual void onQuit()
        {
            __super::onQuit();
            Storm::GuiCore::releaseGfxAdapter();
        }

    };

    inline Storm::GuiApp* GuiApp::instance()
    {
        return static_cast<GuiApp*>(App::instance());
    }

}
