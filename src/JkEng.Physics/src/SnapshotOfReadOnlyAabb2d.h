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
              _size(aabb.Size()),
              _objectInfo(aabb.ObjectInfo())
        {

        }

        glm::vec2 Position() const override
        {
            return _position;
        }

        glm::vec2 Size() const override
        {
            return _size;
        }

        //glm::vec2 Velocity() const override
        //glm::vec2 Acceleration() const override

        const std::any& ObjectInfo() const override
        {
            return _objectInfo;
        }

    private:
        glm::vec2 _position;
        glm::vec2 _size;
        std::any _objectInfo;
    };
}