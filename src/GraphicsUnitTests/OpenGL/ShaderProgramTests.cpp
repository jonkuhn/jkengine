#include <memory>
#include <string>
#include <sstream>

#include <gtest/gtest.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/gtc/type_ptr.hpp>
#pragma clang diagnostic pop

#include "Graphics/OpenGL/ShaderProgram.h"
#include "MockOpenGLWrapper.h"
#include "MockShader.h"
#include "../TestHelpers.h"

using namespace testing;
using namespace Graphics::OpenGL;

class ShaderProgramTests : public Test
{
public:
    ShaderProgramTests()
    {
        EXPECT_CALL(_mockShaderA, Handle()).WillRepeatedly(Return(_testShaderHandleA));
        EXPECT_CALL(_mockShaderB, Handle()).WillRepeatedly(Return(_testShaderHandleB));
    }

protected:
    NiceMock<MockOpenGLWrapper> _mockLib;
    const GLuint _testShaderHandleA = 123;
    const GLuint _testShaderHandleB = 456;
    const GLuint _testProgramHandle = 789;
    MockShader _mockShaderA;
    MockShader _mockShaderB;

    std::unique_ptr<ShaderProgram> CreateTestShaderProgram()
    {
        EXPECT_CALL(_mockLib, CreateProgram()).WillOnce(Return(_testProgramHandle));
        EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(true));
        return std::make_unique<ShaderProgram>(_mockLib, std::initializer_list<IShader*>({&_mockShaderA, &_mockShaderB}));
    }

    std::tuple<std::unique_ptr<ShaderProgram>,
        std::unique_ptr<ShaderProgram>> GetTwoShaderProgramsWithHandles(GLuint handleA, GLuint handleB)
    {
        Sequence seq;
        EXPECT_CALL(_mockLib, CreateProgram()).InSequence(seq).WillOnce(Return(handleA));
        EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).InSequence(seq).WillOnce(SetArgPointee<2>(true));
        EXPECT_CALL(_mockLib, CreateProgram()).InSequence(seq).WillOnce(Return(handleB));
        EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).InSequence(seq).WillOnce(SetArgPointee<2>(true));

        auto programA = std::unique_ptr<ShaderProgram>(new ShaderProgram(_mockLib, {&_mockShaderA, &_mockShaderB}));
        auto programB = std::unique_ptr<ShaderProgram>(new ShaderProgram(_mockLib, {&_mockShaderA, &_mockShaderB}));

        Mock::VerifyAndClear(&_mockLib);

        return std::tuple(std::move(programA), std::move(programB));
    }

};

TEST_F(ShaderProgramTests, Constructor_MakesCallsToCreateAndLinkProgram)
{
    Sequence s1, s2, s3, s4, s5;
    EXPECT_CALL(_mockLib, CreateProgram()).InSequence(s1, s2).WillOnce(Return(_testProgramHandle));
    EXPECT_CALL(_mockLib, AttachShader(_testProgramHandle, _testShaderHandleA)).InSequence(s1);
    EXPECT_CALL(_mockLib, AttachShader(_testProgramHandle, _testShaderHandleB)).InSequence(s2);
    EXPECT_CALL(_mockLib, LinkProgram(_testProgramHandle)).InSequence(s1, s2);
    EXPECT_CALL(_mockLib, GetProgramiv(_testProgramHandle, GL_LINK_STATUS, _))
        .InSequence(s1, s2).WillOnce(SetArgPointee<2>(true));
    EXPECT_CALL(_mockLib, DetachShader(_testProgramHandle, _testShaderHandleA)).InSequence(s1);
    EXPECT_CALL(_mockLib, DetachShader(_testProgramHandle, _testShaderHandleB)).InSequence(s2);

    ShaderProgram shaderProgram(_mockLib, {&_mockShaderA, &_mockShaderB});
}

TEST_F(ShaderProgramTests, Constructor_GivenZeroShaders_ThrowsInvalidArgument)
{
    EXPECT_THROW(
        ShaderProgram shaderProgram(_mockLib, {}),
        std::invalid_argument
    );
}

TEST_F(ShaderProgramTests, Constructor_GivenCreateFails_ThrowsRuntimeErrorWithErrorCode)
{
    GLenum testError = 5555;
    EXPECT_CALL(_mockLib, CreateProgram()).WillOnce(Return(0));
    EXPECT_CALL(_mockLib, GetError()).WillOnce(Return(testError));
    EXPECT_THROW(
        try
        {
            ShaderProgram shaderProgram(_mockLib, {&_mockShaderA, &_mockShaderB});
        }
        catch(const std::runtime_error& e)
        {
            // Just assert that it contains the info log.  Currently it
            // also contains the source code, but that may change.
            std::stringstream ss;
            ss << testError;
            EXPECT_THAT(e.what(), HasSubstr(ss.str()));
            throw;
        },
        std::runtime_error);
}

TEST_F(ShaderProgramTests, Constructor_GivenLinkingFails_ThrowsRuntimeErrorWithInfoLog)
{
    const std::string testInfoLog("fake info log");
    EXPECT_CALL(_mockLib, CreateProgram()).WillOnce(Return(_testProgramHandle));

    // Test that shader gets deleted
    EXPECT_CALL(_mockLib, DeleteProgram(_testProgramHandle));

    EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(false));
    EXPECT_CALL(_mockLib, GetProgramiv(_, GL_INFO_LOG_LENGTH, _)).WillOnce(SetArgPointee<2>(testInfoLog.size()));
    EXPECT_CALL(_mockLib, GetProgramInfoLog(_, _, _, _))
        .WillOnce(FillArgPointeeBuffer<3>(testInfoLog.c_str(), testInfoLog.size()));
    EXPECT_THROW(
        try
        {
            ShaderProgram shaderProgram(_mockLib, {&_mockShaderA, &_mockShaderB});
        }
        catch(const std::runtime_error& e)
        {
            // Just assert that it contains the info log.
            EXPECT_THAT(e.what(), HasSubstr(testInfoLog));
            throw;
        },
        std::runtime_error);
}

TEST_F(ShaderProgramTests, Destructor_CallsDeleteProgram)
{
    EXPECT_CALL(_mockLib, CreateProgram()).WillOnce(Return(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(true));
    EXPECT_CALL(_mockLib, DeleteProgram(_testProgramHandle));

    {
        ShaderProgram shaderProgram(_mockLib, {&_mockShaderA, &_mockShaderB});
    }
}

TEST_F(ShaderProgramTests, MoveConstruct_OnlySourceDoesNotCallDeleteProgram)
{
    EXPECT_CALL(_mockLib, CreateProgram()).WillOnce(Return(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(true));
    EXPECT_CALL(_mockLib, DeleteProgram(_testProgramHandle)).Times(0);

    std::unique_ptr<ShaderProgram> target;
    {
        ShaderProgram source(_mockLib, {&_mockShaderA, &_mockShaderB});
        target = std::make_unique<ShaderProgram>(std::move(source));
    }
    
    // Explicitly verify before target destructor is automatically called
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderProgramTests, MoveConstruct_TargetCallsDeleteProgram)
{
    EXPECT_CALL(_mockLib, CreateProgram()).WillOnce(Return(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetProgramiv(_, GL_LINK_STATUS, _)).WillOnce(SetArgPointee<2>(true));

    std::unique_ptr<ShaderProgram> target;
    {
        ShaderProgram source(_mockLib, {&_mockShaderA, &_mockShaderB});
        target = std::make_unique<ShaderProgram>(std::move(source));
    }

    // Clear mock so we can specifically assert that the target destructor
    // calls DeleteProgram
    Mock::VerifyAndClear(&_mockLib);

    EXPECT_CALL(_mockLib, DeleteProgram(_testProgramHandle)).Times(1);
    target.reset();
}

TEST_F(ShaderProgramTests, MoveAssign_MoveDeletesTargetsOriginalProgram)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<ShaderProgram> source, target;
    std::tie(source, target) = GetTwoShaderProgramsWithHandles(originalSourceHandle, originalTargetHandle);

    // Assert that target's original program is deleted when the move occurs
    EXPECT_CALL(_mockLib, DeleteProgram(originalTargetHandle)).Times(1);
    *target = std::move(*source);

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderProgramTests, MoveAssign_SourceDestructorDeletesNothing)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<ShaderProgram> source, target;
    std::tie(source, target) = GetTwoShaderProgramsWithHandles(originalSourceHandle, originalTargetHandle);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that deleting the source does nothing
    EXPECT_CALL(_mockLib, DeleteProgram(_)).Times(0);
    source.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderProgramTests, MoveAssign_TargetDestructorDeletesSourceProgram)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<ShaderProgram> source, target;
    std::tie(source, target) = GetTwoShaderProgramsWithHandles(originalSourceHandle, originalTargetHandle);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that the source's program (now in target) is deleted
    // when target is destroyed
    EXPECT_CALL(_mockLib, DeleteProgram(originalSourceHandle)).Times(1);
    target.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderProgramTests, Use_CallsUseProgramWithHandle)
{
    auto shaderProgram = CreateTestShaderProgram();
    EXPECT_CALL(_mockLib, UseProgram(_testProgramHandle));
    shaderProgram->Use();
}

TEST_F(ShaderProgramTests, SetUniformWithInt_CallsUseProgramUniform1i)
{
    auto shaderProgram = CreateTestShaderProgram();

    std::string testUniformName = "testUniformName";
    int testUniformLocation = 12345;
    int testUniformValue = 6789;
    EXPECT_CALL(_mockLib, UseProgram(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetUniformLocation(_testProgramHandle, StrEq(testUniformName.c_str())))
        .WillOnce(Return(testUniformLocation));
    EXPECT_CALL(_mockLib, Uniform1i(testUniformLocation, testUniformValue));
    shaderProgram->SetUniform(testUniformName, testUniformValue);
}

TEST_F(ShaderProgramTests, SetUniformWithMat4_CallsUseProgramAndUniformMatrix4fv)
{
    auto shaderProgram = CreateTestShaderProgram();

    std::string testUniformName = "testUniformName";
    int testUniformLocation = 12345;
    glm::mat4 testUniformValue(2.0f);
    EXPECT_CALL(_mockLib, UseProgram(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetUniformLocation(_testProgramHandle, StrEq(testUniformName.c_str())))
        .WillOnce(Return(testUniformLocation));
    EXPECT_CALL(_mockLib, UniformMatrix4fv(testUniformLocation, 1, false, glm::value_ptr(testUniformValue)));
    shaderProgram->SetUniform(testUniformName, testUniformValue);
}

TEST_F(ShaderProgramTests, SetUniformWithVec3_CallsUseProgramAndUniform3fv)
{
    auto shaderProgram = CreateTestShaderProgram();

    std::string testUniformName = "testUniformName";
    int testUniformLocation = 12345;
    glm::vec3 testUniformValue(2.0f);
    EXPECT_CALL(_mockLib, UseProgram(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetUniformLocation(_testProgramHandle, StrEq(testUniformName.c_str())))
        .WillOnce(Return(testUniformLocation));
    EXPECT_CALL(_mockLib, Uniform3fv(testUniformLocation, 1, glm::value_ptr(testUniformValue)));
    shaderProgram->SetUniform(testUniformName, testUniformValue);
}

TEST_F(ShaderProgramTests, SetUniformWithVec2_CallsUseProgramAndUniform2fv)
{
    auto shaderProgram = CreateTestShaderProgram();

    std::string testUniformName = "testUniformName";
    int testUniformLocation = 12345;
    glm::vec2 testUniformValue(2.0f);
    EXPECT_CALL(_mockLib, UseProgram(_testProgramHandle));
    EXPECT_CALL(_mockLib, GetUniformLocation(_testProgramHandle, StrEq(testUniformName.c_str())))
        .WillOnce(Return(testUniformLocation));
    EXPECT_CALL(_mockLib, Uniform2fv(testUniformLocation, 1, glm::value_ptr(testUniformValue)));
    shaderProgram->SetUniform(testUniformName, testUniformValue);
}
