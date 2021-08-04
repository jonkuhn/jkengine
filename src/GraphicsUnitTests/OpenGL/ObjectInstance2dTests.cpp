#include <gtest/gtest.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include "Graphics/OpenGL/ObjectInstance2d.h"
#include "../TestHelpers.h"

using namespace testing;
using namespace Graphics::OpenGL;

class ObjectInstance2dTests : public Test
{
protected:
    Registry<ObjectInstance2d> _instanceRegistry;

    ObjectInstance2d CreateObjectInstance2d()
    {
        return ObjectInstance2d(&_instanceRegistry);
    }
};

TEST_F(ObjectInstance2dTests, ConstructorDestructor_VerifyRegistration)
{
    {
        ObjectInstance2d obj(&_instanceRegistry);

        // exactly one instance should be registered (the one we just created)
        int count = 0;
        for(auto* instance : _instanceRegistry)
        {
            count++;
            EXPECT_EQ(instance, &obj);
        }
        EXPECT_EQ(count, 1);
    }

    // After it is destroyed there should be no registered instances
    EXPECT_EQ(_instanceRegistry.begin(), _instanceRegistry.end());
}

TEST_F(ObjectInstance2dTests, Constructor_ModelMatrixHasPositionAtOriginRotation0And1by1Size)
{
    auto obj = CreateObjectInstance2d();

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    expected = glm::translate(expected, glm::vec3(0.0f, 0.0f, 0.0f));
    expected = glm::rotate(expected, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));  
    expected = glm::scale(expected, glm::vec3(1.0f, 1.0f, 0.0f));  
    ExpectEqual(actual, expected);
}

TEST_F(ObjectInstance2dTests, ConstructorDestructor_NullRegistry)
{
    {
        ObjectInstance2d obj(nullptr);
    }
}

TEST_F(ObjectInstance2dTests, Position_GivenPositionChangeAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedPosition)
{
    glm::vec2 testPosition(128.5f, 64.125f);
    auto obj = CreateObjectInstance2d();
    // fetch to ensure model matrix would be cached with default values
    obj.ModelMatrix();

    obj.Position(testPosition);

    auto actual = obj.ModelMatrix();
    auto expected = glm::mat4(1.0f);
    expected = glm::translate(expected, glm::vec3(testPosition, 0.0f));
    expected = glm::rotate(expected, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));  
    expected = glm::scale(expected, glm::vec3(1.0f, 1.0f, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(ObjectInstance2dTests, Size_GivenSizeChangedAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedSize)
{
    glm::vec2 testSize(12.25, 6.5);
    auto obj = CreateObjectInstance2d();
    // fetch to ensure model matrix would be cached with default values
    obj.ModelMatrix();

    obj.Size(testSize);

    auto actual = obj.ModelMatrix();
    auto expected = glm::mat4(1.0f);
    expected = glm::translate(expected, glm::vec3(0.0f, 0.0f, 0.0f));
    expected = glm::rotate(expected, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));  
    expected = glm::scale(expected, glm::vec3(testSize.x, testSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(ObjectInstance2dTests, Rotation_GivenRotationChangedAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedRotation)
{
    float testRotationDegrees = 12.5;
    auto obj = CreateObjectInstance2d();
    // fetch to ensure model matrix would be cached with default values
    obj.ModelMatrix();

    obj.Rotation(testRotationDegrees);

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    glm::vec2 expectedSize(1.0f, 1.0f);
    expected = glm::translate(expected, glm::vec3(0.0f, 0.0f, 0.0f));
    expected = glm::translate(expected, glm::vec3(0.5 * expectedSize.x, 0.5 * expectedSize.y, 0.0f));  
    expected = glm::rotate(expected, glm::radians(testRotationDegrees), glm::vec3(0.0f, 0.0f, -1.0f));  
    expected = glm::translate(expected, glm::vec3(-0.5 * expectedSize.x, -0.5 * expectedSize.y, 0.0f));  
    expected = glm::scale(expected, glm::vec3(expectedSize, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(ObjectInstance2dTests, ModelMatrix_GivenNonZeroPositionScaleAndRotation_ModelMatrixIsCorrect)
{
    glm::vec2 testPosition(128.5f, 64.125f);
    glm::vec2 testSize(12.25, 6.5);
    float testRotationDegrees = 12.5;
    auto obj = CreateObjectInstance2d();
    // fetch to ensure model matrix would be cached with default values
    obj.ModelMatrix();

    obj.Rotation(testRotationDegrees);
    obj.Position(testPosition);
    obj.Size(testSize);

    auto actual = obj.ModelMatrix();
    auto expected = glm::mat4(1.0f);
    expected = glm::translate(expected, glm::vec3(testPosition, 0.0f));
    expected = glm::translate(expected, glm::vec3(0.5 * testSize.x, 0.5 * testSize.y, 0.0f));  
    expected = glm::rotate(expected, glm::radians(testRotationDegrees), glm::vec3(0.0f, 0.0f, -1.0f));  
    expected = glm::translate(expected, glm::vec3(-0.5 * testSize.x, -0.5 * testSize.y, 0.0f));  
    expected = glm::scale(expected, glm::vec3(testSize.x, testSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}
