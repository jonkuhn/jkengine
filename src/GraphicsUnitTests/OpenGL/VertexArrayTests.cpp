
#include <memory>
#include <string>
#include <sstream>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Graphics/OpenGL/VertexArray.h"
#include "../TestHelpers.h"
#include "MockOpenGLWrapper.h"

using namespace testing;
using namespace Graphics::OpenGL;

namespace
{
    struct TestSimpleVertex
    {
        float x;
        float y;
        float z;
    };
}

class VertexArrayTests : public Test
{
public:
    VertexArrayTests()
    {
        _testThreeSimpleVertices.push_back(TestSimpleVertex({-0.5f, -0.5f, 0.0f}));
        _testThreeSimpleVertices.push_back(TestSimpleVertex({0.5f, -0.5f, 0.0f}));
        _testThreeSimpleVertices.push_back(TestSimpleVertex({0.0f, 0.5f, 0.0f}));

        _testFourSimpleVertices.push_back(TestSimpleVertex({-0.5f, -0.5f, 0.0f}));
        _testFourSimpleVertices.push_back(TestSimpleVertex({-0.5f, 0.5f, 0.0f}));
        _testFourSimpleVertices.push_back(TestSimpleVertex({0.5f, 0.5f, 0.0f}));
        _testFourSimpleVertices.push_back(TestSimpleVertex({0.5f, -0.5f, 0.0f}));

        _testTriangleElementsOfFourSimpleVertices.push_back(0);
        _testTriangleElementsOfFourSimpleVertices.push_back(1);
        _testTriangleElementsOfFourSimpleVertices.push_back(2);
        _testTriangleElementsOfFourSimpleVertices.push_back(2);
        _testTriangleElementsOfFourSimpleVertices.push_back(3);
        _testTriangleElementsOfFourSimpleVertices.push_back(4);
    }

protected:
    void SetupGenVertexArraysToAlwaysReturnDummyHandle()
    {
        EXPECT_CALL(_mockLib, GenVertexArrays(_, _))
            .WillRepeatedly(SetArgPointee<1>(111));
    }

    void SetupGenVertexArraysToAlwaysReturnGivenHandle(GLuint handle)
    {
        EXPECT_CALL(_mockLib, GenVertexArrays(_, _))
            .WillRepeatedly(SetArgPointee<1>(handle));
    }

    void SetupGenBuffersToAlwaysReturnDummyHandle()
    {
        EXPECT_CALL(_mockLib, GenBuffers(_, _))
            .WillRepeatedly(SetArgPointee<1>(222));
    }

    template<typename TParams>
    void AssertConstructingVertexArrayThrowsRuntimeErrorWithMessageContent(
        TParams&& params,
        const std::string& expectedMessageSubstring1,
        const std::string& expectedMessageSubstring2)

    {
        EXPECT_THROW(
            try
            {
                VertexArray<TestSimpleVertex> vertexArray(
                    _mockLib, std::move(params));
            }
            catch(const std::runtime_error &e)
            {
                EXPECT_THAT(e.what(), HasSubstr(expectedMessageSubstring1));
                EXPECT_THAT(e.what(), HasSubstr(expectedMessageSubstring2));
                throw;
            }, std::runtime_error);
    }

    std::tuple<std::unique_ptr<VertexArray<TestSimpleVertex>>,
        std::unique_ptr<VertexArray<TestSimpleVertex>>> GetTwoVertexArraysWithHandles(
            GLuint vaoA, GLuint vboA, GLuint eboA,
            GLuint vaoB, GLuint vboB, GLuint eboB)
    {
        // Setup GenVertexArrays to return _testVAOHandle so we can test clean up
        SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

        Sequence seq; // used to help gmock differentiate the calls
        EXPECT_CALL(_mockLib, GenVertexArrays(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(vaoA));
        EXPECT_CALL(_mockLib, GenBuffers(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(vboA));
        EXPECT_CALL(_mockLib, GenBuffers(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(eboA));
        EXPECT_CALL(_mockLib, GenVertexArrays(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(vaoB));
        EXPECT_CALL(_mockLib, GenBuffers(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(vboB));
        EXPECT_CALL(_mockLib, GenBuffers(_, _)).InSequence(seq).WillOnce(SetArgPointee<1>(eboB));

        auto vertexArrayA = std::make_unique<VertexArray<TestSimpleVertex>>(
            _mockLib,
            VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
                .AddAttribute(_testSimpleVertexSize)
                .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices));

        auto vertexArrayB = std::make_unique<VertexArray<TestSimpleVertex>>(
            _mockLib,
            VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
                .AddAttribute(_testSimpleVertexSize)
                .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices));

        Mock::VerifyAndClear(&_mockLib);

        return std::tuple(std::move(vertexArrayA), std::move(vertexArrayB));
    }

    NiceMock<MockOpenGLWrapper> _mockLib;
    const GLuint _testVAOHandle = 123;
    const GLuint _testVBOHandle = 456;
    const GLuint _testEBOHandle = 789;

    std::vector<TestSimpleVertex> _testThreeSimpleVertices;
    std::vector<TestSimpleVertex> _testFourSimpleVertices;
    std::vector<GLuint> _testTriangleElementsOfFourSimpleVertices;
    const GLuint _testSimpleVertexSize = 3;

};

TEST_F(VertexArrayTests, ParamsConstructor_GivenVertices_DoesNotThrow)
{
    EXPECT_NO_THROW(
        VertexArray<TestSimpleVertex>::Params params(
            std::vector<TestSimpleVertex>({{0.5f, 0.5f, 0.0f}})
        )
    );
}

TEST_F(VertexArrayTests, ParamsTriangleElementIndices_GivenIndexCountIsMultipleOf3_DoesNotThrow)
{
    VertexArray<TestSimpleVertex>::Params params(_testThreeSimpleVertices);

    EXPECT_NO_THROW(
        params.TriangleElementIndices(std::vector<GLuint>({1, 2, 3}))
    );
}

TEST_F(VertexArrayTests, ParamsTriangleElementIndices_GivenIndexCountNotMultipleOf3_ThrowsInvalidArgument)
{
    VertexArray<TestSimpleVertex>::Params params(_testThreeSimpleVertices);

    EXPECT_THROW(params.TriangleElementIndices(std::vector<GLuint>({1})), std::invalid_argument);
    EXPECT_THROW(params.TriangleElementIndices(std::vector<GLuint>({1, 2})), std::invalid_argument);
}

TEST_F(VertexArrayTests, ParamsAddAttribute_GivenInvalidSize0or5_ThrowsInvalidArgument)
{
    VertexArray<TestSimpleVertex>::Params params(_testThreeSimpleVertices);

    // The only valid sizes are 1, 2, 3, and 4
    EXPECT_THROW(params.AddAttribute(0), std::invalid_argument);
    EXPECT_THROW(params.AddAttribute(5), std::invalid_argument);
}

TEST_F(VertexArrayTests, ParamsAddAttribute_GivenValidSizes_DoesNotThrow)
{
    VertexArray<TestSimpleVertex>::Params params(_testThreeSimpleVertices);

    EXPECT_NO_THROW(params.AddAttribute(1));
    EXPECT_NO_THROW(params.AddAttribute(2));
    EXPECT_NO_THROW(params.AddAttribute(3));
    EXPECT_NO_THROW(params.AddAttribute(4));
}

TEST_F(VertexArrayTests, Constructor_GivenVertices_GeneratesVertexArrayLoadsVertexDataChecksErrors)
{
    // We generally avoid testing sequences of calls but they are
    // important for OpenGL.  Sequence of calls must be:
    // - GetError (to clear errors)
    // - GenVertexArrays and GenBuffers (in any order)
    // - BindVertexArray
    // - BindBuffer
    // - BufferData
    // - BindBuffer(0)
    // - BindVertexArray(0)
    // - GetError
    Sequence s1, s2;

    // Clear Errors first
    EXPECT_CALL(_mockLib, GetError)
        .Times(1).InSequence(s1, s2).WillOnce(Return(0));

    // Generate Vertex array and buffer in any order
    EXPECT_CALL(_mockLib, GenVertexArrays(Eq(1), _))
        .InSequence(s1)
        .WillOnce(SetArgPointee<1>(_testVAOHandle));

    EXPECT_CALL(_mockLib, GenBuffers(Eq(1), _))
        .InSequence(s2)
        .WillOnce(SetArgPointee<1>(_testVBOHandle));

    // Must bind the vertex array next
    EXPECT_CALL(_mockLib, BindVertexArray(_)).Times(AnyNumber()); // ignore other calls
    EXPECT_CALL(_mockLib, BindVertexArray(_testVAOHandle))
        .Times(1).InSequence(s1, s2);

    // Must bind the buffer next
    EXPECT_CALL(_mockLib, BindBuffer(_, _)).Times(AnyNumber()); // ignore other calls
    EXPECT_CALL(_mockLib, BindBuffer(GLenum(GL_ARRAY_BUFFER), _testVBOHandle))
        .Times(1).InSequence(s1, s2);

    // to simplify assertions make a copy we can move into the params
    std::vector<TestSimpleVertex> testVertices(_testThreeSimpleVertices);

    // Must set the buffer data next
    EXPECT_CALL(_mockLib, BufferData(
        GLenum(GL_ARRAY_BUFFER),
        GLsizeiptr(_testThreeSimpleVertices.size() * sizeof(TestSimpleVertex)),
        testVertices.data(),
        GLenum(GL_STATIC_DRAW)))
        .Times(1).InSequence(s1, s2);

    // Unbind in any order later.  The order does not matter as long
    // as VertexAttribPointer is called with the vertex array bound
    // (this will be verified in another test)
    EXPECT_CALL(_mockLib, BindVertexArray(0))
        .Times(1).InSequence(s1);

    EXPECT_CALL(_mockLib, BindBuffer(GLenum(GL_ARRAY_BUFFER), 0))
        .Times(1).InSequence(s2);

    // Must check errors last
    EXPECT_CALL(_mockLib, GetError)
        .Times(1).InSequence(s1, s2).WillOnce(Return(0));

    VertexArray<TestSimpleVertex> vertexArray(
        _mockLib,
        VertexArray<TestSimpleVertex>::Params(std::move(testVertices))
            .AddAttribute(3));
}

TEST_F(VertexArrayTests, Constructor_GivenVertexCountIsNotMultipleOf3AndNoElementIndices_ThrowsInvalidArgument)
{
    SetupGenVertexArraysToAlwaysReturnDummyHandle();
    SetupGenBuffersToAlwaysReturnDummyHandle();

    EXPECT_THROW(
        VertexArray<TestSimpleVertex> vertexArray(
            _mockLib,
            std::vector<TestSimpleVertex>({{0.5f, 0.5f, 0.0f}})
        ),
        std::invalid_argument
    );
}

TEST_F(VertexArrayTests, Constructor_GivenNoAttributes_ThrowsInvalidArgument)
{
    SetupGenVertexArraysToAlwaysReturnDummyHandle();
    SetupGenBuffersToAlwaysReturnDummyHandle();

    EXPECT_THROW(
        VertexArray<TestSimpleVertex> vertexArray(
            _mockLib,
            VertexArray<TestSimpleVertex>::Params(_testThreeSimpleVertices)),
        std::invalid_argument);
}

TEST_F(VertexArrayTests, Constructor_Given2Attributes_ConfiguresAndEnables2Attributes)
{
    struct TwoAttributeVertex
    {
        // attribute 0 has size 3
        float a1, a2, a3;
        // attribute 1 has size 4
        float b1, b2, b3, b4;
    };

    const GLint attribute0Size = 3;
    const GLint attribute1Size = 4;

    std::vector<TwoAttributeVertex> testVertices({
        {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f},
        {2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f},
        {3.0f, 6.0f, 9.0f, 16.0f, 15.0f, 18.0f, 21.0f},
    });

    SetupGenBuffersToAlwaysReturnDummyHandle();

    // We generally avoid testing sequences of calls but they are
    // important for OpenGL.  Sequence of calls must be:
    // - GetError (to clear errors)
    // - GenVertexArrays
    // - BindVertexArray
    // - VertexAttribPointer and EnableVertexAttribArray in any order
    // - GetError
    Sequence s1, s2, s3, s4;

    // Clear Errors first
    EXPECT_CALL(_mockLib, GetError)
        .Times(1).InSequence(s1, s2, s3, s4).WillOnce(Return(0));

    // Must create the vertex array first
    EXPECT_CALL(_mockLib, GenVertexArrays(Eq(1), _))
        .InSequence(s1, s2, s3, s4)
        .WillOnce(SetArgPointee<1>(_testVAOHandle));

    // Must bind the vertex array next
    EXPECT_CALL(_mockLib, BindVertexArray(_)).Times(AnyNumber()); // ignore other calls
    EXPECT_CALL(_mockLib, BindVertexArray(_testVAOHandle))
        .Times(1).InSequence(s1, s2, s3, s4);

    // May call VertexAttribPointer and EnableVertexAttribArray for both
    // attributes in any order
    EXPECT_CALL(_mockLib, VertexAttribPointer(
        0, attribute0Size, GL_FLOAT, GL_FALSE, sizeof(TwoAttributeVertex), reinterpret_cast<void*>(0))
    ).Times(1).InSequence(s1);

    EXPECT_CALL(_mockLib, VertexAttribPointer(
        1, attribute1Size, GL_FLOAT, GL_FALSE, sizeof(TwoAttributeVertex), reinterpret_cast<void*>(attribute0Size * sizeof(GLfloat)))
    ).Times(1).InSequence(s2);

    EXPECT_CALL(_mockLib, EnableVertexAttribArray(0)).Times(1).InSequence(s3);
    EXPECT_CALL(_mockLib, EnableVertexAttribArray(1)).Times(1).InSequence(s4);

    // Must check errors last
    EXPECT_CALL(_mockLib, GetError)
        .Times(1).InSequence(s1, s2, s3, s4).WillOnce(Return(0));

    VertexArray<TwoAttributeVertex> vertexArray(
        _mockLib,
        VertexArray<TwoAttributeVertex>::Params(testVertices)
            .AddAttribute(attribute0Size)
            .AddAttribute(attribute1Size)
    );
}

TEST_F(VertexArrayTests, Constructor_GivenTriangleElementIndices_SetsUpElementBufferChecksErrors)
{
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

    // We generally avoid testing sequences of calls but they are
    // important for for OpenGL.  Sequence of calls must be:
    // 
    // - GetError (to clear errors)
    // - BindVertexArray and GenBuffers in any order
    // - BindBuffer (for GL_ELEMENT_ARRAY_BUFFER)
    // - BufferData (for GL_ELEMENT_ARRAY_BUFFER)
    // - BindVertexArray(0)
    // - GetError
    Sequence s1, s2;
    // Clear Errors first
    EXPECT_CALL(_mockLib, GetError)
        .Times(1).InSequence(s1).WillOnce(Return(0));

    // Vertex array must be bound before binding element buffer
    EXPECT_CALL(_mockLib, BindVertexArray(_testVAOHandle))
        .Times(1).InSequence(s1);

    // This test doesn't want to care about this call, but it has to rely
    // on the sequence of the calls to return _testEBOHandle from the correct
    // call.  This setup returns a dummy value the test should not care about
    // or assert against, but in reality it will be the VBO handle.
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(s2).WillOnce(SetArgPointee<1>(999));

    // Generate the buffer for the element indices
    EXPECT_CALL(_mockLib, GenBuffers(Eq(1), _))
        .InSequence(s2)
        .WillOnce(SetArgPointee<1>(_testEBOHandle));

    // Bind the buffer after binding the VAO and generating the buffer
    EXPECT_CALL(_mockLib, BindBuffer(_, _)).Times(AnyNumber()); // ignore other calls
    EXPECT_CALL(_mockLib, BindBuffer(GLenum(GL_ELEMENT_ARRAY_BUFFER), _testEBOHandle))
        .Times(1).InSequence(s1, s2);

    // to simplify assertions make a copy we can move into the params
    std::vector<GLuint> testElements(_testTriangleElementsOfFourSimpleVertices);

    // Set the buffer data after binding the buffer
    EXPECT_CALL(_mockLib, BufferData(_, _, _, _)).Times(AnyNumber()); // ignore other calls
    EXPECT_CALL(_mockLib, BufferData(
        GLenum(GL_ELEMENT_ARRAY_BUFFER),
        GLsizeiptr(testElements.size() * sizeof(GLuint)),
        testElements.data(),
        GLenum(GL_STATIC_DRAW)))
        .Times(1).InSequence(s1, s2);

    // Unbind the VAO
    EXPECT_CALL(_mockLib, BindVertexArray(0))
        .Times(1).InSequence(s1, s2);

    // Must check errors last
    EXPECT_CALL(_mockLib, GetError)
        .Times(1).InSequence(s1, s2).WillOnce(Return(0));

    VertexArray<TestSimpleVertex> vertexArray(
        _mockLib,
        VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
        .AddAttribute(_testSimpleVertexSize)
        .TriangleElementIndices(std::move(testElements)));
}

TEST_F(VertexArrayTests, Constructor_GivenGenVertexArraysThrows_ThrowsRuntimeError)
{
    // called once to clear errors
    Sequence s1; // used to help gmock differentiate GetErrors calls
    EXPECT_CALL(_mockLib, GetError()) .InSequence(s1).WillOnce(Return(0));

    EXPECT_CALL(_mockLib, GenVertexArrays(_, _))
        .WillOnce(SetArgPointee<1>(0));

    // called once to get error after receiving invalid handle
    const GLenum testErrorCode = 123456;
    EXPECT_CALL(_mockLib, GetError())
        .InSequence(s1).WillOnce(Return(testErrorCode));
        
    AssertConstructingVertexArrayThrowsRuntimeErrorWithMessageContent(
        VertexArray<TestSimpleVertex>::Params(_testThreeSimpleVertices)
            .AddAttribute(_testSimpleVertexSize),
        "GenVertexArrays", std::to_string(testErrorCode));
}

TEST_F(VertexArrayTests, Constructor_GivenFirstGenBuffersThrows_ThrowsRuntimeErrorAndCleansUp)
{
    // called once to clear errors
    Sequence s1; // used to help gmock differentiate GetErrors calls
    EXPECT_CALL(_mockLib, GetError()) .InSequence(s1).WillOnce(Return(0));

    // Setup GenVertexArrays to return _testVAOHandle so we can test clean up
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

    // First GenBuffers call returns invalid handle
    EXPECT_CALL(_mockLib, GenBuffers(_, _)).WillOnce(SetArgPointee<1>(0));

    // Expect VAO to be cleaned up
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(_testVAOHandle))));

    // called once to get error after receiving invalid handle
    const GLenum testErrorCode = 123456;
    EXPECT_CALL(_mockLib, GetError())
        .InSequence(s1).WillOnce(Return(testErrorCode));

    AssertConstructingVertexArrayThrowsRuntimeErrorWithMessageContent(
        VertexArray<TestSimpleVertex>::Params(_testThreeSimpleVertices)
            .AddAttribute(_testSimpleVertexSize),
        "GenBuffers", std::to_string(testErrorCode));
}
TEST_F(VertexArrayTests, Constructor_GivenSecondGenBuffersThrows_ThrowsRuntimeErrorAndCleansUp)
{
    // called once to clear errors
    Sequence s1; // used to help gmock differentiate GetErrors calls
    EXPECT_CALL(_mockLib, GetError()).InSequence(s1).WillOnce(Return(0));

    // Setup GenVertexArrays to return _testVAOHandle so we can test clean up
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

    // Sequence is just used to help gmock differentiate multiple calls to GenBuffers
    Sequence s2;

    // First GenBuffers call works fine (returns test VBO handle so we can test cleanup)
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(s2).WillOnce(SetArgPointee<1>(_testVBOHandle));

    // Second GenBuffers call returns invalid handle
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(s2).WillOnce(SetArgPointee<1>(0));

    // Expect VAO to be cleaned up
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(_testVAOHandle))));

    // Expect VBO to be cleaned up
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testVBOHandle))));

    // called once to get error after receiving invalid handle
    const GLenum testErrorCode = 123456;
    EXPECT_CALL(_mockLib, GetError())
        .InSequence(s1).WillOnce(Return(testErrorCode));

    AssertConstructingVertexArrayThrowsRuntimeErrorWithMessageContent(
        VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
            .AddAttribute(_testSimpleVertexSize)
            .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices),
        "GenBuffers", std::to_string(testErrorCode));
}

TEST_F(VertexArrayTests, Constructor_GivenErrorSetAtEndOfCostruction_ThrowsRuntimeErrorAndCleansUp)
{
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);
    SetupGenBuffersToAlwaysReturnDummyHandle();

    // called once to clear errors
    Sequence s1; // used to help gmock differentiate GetErrors calls
    EXPECT_CALL(_mockLib, GetError()).InSequence(s1).WillOnce(Return(0));

    // Setup GenVertexArrays to return _testVAOHandle so we can test clean up
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

    // GenBuffers returns test VBO handle, then test EBO so we can test cleanup
    Sequence s2; // used to help gmock differentiate GenBuffers calls
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(s2).WillOnce(SetArgPointee<1>(_testVBOHandle));
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(s2).WillOnce(SetArgPointee<1>(_testEBOHandle));

    // called any number of times to get error at end of constructor
    const GLenum testErrorCode = 123456;
    EXPECT_CALL(_mockLib, GetError())
        .InSequence(s1).WillRepeatedly(Return(testErrorCode));

    // Expect VAO to be cleaned up
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(_testVAOHandle))));

    // Expect both VBOs to be cleaned up
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testVBOHandle))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testEBOHandle))));

    AssertConstructingVertexArrayThrowsRuntimeErrorWithMessageContent(
        VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
            .AddAttribute(_testSimpleVertexSize)
            .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices),
        "VertexArray Constructor", std::to_string(testErrorCode));
}

TEST_F(VertexArrayTests, Destructor_CleansUpVertexArrayAndBuffers)
{
    // Setup GenVertexArrays to return _testVAOHandle so we can test clean up
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

    // GenBuffers returns test VBO handle, then test EBO so we can test cleanup
    Sequence seq; // used to help gmock differentiate GenBuffers calls
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(seq).WillOnce(SetArgPointee<1>(_testVBOHandle));
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(seq).WillOnce(SetArgPointee<1>(_testEBOHandle));

    // Expect VAO to be cleaned up
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(_testVAOHandle))));

    // Expect both VBOs to be cleaned up
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testVBOHandle))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testEBOHandle))));

    // Construct vertex array in a block to trigger destructor
    {
        VertexArray<TestSimpleVertex> vertexArray(
            _mockLib,
            VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
                .AddAttribute(_testSimpleVertexSize)
                .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices));
    }
}

TEST_F(VertexArrayTests, MoveConstruct_TargetCallsDeleteVertexArrayAndBuffers)
{
    // Setup GenVertexArrays to return _testVAOHandle so we can test clean up
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);

    // GenBuffers returns test VBO handle, then test EBO so we can test cleanup
    Sequence seq; // used to help gmock differentiate GenBuffers calls
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(seq).WillOnce(SetArgPointee<1>(_testVBOHandle));
    EXPECT_CALL(_mockLib, GenBuffers(_, _))
        .InSequence(seq).WillOnce(SetArgPointee<1>(_testEBOHandle));

    std::unique_ptr<VertexArray<TestSimpleVertex>> target;
    {
        VertexArray<TestSimpleVertex> source(
            _mockLib,
            VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
                .AddAttribute(_testSimpleVertexSize)
                .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices));
        target = std::make_unique<VertexArray<TestSimpleVertex>>(std::move(source));
    }

    // Clear mock so we can specifically assert that the target destructor
    // calls DeleteTexture
    Mock::VerifyAndClear(&_mockLib);

    // Expect VAO  and both VBOs to be cleaned up
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(_testVAOHandle))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testVBOHandle))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(_testEBOHandle))));
    target.reset();
}

TEST_F(VertexArrayTests, MoveAssign_MoveDeletesTargetsOriginalHandles)
{
    const GLuint originalSourceVao = 111;
    const GLuint originalSourceVbo = 222;
    const GLuint originalSourceEbo = 333;
    const GLuint originalTargetVao = 444;
    const GLuint originalTargetVbo = 555;
    const GLuint originalTargetEbo = 666;

    std::unique_ptr<VertexArray<TestSimpleVertex>> source, target;
    std::tie(source, target) = GetTwoVertexArraysWithHandles(
        originalSourceVao, originalSourceVbo, originalSourceEbo,
        originalTargetVao, originalTargetVbo, originalTargetEbo);

    // Assert that target's original vertex array's buffers are deleted when
    // the move occurs
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(originalTargetVao))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(originalTargetVbo))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(originalTargetEbo))));
    *target = std::move(*source);

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(VertexArrayTests, MoveAssign_SourceDestructorDeletesNothing)
{
    const GLuint originalSourceVao = 111;
    const GLuint originalSourceVbo = 222;
    const GLuint originalSourceEbo = 333;
    const GLuint originalTargetVao = 444;
    const GLuint originalTargetVbo = 555;
    const GLuint originalTargetEbo = 666;

    std::unique_ptr<VertexArray<TestSimpleVertex>> source, target;
    std::tie(source, target) = GetTwoVertexArraysWithHandles(
        originalSourceVao, originalSourceVbo, originalSourceEbo,
        originalTargetVao, originalTargetVbo, originalTargetEbo);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that deleting the source does nothing
    EXPECT_CALL(_mockLib, DeleteVertexArrays(_, _)).Times(0);
    EXPECT_CALL(_mockLib, DeleteBuffers(_, _)).Times(0);
    EXPECT_CALL(_mockLib, DeleteBuffers(_, _)).Times(0);
    source.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(VertexArrayTests, MoveAssign_TargetDestructorDeletesSourceTexture)
{
    const GLuint originalSourceVao = 111;
    const GLuint originalSourceVbo = 222;
    const GLuint originalSourceEbo = 333;
    const GLuint originalTargetVao = 444;
    const GLuint originalTargetVbo = 555;
    const GLuint originalTargetEbo = 666;

    std::unique_ptr<VertexArray<TestSimpleVertex>> source, target;
    std::tie(source, target) = GetTwoVertexArraysWithHandles(
        originalSourceVao, originalSourceVbo, originalSourceEbo,
        originalTargetVao, originalTargetVbo, originalTargetEbo);

    *target = std::move(*source);

    // clear expectations so no previous calls affect our real assertion below
    Mock::VerifyAndClear(&_mockLib);

    // Assert that the source's buffers (now in target) is deleted
    // when target is destroyed
    EXPECT_CALL(_mockLib, DeleteVertexArrays(1, Pointee(Eq(originalSourceVao))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(originalSourceVbo))));
    EXPECT_CALL(_mockLib, DeleteBuffers(1, Pointee(Eq(originalSourceEbo))));
    target.reset();

    // Explicitly check expectations so that other destructors do not get
    // called before verification.
    Mock::VerifyAndClear(&_mockLib);
}

TEST_F(VertexArrayTests, Draw_GivenTriangleElementIndices_BindsVertexArrayAndDrawsElements)
{
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);
    SetupGenBuffersToAlwaysReturnDummyHandle();

    VertexArray<TestSimpleVertex> vertexArray(
        _mockLib,
        VertexArray<TestSimpleVertex>::Params(_testFourSimpleVertices)
        .AddAttribute(_testSimpleVertexSize)
        .TriangleElementIndices(_testTriangleElementsOfFourSimpleVertices));

    // Clear because everything above was just the Arrange portion of the test
    Mock::VerifyAndClear(&_mockLib);

    // Now set up the real expectations this test is concerned with and
    // call the method under test (Draw)

    // We generally avoid testing sequences of calls but they are
    // important for for OpenGL.  Sequence of calls must be:
    // - BindVertexArray
    // - DrawArrays
    Sequence s1;
    EXPECT_CALL(_mockLib, BindVertexArray(_testVAOHandle))
        .Times(1).InSequence(s1);

    const void* expectedIndicesLocation = 0;
    EXPECT_CALL(_mockLib,
        DrawElements(
            GL_TRIANGLES,
            _testTriangleElementsOfFourSimpleVertices.size(),
            GL_UNSIGNED_INT,
            expectedIndicesLocation))
        .Times(1).InSequence(s1);

    vertexArray.Draw();
}

TEST_F(VertexArrayTests, Draw_GivenNoTriangleElementIndices_BindsVertexArrayAndDrawsArrays)
{
    SetupGenVertexArraysToAlwaysReturnGivenHandle(_testVAOHandle);
    SetupGenBuffersToAlwaysReturnDummyHandle();

    VertexArray<TestSimpleVertex> vertexArray(
        _mockLib,
        VertexArray<TestSimpleVertex>::Params(_testThreeSimpleVertices)
        .AddAttribute(_testSimpleVertexSize));

    // Clear because everything above was just the Arrange portion of the test
    Mock::VerifyAndClear(&_mockLib);

    // Now set up the real expectations this test is concerned with and
    // call the method under test (Draw)

    // We generally avoid testing sequences of calls but they are
    // important for for OpenGL.  Sequence of calls must be:
    // - BindVertexArray
    // - DrawArrays
    Sequence s1;
    EXPECT_CALL(_mockLib, BindVertexArray(_testVAOHandle))
        .Times(1).InSequence(s1);

    const GLint expectedStartIndex = 0;
    EXPECT_CALL(_mockLib, DrawArrays(GL_TRIANGLES, expectedStartIndex, _testThreeSimpleVertices.size()))
        .Times(1).InSequence(s1);

    vertexArray.Draw();
}


