#include <string>
#include <sstream>

#include <gtest/gtest.h>

#include "Graphics/OpenGL/Shader.h"
#include "../TestHelpers.h"
#include "MockOpenGLWrapper.h"

using namespace testing;
using namespace Graphics::OpenGL;

class ShaderTests : public Test
{
public:
    ShaderTests()
        : _testSource("fake source code")
    {

    }

protected:
    void SetupMockCreateToAlwaysSucceed()
    {
        EXPECT_CALL(_mockLib, CreateShader(_)).WillRepeatedly(Return(_testHandle));
    }

    void SetupMockCompileToAlwaysSucceed()
    {
        EXPECT_CALL(_mockLib, GetShaderiv(_, GL_COMPILE_STATUS, _)).WillRepeatedly(SetArgPointee<2>(true));
    }

    std::tuple<std::unique_ptr<Shader>,
        std::unique_ptr<Shader>> GetTwoShadersWithHandles(GLuint handleA, GLuint handleB)
    {
        Sequence seq;
        EXPECT_CALL(_mockLib, CreateShader(_)).InSequence(seq).WillOnce(Return(handleA));
        EXPECT_CALL(_mockLib, CreateShader(_)).InSequence(seq).WillOnce(Return(handleB));
        SetupMockCompileToAlwaysSucceed();

        auto shaderA = std::make_unique<Shader>(&_mockLib, Shader::Type::Vertex, _testSource);
        auto shaderB = std::make_unique<Shader>(&_mockLib, Shader::Type::Vertex, _testSource);

        Mock::VerifyAndClear(&_mockLib);

        return std::tuple(std::move(shaderA), std::move(shaderB));
    }

    NiceMock<MockOpenGLWrapper> _mockLib;
    const GLuint _testHandle = 456;
    const std::string _testSource;
};

TEST_F(ShaderTests, Constructor_MakesCallsToCreateAndCompileShader)
{
    EXPECT_CALL(_mockLib, CreateShader(_)).WillOnce(Return(_testHandle));
    EXPECT_CALL(_mockLib, ShaderSource(_testHandle, 1, DoublePtrStrEq(_testSource), nullptr));
    EXPECT_CALL(_mockLib, CompileShader(_testHandle));
    EXPECT_CALL(_mockLib, GetShaderiv(_testHandle, GL_COMPILE_STATUS, _))
        .WillOnce(SetArgPointee<2>(true));

    Shader shader(&_mockLib, Shader::Type::Vertex, _testSource);
}

TEST_F(ShaderTests, Constructor_CallsCreateShaderWithExpectedType)
{
    SetupMockCompileToAlwaysSucceed();

    EXPECT_CALL(_mockLib, CreateShader(GL_VERTEX_SHADER)).WillOnce(Return(_testHandle));
    Shader vertexShader(&_mockLib, Shader::Type::Vertex, _testSource);

    EXPECT_CALL(_mockLib, CreateShader(GL_FRAGMENT_SHADER)).WillOnce(Return(_testHandle));
    Shader fragmentShader(&_mockLib, Shader::Type::Fragment, _testSource);
}

TEST_F(ShaderTests, ConstructFromStream_PassesSourceCorrectly)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockCompileToAlwaysSucceed();

    EXPECT_CALL(_mockLib, ShaderSource(_, 1, DoublePtrStrEq(_testSource), nullptr));

    std::stringstream sourceStream(_testSource);
    Shader shader(&_mockLib, Shader::Type::Vertex, sourceStream);
}

TEST_F(ShaderTests, Constructor_GivenCreateFails_ThrowsRuntimeErrorWithErrorCode)
{
    GLenum testError = 5555;
    EXPECT_CALL(_mockLib, CreateShader(_)).WillOnce(Return(0));
    EXPECT_CALL(_mockLib, GetError()).WillOnce(Return(testError));
    EXPECT_THROW(
        try
        {
            Shader shader(&_mockLib, Shader::Type::Vertex, _testSource);
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

TEST_F(ShaderTests, Constructor_GivenCompilationFails_ThrowsRuntimeErrorWithInfoLogAndDeletesShader)
{
    const std::string testInfoLog("fake info log");
    EXPECT_CALL(_mockLib, CreateShader(_)).WillOnce(Return(_testHandle));
    
    // Test that shader gets deleted
    EXPECT_CALL(_mockLib, DeleteShader(_testHandle));

    EXPECT_CALL(_mockLib, GetShaderiv(_, GL_COMPILE_STATUS, _)).WillOnce(SetArgPointee<2>(false));
    EXPECT_CALL(_mockLib, GetShaderiv(_, GL_INFO_LOG_LENGTH, _)).WillOnce(SetArgPointee<2>(testInfoLog.size()));
    EXPECT_CALL(_mockLib, GetShaderInfoLog(_, _, _, _))
        .WillOnce(FillArgPointeeBuffer<3>(testInfoLog.c_str(), testInfoLog.size()));
    EXPECT_THROW(
        try
        {
            Shader shader(&_mockLib, Shader::Type::Vertex, _testSource);
        }
        catch(const std::runtime_error& e)
        {
            // Just assert that it contains the info log.  Currently it
            // also contains the source code, but that may change.
            EXPECT_THAT(e.what(), HasSubstr(testInfoLog));
            throw;
        },
        std::runtime_error);
}

TEST_F(ShaderTests, Destructor_CallsDeleteShader)
{
    EXPECT_CALL(_mockLib, CreateShader(_))
        .WillOnce(Return(_testHandle));
    EXPECT_CALL(_mockLib, DeleteShader(_testHandle));
    SetupMockCompileToAlwaysSucceed();

    {
        Shader shader(&_mockLib, Shader::Type::Vertex, _testSource);
    }
}

TEST_F(ShaderTests, MoveConstruct_TargetCallsDeleteShader)
{
    EXPECT_CALL(_mockLib, CreateShader(_)).WillOnce(Return(_testHandle));
    SetupMockCompileToAlwaysSucceed();

    std::unique_ptr<Shader> target;
    {
        Shader source(&_mockLib, Shader::Type::Vertex, _testSource);
        target = std::make_unique<Shader>(std::move(source));
    }

    // Clear mock so we can specifically assert that the target destructor
    // calls DeleteShader
    Mock::VerifyAndClear(&_mockLib);

    EXPECT_CALL(_mockLib, DeleteShader(_testHandle)).Times(1);
    target.reset();
}

TEST_F(ShaderTests, MoveAssign_MoveDeletesTargetsOriginalShader)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<Shader> source, target;
    std::tie(source, target) = GetTwoShadersWithHandles(originalSourceHandle, originalTargetHandle);

    // Assert that target's original texture is deleted when the move occurs
    EXPECT_CALL(_mockLib, DeleteShader(originalTargetHandle)).Times(1);
    *target = std::move(*source);

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderTests, MoveAssign_SourceDestructorDeletesNothing)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<Shader> source, target;
    std::tie(source, target) = GetTwoShadersWithHandles(originalSourceHandle, originalTargetHandle);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that deleting the source does nothing
    EXPECT_CALL(_mockLib, DeleteShader(_)).Times(0);
    source.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderTests, MoveAssign_TargetDestructorDeletesSourceShader)
{
    const GLuint originalSourceHandle = 888;
    const GLuint originalTargetHandle = 999;

    std::unique_ptr<Shader> source, target;
    std::tie(source, target) = GetTwoShadersWithHandles(originalSourceHandle, originalTargetHandle);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that the source's shader (now in target) is deleted
    // when target is destroyed
    EXPECT_CALL(_mockLib, DeleteShader(originalSourceHandle)).Times(1);
    target.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(ShaderTests, Handle_ReturnsShaderHandle)
{
    SetupMockCreateToAlwaysSucceed();
    SetupMockCompileToAlwaysSucceed();

    Shader shader(&_mockLib, Shader::Type::Vertex, _testSource);
    EXPECT_EQ(shader.Handle(), _testHandle);
}
