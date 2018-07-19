#pragma once

namespace Storm
{
    class GuiWindow;
    class Gfx2DRenderer;
    class Gfx2DAdapter;

    class GuiCore
    {
    public:
        enum Gfx2DRenderType
        {
            None,
            Direct2DRenderer,
            Direct2DRendererHWND,
            Direct3D11Renderer,
        };

        static void addGuiWindow(Storm::Window* window, GuiWindow* guiWindow);
        static void removeGuiWindow(Storm::Window* window);

        static void setupGfxAdpater(Gfx2DRenderType);
        static void releaseGfxAdapter();
        static Gfx2DAdapter* gfxAdpater();

    private:
        static Storm::TinyMap<Window*, GuiWindow*> windowMap_;

        static std::unique_ptr<Gfx2DAdapter> gfxAdpater_;
    };


    class Gfx2DAdapter
    {
    public:
        virtual ~Gfx2DAdapter() { ; }
        virtual bool setup() = 0;
        virtual GuiCore::Gfx2DRenderType type() const= 0;
        virtual std::shared_ptr<Gfx2DRenderer> createWindow2DRenderer(Window* window) = 0;
    };

}