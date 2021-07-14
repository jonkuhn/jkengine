#pragma once

namespace Graphics
{
    class IWindow
    {
    public:
        virtual ~IWindow() = default;
        virtual bool Update() = 0;
    };
}
