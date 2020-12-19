#include "TestHelpers.h"

std::string GetValidVertexShaderCode()
{
    return R"##RAW##(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
        )##RAW##";
}

std::string GetValidFragmentShaderCode()
{
    return R"##RAW##(
        #version 330 core
        out vec4 FragColor;
        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
        )##RAW##";
}

std::string GetVertexShaderCodeWithUniforms()
{
    return R"##RAW##(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 testMatrix4;
        uniform sampler2D testSampler;
        uniform vec3 testVector3;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
        )##RAW##";
}

std::string GetFragmentShaderCodeWithUniforms()
{
    return R"##RAW##(
        #version 330 core
        out vec4 FragColor;
        uniform mat4 testMatrix4;
        uniform sampler2D testSampler;
        uniform vec3 testVector3;
        void main()
        {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
        )##RAW##";
}
