#pragma 

namespace Storm
{
    namespace RGBA_
    {
        static const uint32_t sc_redShift = 24;
        static const uint32_t sc_greenShift = 16;
        static const uint32_t sc_blueShift = 8;
        static const uint32_t sc_alphaShitf = 0;

        static const uint32_t sc_redMask = 0xff << sc_redShift;
        static const uint32_t sc_greenMask = 0xff << sc_greenShift;
        static const uint32_t sc_blueMask = 0xff << sc_blueShift;
        static const uint32_t sc_alphaMask = 0xff << sc_alphaShitf;
    };

    namespace BGRA_
    {
        static const uint32_t sc_redShift = 8;
        static const uint32_t sc_greenShift = 16;
        static const uint32_t sc_blueShift = 24;
        static const uint32_t sc_alphaShitf = 0;

        static const uint32_t sc_redMask = 0xff << sc_redShift;
        static const uint32_t sc_greenMask = 0xff << sc_greenShift;
        static const uint32_t sc_blueMask = 0xff << sc_blueShift;
        static const uint32_t sc_alphaMask = 0xff << sc_alphaShitf;
    };

    namespace ARGB_
    {
        static const uint32_t sc_redShift = 16;
        static const uint32_t sc_greenShift = 8;
        static const uint32_t sc_blueShift = 0;
        static const uint32_t sc_alphaShitf = 24;

        static const uint32_t sc_redMask = 0xff << sc_redShift;
        static const uint32_t sc_greenMask = 0xff << sc_greenShift;
        static const uint32_t sc_blueMask = 0xff << sc_blueShift;
        static const uint32_t sc_alphaMask = 0xff << sc_alphaShitf;
    };

    namespace ABGR_
    {
        static const uint32_t sc_redShift = 0;
        static const uint32_t sc_greenShift = 8;
        static const uint32_t sc_blueShift = 16;
        static const uint32_t sc_alphaShitf = 24;

        static const uint32_t sc_redMask = 0xff << sc_redShift;
        static const uint32_t sc_greenMask = 0xff << sc_greenShift;
        static const uint32_t sc_blueMask = 0xff << sc_blueShift;
        static const uint32_t sc_alphaMask = 0xff << sc_alphaShitf;
    };

    class Color
    {

    public:
        Color()
        {

        }

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : red_(r), green_(g), blue_(b), alpha_(a)
        {

        }

        static Color fromRGBA(unsigned int rgba)
        {
            Color color;
            color.setColorRGBA(rgba);
            return color;
        }

        static Color fromBGRA(unsigned int bgra)
        {
            Color color;
            color.setColorBGRA(bgra);
            return color;
        }

        static Color fromARGB(unsigned int argb)
        {
            Color color;
            color.setColorARGB(argb);
            return color;
        }

        static Color fromABGR(unsigned int abgr)
        {
            Color color;
            color.setColorABGR(abgr);
            return color;
        }

        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        {
            red_ = r; green_ = g, blue_ = b; alpha_ = a;
        }

        void setColorRGBA(unsigned int rgba)
        {
            red_   = static_cast<uint8_t>((rgba & RGBA_::sc_redMask)   >> RGBA_::sc_redShift);
            green_ = static_cast<uint8_t>((rgba & RGBA_::sc_greenMask) >> RGBA_::sc_greenShift);
            blue_  = static_cast<uint8_t>((rgba & RGBA_::sc_blueMask)  >> RGBA_::sc_blueShift);
            alpha_ = static_cast<uint8_t>((rgba & RGBA_::sc_alphaMask) >> RGBA_::sc_alphaShitf);
        }

        void setColorBGRA(unsigned int bgra)
        {
            red_   = static_cast<uint8_t>((bgra & BGRA_::sc_redMask)   >> BGRA_::sc_redShift);
            green_ = static_cast<uint8_t>((bgra & BGRA_::sc_greenMask) >> BGRA_::sc_greenShift);
            blue_  = static_cast<uint8_t>((bgra & BGRA_::sc_blueMask)  >> BGRA_::sc_blueShift);
            alpha_ = static_cast<uint8_t>((bgra & BGRA_::sc_alphaMask) >> BGRA_::sc_alphaShitf);
        }

        void setColorARGB(unsigned int argb)
        {
            red_   = static_cast<uint8_t>((argb & ARGB_::sc_redMask)   >> ARGB_::sc_redShift);
            green_ = static_cast<uint8_t>((argb & ARGB_::sc_greenMask) >> ARGB_::sc_greenShift);
            blue_  = static_cast<uint8_t>((argb & ARGB_::sc_blueMask)  >> ARGB_::sc_blueShift);
            alpha_ = static_cast<uint8_t>((argb & ARGB_::sc_alphaMask) >> ARGB_::sc_alphaShitf);
        }

        void setColorABGR(unsigned int abgr)
        {
            red_   = static_cast<uint8_t>((abgr & ABGR_::sc_redMask)  >> ABGR_::sc_redShift);
            green_ = static_cast<uint8_t>((abgr & ABGR_::sc_greenMask)>> ABGR_::sc_greenShift);
            blue_  = static_cast<uint8_t>((abgr & ABGR_::sc_blueMask) >> ABGR_::sc_blueShift);
            alpha_ = static_cast<uint8_t>((abgr & ABGR_::sc_alphaMask)>> ABGR_::sc_alphaShitf);
        }

        uint32_t toRGBA() const
        {
            return (red() << 24) | (green() << 16) | (blue() << 8) | (alpha());
        }

        uint32_t toBGRA() const
        {
            return (blue() << 24) | (green() << 16) | (red() << 8) | (alpha());
        }

        uint32_t toARGB() const
        {
            return (alpha() << 24) | (red() << 16) | (green() << 8) | (blue());
        }

        uint32_t toABGR() const
        {
            return (alpha() << 24) | (blue() << 16) | (green() << 8) | (red());
        }

        void setRed(uint8_t r) { red_ = r; }
        uint8_t red() const { return red_; }

        void setGreen(uint8_t g) { green_ = g; }
        uint8_t green() const { return green_; }

        void setBlue(uint8_t b) { blue_ = b; }
        uint8_t blue() const { return blue_; }

        void setAlpha(uint8_t a) { alpha_ = a; }
        uint8_t alpha() const { return alpha_; }

        bool operator == (const Color& color) const
        {
            return red_   == color.red_
                && green_ == color.green_
                && blue_  == color.blue_
                && alpha_ == color.alpha_;
        }

        bool operator != (const Color& color) const
        {
            return red_   != color.red_
                || green_ != color.green_
                || blue_  != color.blue_
                || alpha_ != color.alpha_;
        }

    private:
        uint8_t red_ = 0;
        uint8_t green_ = 0;
        uint8_t blue_ = 0;
        uint8_t alpha_ = 0;
    };

}