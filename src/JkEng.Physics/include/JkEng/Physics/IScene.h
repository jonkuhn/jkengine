#pragma once

namespace JkEng::Physics
{
    class IScene
    {
    public:
        static constexpr float StepTime = 1.0f / 60.0f;
        virtual ~IScene() = default;
        virtual void Update(float deltaTime) = 0;
        virtual float TimeNotYetSimulated() = 0;
    };
}
