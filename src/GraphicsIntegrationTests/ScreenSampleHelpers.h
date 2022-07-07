#pragma once

#include <array>
#include <string>

#include <gtest/gtest.h>

#include <jkengine/Graphics/Color.h>
#include <jkengine/Graphics/IScreenshot.h>

inline void ExpectColorAtScreenPosition(
    const Graphics::IScreenshot& screenshot,
    std::string name,
    const Graphics::Color& expectedColor,
    float x,
    float y,
    unsigned int row,
    unsigned int column)
{
    auto sx = static_cast<unsigned int>(x);
    auto sy = static_cast<unsigned int>(y);
    EXPECT_EQ(expectedColor, screenshot.GetPixel(x, y))
        << "(" << name <<  ") row = " << row << " column = " << column
        << " (" << sx << ", " << sy << ")";
}

inline void ExpectColorAtScreenPositionIfOnScreen(
    const Graphics::IScreenshot& screenshot,
    std::string name,
    const Graphics::Color& expectedColor,
    float x,
    float y,
    unsigned int row,
    unsigned int column)
{
    // only check points that are on screen
    if(x > 0 && x < screenshot.Width() && y > 0 && y < screenshot.Height())
    {
        ExpectColorAtScreenPosition(screenshot, name, expectedColor, x, y, row, column);
    }
}

template<unsigned int ColumnCount, unsigned int RowCount>
inline void ExpectTileColorGridOnScreen(
    const Graphics::IScreenshot& screenshot,
    std::array<std::array<Graphics::Color, ColumnCount>, RowCount> expectedTileColors)
{
    auto columnWidth = static_cast<float>(screenshot.Width()) / static_cast<float>(ColumnCount);
    auto rowHeight = static_cast<float>(screenshot.Height()) / static_cast<float>(RowCount);
    auto sampleDistanceFromEdgeX = columnWidth / 8.0f;
    auto sampleDistanceFromEdgeY = rowHeight / 8.0f;

    // sample each tile near 4 corners plus center
    auto sampleLeftX = sampleDistanceFromEdgeX;
    auto sampleUpperY = sampleDistanceFromEdgeY;
    auto sampleRightX = columnWidth - sampleDistanceFromEdgeX;
    auto sampleLowerY = rowHeight - sampleDistanceFromEdgeY;
    auto sampleCenterX = columnWidth / 2.0f;
    auto sampleCenterY = rowHeight / 2.0f;
    
    for(unsigned int row = 0; row < RowCount; row++)
    {
        for(unsigned int column = 0; column < ColumnCount; column++)
        {
            auto expectedColor = expectedTileColors[row][column];
            auto x = static_cast<float>(column) * columnWidth;
            auto y = static_cast<float>(row) * rowHeight;

            ExpectColorAtScreenPosition(screenshot, "upper left",
                expectedColor, x + sampleLeftX, y + sampleUpperY, row, column);

            ExpectColorAtScreenPosition(screenshot, "upper right",
                expectedColor, x + sampleRightX, y + sampleUpperY, row, column);

            ExpectColorAtScreenPosition(screenshot, "center",
                expectedColor, x + sampleCenterX, y + sampleCenterY, row, column);

            ExpectColorAtScreenPosition(screenshot, "lower left",
                expectedColor, x + sampleLeftX, y + sampleLowerY, row, column);

            ExpectColorAtScreenPosition(screenshot, "lower right",
                expectedColor, x + sampleRightX, y + sampleLowerY, row, column);
        }
    }
}
