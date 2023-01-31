#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IReadOnlyAabb2d.h"

namespace JkEng::Physics
{
    class SnapshotOfReadOnlyAabb2d final : public IReadOnlyAabb2d
    {
    public:
        SnapshotOfReadOnlyAabb2d(const IReadOnlyAabb2d& aabb)
            : _position(aabb.Position()),
              _previousPosition(aabb.PreviousPosition()),
              _size(aabb.Size()),
              _velocity(aabb.Velocity()),
              _acceleration(aabb.Acceleration()),
              _objectInfo(aabb.ObjectInfo())
        {

        }

        glm::vec2 Position() const override
        {
            return _position;
        }

        const glm::vec2& PreviousPosition() const override
        {
            return _previousPosition;
        }

        glm::vec2 Size() const override
        {
            return _size;
        }

        glm::vec2 Velocity() const override
        {
            return _velocity;
        }

        glm::vec2 Acceleration() const override
        {
            return _acceleration;
        }

        const std::any& ObjectInfo() const override
        {
            return _objectInfo;
        }

    private:
        glm::vec2 _position;
        glm::vec2 _previousPosition;
        glm::vec2 _size;
        glm::vec2 _velocity;
        glm::vec2 _acceleration;
        std::any _objectInfo;
    };
}