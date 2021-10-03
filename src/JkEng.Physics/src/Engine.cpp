#include "Engine.h"

#include <memory>

#include "Scene.h"

using namespace JkEng::Physics;

std::unique_ptr<IScene> Engine::CreateScene(const SceneDefinition& definition)
{
    return std::make_unique<Scene>(definition);
}
