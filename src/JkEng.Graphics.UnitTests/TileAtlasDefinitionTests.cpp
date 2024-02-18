#include <stdexcept>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "FakeImage.h"
#include <JkEng/Graphics/TileAtlasDefinition.h>

using namespace testing;
using JkEng::Graphics::TileAtlasDefinition;

class TileAtlasDefinitionTests : public Test
{

};

TEST_F(TileAtlasDefinitionTests, Constructor_GivenEachTileBorderThicknessInTilesIsValid_DoesNotThrow)
{
    FakeImage fakeImage(256, 256, JkEng::Graphics::IImage::PixelFormat::RGBA);
    glm::vec2 atlasSizeInTiles(8.0f, 8.0f);
    glm::vec2 tooBigBorder(0.1f, 0.1f);

    EXPECT_NO_THROW(
        TileAtlasDefinition tileAtlasDefinition(3, &fakeImage, atlasSizeInTiles, tooBigBorder)
    );
}

TEST_F(TileAtlasDefinitionTests, Constructor_GivenEachTileBorderThicknessInTilesXIs50PercentOfTileSize_ThrowsInvalidArgument)
{
    FakeImage fakeImage(256, 256, JkEng::Graphics::IImage::PixelFormat::RGBA);
    glm::vec2 atlasSizeInTiles(8.0f, 8.0f);
    glm::vec2 tooBigBorder(0.5f, 0.1f);

    EXPECT_THROW(
        try
        {
            TileAtlasDefinition tileAtlasDefinition(3, &fakeImage, atlasSizeInTiles, tooBigBorder);
        }
        catch (std::runtime_error& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("eachTileBorderThicknessInTiles"));
            throw;
        },
        std::invalid_argument);
}

TEST_F(TileAtlasDefinitionTests, Constructor_GivenEachTileBorderThicknessInTilesXIsOver50PercentOfTileSize_ThrowsInvalidArgument)
{
    FakeImage fakeImage(256, 256, JkEng::Graphics::IImage::PixelFormat::RGBA);
    glm::vec2 atlasSizeInTiles(8.0f, 8.0f);
    glm::vec2 tooBigBorder(0.7f, 0.1f);

    EXPECT_THROW(
        try
        {
            TileAtlasDefinition tileAtlasDefinition(3, &fakeImage, atlasSizeInTiles, tooBigBorder);
        }
        catch (std::runtime_error& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("eachTileBorderThicknessInTiles"));
            throw;
        },
        std::invalid_argument);
}

TEST_F(TileAtlasDefinitionTests, Constructor_GivenEachTileBorderThicknessInTilesYIs50PercentOfTileSize_ThrowsInvalidArgument)
{
    FakeImage fakeImage(256, 256, JkEng::Graphics::IImage::PixelFormat::RGBA);
    glm::vec2 atlasSizeInTiles(8.0f, 8.0f);
    glm::vec2 tooBigBorder(0.1f, 0.5f);

    EXPECT_THROW(
        try
        {
            TileAtlasDefinition tileAtlasDefinition(3, &fakeImage, atlasSizeInTiles, tooBigBorder);
        }
        catch (std::runtime_error& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("eachTileBorderThicknessInTiles"));
            throw;
        },
        std::invalid_argument);
}

TEST_F(TileAtlasDefinitionTests, Constructor_GivenEachTileBorderThicknessInTilesYIsOver50PercentOfTileSize_ThrowsInvalidArgument)
{
    FakeImage fakeImage(256, 256, JkEng::Graphics::IImage::PixelFormat::RGBA);
    glm::vec2 atlasSizeInTiles(8.0f, 8.0f);
    glm::vec2 tooBigBorder(0.1f, 0.7f);

    EXPECT_THROW(
        try
        {
            TileAtlasDefinition tileAtlasDefinition(3, &fakeImage, atlasSizeInTiles, tooBigBorder);
        }
        catch (std::runtime_error& e)
        {
            EXPECT_THAT(e.what(), HasSubstr("eachTileBorderThicknessInTiles"));
            throw;
        },
        std::invalid_argument);
}
