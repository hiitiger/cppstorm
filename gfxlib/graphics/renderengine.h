#pragma once

namespace Storm
{
    class RenderEngine
    {
    public:
        RenderEngine();

        ~RenderEngine();

        void setup();

        void clear();

        void commit();

    };
}