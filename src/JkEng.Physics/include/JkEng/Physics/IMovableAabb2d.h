#pragma once

#include <any>

#include "IReadOnlyAabb2d.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

namespace JkEng::Physics
{
    class IMovableAabb2d : public IReadOnlyAabb2d
    {
    public:
        virtual ~IMovableAabb2d() = default;

        using IReadOnlyAabb2d::Size;
        using IReadOnlyAabb2d::Position;
        //using IReadOnlyAabb2d::Velocity;
        //using IReadOnlyAabb2d::Acceleration;
        using IReadOnlyAabb2d::ObjectInfo;
        using IReadOnlyAabb2d::ObjectInfoAs;

        virtual void Size(const glm::vec2& size) = 0;
        virtual void Position(const glm::vec2& position) = 0;
        //virtual void Velocity(const glm::vec2& velocity) = 0;
        //virtual void Acceleration(const glm::vec2& acceleration) = 0;
        virtual std::any& ObjectInfo() = 0;

        template<typename T>
        inline T& ObjectInfoAs()
        {
            return std::any_cast<T&>(ObjectInfo());
        }
    };
}