#pragma once

#include <memory>

namespace Graphics
{
    class Color;
    class ICamera2d;

    class IScene
    {
    public:
        virtual ~IScene() = default;

        virtual void ClearColor(const Color &color) = 0;
        virtual ICamera2d* Camera2d() = 0;
        virtual void Render() = 0;
    };
}
