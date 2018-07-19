#pragma once

namespace Storm {

    class GfxApp : public App
    {
    public:
        GfxApp()
        {

        }

        ~GfxApp()
        {

        }

        void setGfxWindow(Window* window)
        {
            DAssert(window && !gfxWindow_);
            gfxWindow_ = window;
        }

        Window* gfxWindow() const
        {
            return gfxWindow_;
        }

        void setupGfxRenderer()
        {
            renderer_.reset(new RenderEngine());
        }

    protected:

        virtual void onFrame()
        {
            __super::onFrame();

            renderer_->clear();
            renderer_->commit();
        }

        virtual void onIdleWait()
        {

        }

        virtual void onQuit()
        {

            __super::onQuit();
        }

    private:
        Window* gfxWindow_ = nullptr;
        std::unique_ptr<RenderEngine> renderer_;
    };

}
