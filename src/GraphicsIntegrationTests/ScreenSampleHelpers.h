#pragma once

#include <array>
#include <gtest/gtest.h>

#include "Graphics/Color.h"
#include "Graphics/IScreenshot.h"

template<unsigned int ColumnCount, unsigned int RowCount>
void ExpectTileColorGridOnScreen(
    const Graphics::IScreenshot& screenshot,
    std::array<std::array<Graphics::Color, ColumnCount>, RowCount> expectedTileColors)
{
    auto columnWidth = screenshot.Width() / ColumnCount;
    auto rowHeight = screenshot.Height() / RowCount;
    auto sampleDistanceFromEdgeX = columnWidth / 16;
    auto sampleDistanceFromEdgeY = rowHeight / 16;

    // sample each tile near 4 corners plus center
    auto sampleLeftX = sampleDistanceFromEdgeX;
    auto sampleUpperY = sampleDistanceFromEdgeY;
    auto sampleRightX = columnWidth - sampleDistanceFromEdgeX;
    auto sampleLowerY = rowHeight - sampleDistanceFromEdgeY;
    auto sampleCenterX = columnWidth / 2;
    auto sampleCenterY = rowHeight / 2;
    
    for(unsigned int row = 0; row < RowCount; row++)
    {
        for(unsigned int column = 0; column < ColumnCount; column++)
        {
            auto expectedColor = expectedTileColors[row][column];
            auto x = column * columnWidth;
            auto y = row * rowHeight;
            EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleLeftX, y + sampleUpperY))
                << "(upper left) row = " << row << " column = " << column;
            EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleRightX, y + sampleUpperY))
                << "(upper right) row = " << row << " column = " << column;
            EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleCenterX, y + sampleCenterY))
                << "(center) row = " << row << " column = " << column;
            EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleLeftX, y + sampleLowerY))
                << "(lower left) row = " << row << " column = " << column;
            EXPECT_EQ(expectedColor, screenshot.GetPixel(x + sampleRightX, y + sampleLowerY))
                << "(lower right) row = " << row << " column = " << column;
        }
    }
}
