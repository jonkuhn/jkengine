#pragma once

#include <memory>

#include "IScene.h"
#include "IScreenshot.h"

namespace JkEng::Graphics
{
    class SceneDefinition;

    class IEngine
    {
    public:
        virtual ~IEngine() = default;

        virtual std::unique_ptr<IScene> CreateScene(const SceneDefinition& definition) = 0;

        virtual std::unique_ptr<IScreenshot> TakeScreenshot() = 0;
    };
}
