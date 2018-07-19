#pragma once


namespace Storm
{
    class Font
    {
    public:
        std::wstring font_;
        float size_;
        bool bold_;
        bool italic_;
        bool underline_;
        bool strikeline_;

        Font(const std::wstring& font = L"", float size = 12.0f, bool bold = false, bool italic = false, bool underline = false, bool strikeline = false)
            : font_(font)
            , size_(size)
            , bold_(bold)
            , italic_(italic)
            , underline_(underline)
            , strikeline_(strikeline)
        {

        }

        int compare(const Font& other) const
        {
            int result = 0;
            do
            {
                result = font_.compare(other.font_);
                if (result != 0)
                    break;

                result = (int)(size_ - other.size_);
                if (result != 0)
                    break;

                result = (int)bold_ - (int)other.bold_;
                if (result != 0)
                    break;

                result = (int)italic_ - (int)other.italic_;
                if (result != 0)
                    break;

                result = (int)underline_ - (int)other.underline_;
                if (result != 0)
                    break;

                result = (int)strikeline_ - (int)other.strikeline_;
                if (result != 0)
                    break;

            } while (false);
        }

        bool operator == (const Font& other) const { return compare(other) == 0; }
        bool operator != (const Font& other) const { return compare(other) != 0; }
        bool operator <  (const Font& other) const { return compare(other) < 0; }
        bool operator <= (const Font& other) const { return compare(other) <= 0; }
        bool operator >  (const Font& other) const { return compare(other) > 0; }
        bool operator >= (const Font& other) const { return compare(other) >= 0; }
    };
}