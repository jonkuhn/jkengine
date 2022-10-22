#pragma once

#include <memory>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include "IMovableAabb2d.h"

#define SEPARATE_COLLISION_INFO

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
#ifndef SEPARATE_COLLISION_INFO
            _collisionHandler(std::move(collisionHandler)),
            _objectInfo(std::move(objectInfo))
#else
            _collisionInfo(std::make_unique<CollisionInfo>(
                std::move(collisionHandler),
                std::move(objectInfo)
            ))
#endif
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
#ifndef SEPARATE_COLLISION_INFO
            _collisionHandler(std::move(collisionHandler)),
            _objectInfo(std::move(objectInfo))
#else
            _collisionInfo(std::make_unique<CollisionInfo>(
                std::move(collisionHandler),
                std::move(objectInfo)
            ))
#endif
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

#ifndef SEPARATE_COLLISION_INFO
        inline IReadOnlyAabb2d::CollisionHandler CollisionHandler() const
        {
            return _collisionHandler;
        }
#else
        inline IReadOnlyAabb2d::CollisionHandler CollisionHandler() const
        {
            return _collisionInfo->_collisionHandler;
        }
#endif

        inline float LeftXMin() const { return _leftXMin; }
        inline float RightXMax() const { return _rightXMax; }
        inline float BottomYMin() const { return _bottomYMin; }
        inline float TopYMax() const { return _topYMax; }

        virtual glm::vec2 Size() const { return glm::vec2(_rightXMax - _leftXMin, _topYMax - _bottomYMin); }
        virtual glm::vec2 Position() const { return glm::vec2(_leftXMin, _bottomYMin); }

#ifndef SEPARATE_COLLISION_INFO
        virtual const std::any& ObjectInfo() const { return _objectInfo; }
        virtual std::any& ObjectInfo() { return _objectInfo; }
#else
        virtual const std::any& ObjectInfo() const { return _collisionInfo->_objectInfo; }
        virtual std::any& ObjectInfo() { return _collisionInfo->_objectInfo; }
#endif

        virtual void Size(const glm::vec2& size)
        {
            SetPositionAndSize(Position(), size);
        }

        virtual void Position(const glm::vec2& position)
        {
            SetPositionAndSize(position, Size());
        }

    private:
        float _leftXMin;
        float _rightXMax;
        float _bottomYMin;
        float _topYMax;

#ifndef SEPARATE_COLLISION_INFO
        IReadOnlyAabb2d::CollisionHandler _collisionHandler;
        std::any _objectInfo;
#else
        // TODO: Not sure if this is good or not but it reduces the
        // size of Aabb significantly at the expense of a separate heap
        // allocation for the collision info
        class CollisionInfo
        {
        public:
            CollisionInfo(
                IReadOnlyAabb2d::CollisionHandler&& collisionHandler,
                std::any&& objectInfo)

              : _collisionHandler(std::move(collisionHandler)),
                _objectInfo(std::move(objectInfo))
            {

            }
            IReadOnlyAabb2d::CollisionHandler _collisionHandler;
            std::any _objectInfo;
        };

        std::unique_ptr<CollisionInfo> _collisionInfo;
#endif


        void SetPositionAndSize(const glm::vec2& position, const glm::vec2& size);
    };
}

#undef SEPARATE_COLLISION_INFO
