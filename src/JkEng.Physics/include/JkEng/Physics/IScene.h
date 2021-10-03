#pragma once

namespace JkEng::Physics
{
    class IScene
    {
    public:
        virtual ~IScene() = default;
        virtual void Update() = 0;
    };
}
