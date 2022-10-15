#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma clang diagnostic pop

#include <gmock/gmock.h>
#include <gtest/gtest.h>

MATCHER_P(DoublePtrStrEq, expectedStr, "") { return expectedStr == std::string(*arg); }

ACTION_TEMPLATE(FillArgPointeeBuffer, HAS_1_TEMPLATE_PARAMS(unsigned, argIndex), AND_2_VALUE_PARAMS(srcBuffer, srcSize))
{
    std::memcpy(std::get<argIndex>(args), srcBuffer, srcSize);
}

ACTION_TEMPLATE(SavePointerToArgReferee, HAS_1_TEMPLATE_PARAMS(unsigned, argIndex), AND_1_VALUE_PARAMS(pointer))
{
    *pointer = &(std::get<argIndex>(args));
}

inline void ExpectEqual(const glm::mat4& actual, const glm::mat4& expected)
{
    EXPECT_TRUE(0 == std::memcmp(glm::value_ptr(actual), glm::value_ptr(expected), 4 * 4 * sizeof(float)))
        << "Matrices are not equal" << std::endl
        << "actual:" << glm::to_string(actual) << std::endl
        << " expected:" << glm::to_string(expected);
}
