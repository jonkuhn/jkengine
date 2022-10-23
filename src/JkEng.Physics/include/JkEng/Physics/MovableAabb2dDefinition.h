#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include <JkEng/AfterCreatePtr.h>

#include "IReadOnlyAabb2d.h"

namespace JkEng::Physics
{
    class IMovableAabb2d;

    class MovableAabb2dDefinition final
    {
    public:
        MovableAabb2dDefinition(
            AfterCreatePtr<IMovableAabb2d>* aabbAfterCreate,
            glm::vec2 position,
            glm::vec2 size,
            IReadOnlyAabb2d::CollisionHandler collisionHandler,
            std::any objectInfo)

          : _aabbAfterCreate(aabbAfterCreate),
            _position(std::move(position)),
            _size(std::move(size)),
            _collisionHandler(
                collisionHandler != nullptr
                    ? std::move(collisionHandler)
                    : [](const IReadOnlyAabb2d&){}),
            _objectInfo(std::move(objectInfo))
        {

        }
        
        inline const glm::vec2& Position() const
        {
            return _position;
        }

        inline const glm::vec2& Size() const
        {
            return _size;
        }

        inline const IReadOnlyAabb2d::CollisionHandler& CollisionHandler() const
        {
            return _collisionHandler;
        }

        inline const std::any& ObjectInfo() const
        {
            return _objectInfo;
        }

        inline void SetAfterCreatePtr(IMovableAabb2d* movableAabb) const
        {
            if (_aabbAfterCreate != nullptr)
            {
                _aabbAfterCreate->Initialize(movableAabb);
            }
        }

    private:
        AfterCreatePtr<IMovableAabb2d>* _aabbAfterCreate;
        glm::vec2 _position;
        glm::vec2 _size;
        IReadOnlyAabb2d::CollisionHandler _collisionHandler;
        std::any _objectInfo;
    };
}
