#pragma once
#include <gmock/gmock.h>

#include <JkEng/Graphics/ISprite.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
class MockSprite : public JkEng::Graphics::ISprite
{
public:
    MOCK_METHOD(void, Show, (bool), (override));
    MOCK_METHOD(bool, Show, (), (const, override));
    MOCK_METHOD(void, Position, (const glm::vec2&), (override));
    MOCK_METHOD(const glm::vec3&, Position, (), (const, override));
    MOCK_METHOD(void, Size, (glm::vec2), (override));
    MOCK_METHOD(const glm::vec2&, Size, (), (const, override));
    MOCK_METHOD(void, Rotation, (float), (override));
    MOCK_METHOD(float, Rotation, (), (const, override));
    MOCK_METHOD(void, Mirror, (bool, bool), (override));
    MOCK_METHOD(void, AtlasLocation, (JkEng::Graphics::GridLocation), (override));
    MOCK_METHOD(const JkEng::Graphics::GridLocation&, AtlasLocation, (), (const, override));
};
#pragma clang diagnostic pop
