#pragma once

#include <any>
#include <functional>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

namespace JkEng::Physics
{
    class IReadOnlyAabb2d
    {
    public:
        typedef std::function<void(const IReadOnlyAabb2d&)> CollisionHandler;

        virtual ~IReadOnlyAabb2d() = default;
        virtual glm::vec2 Size() const = 0;
        virtual const glm::vec2& Position() const = 0;
        virtual const glm::vec2& Velocity() const = 0;
        virtual const glm::vec2& Acceleration() const = 0;
        virtual const std::any& ObjectInfo() const = 0;

        template<typename T>
        inline const T& ObjectInfoAs() const
        {
            return std::any_cast<const T&>(ObjectInfo());
        }
    };
}