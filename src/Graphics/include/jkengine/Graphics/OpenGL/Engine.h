#pragma once

#include <memory>

#include <jkengine/Window/IOpenGLWindow.h>

#include "../IEngine.h"
#include "Camera2d.h"
#include "OpenGLWrapper.h"

namespace Graphics::OpenGL
{
    class Engine : public IEngine
    {
    public:
        Engine(Window::IOpenGLWindow& window);

        // There is need to copy or move this class.
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

        virtual std::unique_ptr<IScene> CreateScene(const SceneDefinition& definition) override;

        std::unique_ptr<IScreenshot> TakeScreenshot() override;

    private:
        Window::IOpenGLWindow& _window;
        OpenGLWrapper _gl;
    };
}
