#pragma once

namespace Storm
{
    class GuiWindow : public CoreObject
    {
        Q_LEAK_DETECTOR(GuiWindow)
    public:
        explicit GuiWindow(Storm::Window* window)
            : window_(window)
        {
            setup();
        }

        GuiWindow()
            : window_(new Storm::Window())
            , owned_(true)
        {
            window_->create();
            setup();
        }

        ~GuiWindow()
        {
            dispose();
        }

        void show()
        {
            if (!window_->windowId())
            {
                window_->create();
                setup();
            }
            window_->show();
        }

        void close()
        {
            window_->close();
        }

        void render()
        {
            renderFrame();
        }

    protected:
        virtual void onWindowVisibleChanged(Window*, bool)
        {

        }

        virtual void onWindowDestroyed(Window* )
        {
            GuiCore::removeGuiWindow(window_);
        }

        virtual void onWindowResized(Window*, const Storm::SizeI& sz)
        {
            if (gfx2DRenderer_)
            {
                gfx2DRenderer_->resize(sz);
            }
        }

        void dispose()
        {
            gfx2DRenderer_ = nullptr;

            if (window_)
            {
                if (owned_)
                {
                    delete window_;
                }

                window_ = nullptr;
            }
        }
    private:
        void setup()
        {
            setupGfx2D();
            setupEventCallback();
            GuiCore::addGuiWindow(window_, this);
            fpsTimer_.start();
        }

        bool setupGfx2D()
        {
            gfx2DRenderer_ = GuiCore::gfxAdpater()->createWindow2DRenderer(window_);
            return !!gfx2DRenderer_;
        }

        bool setupEventCallback()
        {
            DAssert(window_);
            window_->showHideEvent().add(delegate(&GuiWindow::onWindowVisibleChanged, this));
            window_->destroyEvent().add(delegate(&GuiWindow::onWindowDestroyed, this));
            window_->resizeEvent().add(delegate(&GuiWindow::onWindowResized, this));

            return true;
        }

        void renderFrame()
        {
            if (!window_)
            {
                return;
            }

            fpsTimer_.tick();

            window_->setTitle(std::to_wstring(fpsTimer_.fps()));

            if (gfx2DRenderer_)
            {
                if (gfx2DRenderer_->begin())
                {
                    static unsigned int xx = 0xff;
                    xx += 0x01010100;

                    gfx2DRenderer_->clear(Storm::Color::fromBGRA(xx));

                    gfx2DRenderer_->drawText(std::to_wstring(fpsTimer_.fps()), RectF(10, 10, 210, 110));

                    gfx2DRenderer_->end();
                }
            }
        }
    private:
        Storm::Window* window_ = nullptr;
        bool owned_ = false;

        std::shared_ptr<Gfx2DRenderer> gfx2DRenderer_;
        FpsTimer fpsTimer_;
    };
}