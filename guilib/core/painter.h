#pragma once

namespace Storm
{
    class Gfx2DRenderer
    {
    public:
        virtual ~Gfx2DRenderer() {}
        virtual bool begin() = 0;
        virtual void end() = 0;

        virtual void resize(const Storm::SizeI& sz) = 0;
        virtual void clear(const Storm::Color& color) = 0;

        virtual void setBrushColor(const Storm::Color& color) = 0;
        virtual Storm::Color brushColor() const= 0;

        virtual void drawRect(const Storm::RectF& rect) = 0;
        virtual void drawRect(const Storm::RectF& rect, const Storm::Color& color) = 0;
        virtual void fillRect(const Storm::RectF& rect) = 0;
        virtual void fillRect(const Storm::RectF& rect, const Storm::Color& color) = 0;

        virtual void drawImage() = 0;

        virtual void drawText(const std::wstring& text, const Storm::RectF& rect) = 0;
        virtual void drawText(const std::wstring& text, const Storm::RectF& rect, const Storm::Font& font)= 0;
    };
}
