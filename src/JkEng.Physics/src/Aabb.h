#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IMovableAabb2d.h"

namespace JkEng::Physics
{
    class Aabb final : public IMovableAabb2d
    {
    public:

        Aabb(const glm::vec2& position,
            const glm::vec2& size,
            IReadOnlyAabb2d::CollisionHandler collisionHandler,
            std::any objectInfo)
          : _leftXMin(position.x),
            _rightXMax(position.x + size.x),
            _bottomYMin(position.y),
            _topYMax(position.y + size.y),
            _collisionHandler(collisionHandler),
            _objectInfo(std::move(objectInfo))
        {

        }

        Aabb(float leftXMin,
            float rightXMax,
            float bottomYMin,
            float topYMax,
            IReadOnlyAabb2d::CollisionHandler collisionHandler,
            std::any objectInfo)
          : _leftXMin(leftXMin),
            _rightXMax(rightXMax),
            _bottomYMin(bottomYMin),
            _topYMax(topYMax),
            _collisionHandler(collisionHandler),
            _objectInfo(objectInfo)
        {

        }

        Aabb(const Aabb&) = delete;
        Aabb& operator=(const Aabb&) = delete;

        Aabb(Aabb&&) = default;
        Aabb& operator=(Aabb&&) = default;

        inline bool IsColliding(const Aabb &other) const
        {
            return _leftXMin <= other._rightXMax && _rightXMax >= other._leftXMin
                && _bottomYMin <= other._topYMax && _topYMax >= other._bottomYMin;
        }

        inline IReadOnlyAabb2d::CollisionHandler CollisionHandler() const
        {
            return _collisionHandler;
        }

        inline float LeftXMin() const { return _leftXMin; }
        inline float RightXMax() const { return _rightXMax; }
        inline float BottomYMin() const { return _bottomYMin; }
        inline float TopYMax() const { return _topYMax; }

        virtual glm::vec2 Size() const { return glm::vec2(_rightXMax - _leftXMin, _topYMax - _bottomYMin); }
        virtual glm::vec2 Position() const { return glm::vec2(_leftXMin, _bottomYMin); }

        virtual const std::any& ObjectInfo() const { return _objectInfo; }
        virtual std::any& ObjectInfo() { return _objectInfo; }

        virtual void Size(const glm::vec2& size)
        {
            SetPositionAndSize(Position(), size);
        }

        virtual void Position(const glm::vec2& position)
        {
            SetPositionAndSize(position, Size());
        }

    private:
        // Performance Note:  By moving _collisionHandler and _objectInfo
        // to a separate buffer the Aabb object ends up being 32 bytes
        // instead of 112 bytes (when building with clang on my mid-2014
        // Intel MacBook Pro anyway).  The time to check collisions for 1000
        // non-overlapping Aabbs was ~3% faster, but it was the same or
        // slower for 1000 Aabbs with 1998 collisions.

        float _leftXMin;
        float _rightXMax;
        float _bottomYMin;
        float _topYMax;
        IReadOnlyAabb2d::CollisionHandler _collisionHandler;
        std::any _objectInfo;

        void SetPositionAndSize(const glm::vec2& position, const glm::vec2& size);
    };
}
