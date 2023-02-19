#include <gtest/gtest.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include <JkEng/Graphics/OpenGL/Object2d.h>
#include "../TestHelpers.h"

using namespace testing;
using namespace JkEng::Graphics::OpenGL;

class Object2dTests : public Test
{

};

TEST_F(Object2dTests, Constructor_ModelMatrixHasPositionAtOriginRotation0And1by1Size)
{
    Object2d obj;

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    expected = glm::translate(expected, glm::vec3(0.0f, 0.0f, 0.0f));
    expected = glm::rotate(expected, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));  
    expected = glm::scale(expected, glm::vec3(1.0f, 1.0f, 0.0f));  
    ExpectEqual(actual, expected);
}

TEST_F(Object2dTests, Position_GivenPositionChangeAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedPosition)
{
    glm::vec2 testPosition(128.5f, 64.125f);
    Object2d obj;
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

TEST_F(Object2dTests, Size_GivenSizeChangedAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedSize)
{
    glm::vec2 testSize(12.25, 6.5);
    Object2d obj;
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

TEST_F(Object2dTests, Rotation_GivenRotationChangedAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedRotation)
{
    float testRotationDegrees = 12.5;
    Object2d obj;
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

TEST_F(Object2dTests, Mirror_GivenMirrorChangedAfterFetchingModelMatrix_ModelMatrixMatchesUpdatedMirror)
{
    Object2d obj;
    // fetch to ensure model matrix would be cached with default values
    obj.ModelMatrix();

    obj.Mirror(true, true);

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    glm::vec2 expectedSize(1.0f, 1.0f);
    expected = glm::translate(expected, glm::vec3(expectedSize.x, expectedSize.y, 0.0f));  
    expected = glm::scale(expected, glm::vec3(-expectedSize.x, -expectedSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(Object2dTests, Mirror_GivenMirrorNothing_ModelMatrixMatchesExpectation)
{
    Object2d obj;

    obj.Mirror(false, false);

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    glm::vec2 expectedSize(1.0f, 1.0f);
    expected = glm::scale(expected, glm::vec3(expectedSize.x, expectedSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(Object2dTests, Mirror_GivenMirrorXOnly_ModelMatrixMatchesExpectation)
{
    Object2d obj;

    obj.Mirror(true, false);

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    glm::vec2 expectedSize(1.0f, 1.0f);
    expected = glm::translate(expected, glm::vec3(expectedSize.x, 0.0f, 0.0f));  
    expected = glm::scale(expected, glm::vec3(-expectedSize.x, expectedSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(Object2dTests, Mirror_GivenMirrorYOnly_ModelMatrixMatchesExpectation)
{
    Object2d obj;

    obj.Mirror(false, true);

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    glm::vec2 expectedSize(1.0f, 1.0f);
    expected = glm::translate(expected, glm::vec3(0.0f, expectedSize.y, 0.0f));  
    expected = glm::scale(expected, glm::vec3(expectedSize.x, -expectedSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(Object2dTests, Mirror_GivenMirrorXAndY_ModelMatrixMatchesExpectation)
{
    Object2d obj;

    obj.Mirror(true, true);

    auto actual = obj.ModelMatrix();

    auto expected = glm::mat4(1.0f);
    glm::vec2 expectedSize(1.0f, 1.0f);
    expected = glm::translate(expected, glm::vec3(expectedSize.x, expectedSize.y, 0.0f));  
    expected = glm::scale(expected, glm::vec3(-expectedSize.x, -expectedSize.y, 0.0f));  

    ExpectEqual(actual, expected);
}

TEST_F(Object2dTests, ModelMatrix_GivenNonZeroPositionScaleAndRotation_ModelMatrixIsCorrect)
{
    glm::vec2 testPosition(128.5f, 64.125f);
    glm::vec2 testSize(12.25, 6.5);
    float testRotationDegrees = 12.5;
    Object2d obj;
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
