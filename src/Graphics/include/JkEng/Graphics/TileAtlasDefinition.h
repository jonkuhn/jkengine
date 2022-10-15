#pragma once

#include <stdexcept>
#include <sstream>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include <JkEng/AfterBuildPtr.h>
#include "SpriteDefinition.h"
#include "TileMapDefinition.h"

namespace JkEng::Graphics
{
    class IImage;

    class TileAtlasDefinition final
    {
    public:
        TileAtlasDefinition(size_t numberOfDrawingLayers, const IImage* image, glm::vec2 atlasSizeInTiles)
            : _image(image),
              _atlasSizeInTiles(std::move(atlasSizeInTiles)),
              _perLayerTileMapDefinitions(numberOfDrawingLayers),
              _perLayerSpriteDefinitions(numberOfDrawingLayers)
        {

        }

        inline void AddTileMap(TileMapDefinition tileMapDefinition)
        {
            if (tileMapDefinition.Layer() >= _perLayerTileMapDefinitions.size())
            {
                std::stringstream ss;
                ss << "Tile map layer index " << tileMapDefinition.Layer()
                    << " must be less than " << _perLayerTileMapDefinitions.size();
                throw std::out_of_range(ss.str());
            }

            _perLayerTileMapDefinitions[tileMapDefinition.Layer()]
                .push_back(std::move(tileMapDefinition));
        }

        inline void AddSprite(SpriteDefinition spriteDefinition)
        {
            if (spriteDefinition.Layer() >= _perLayerSpriteDefinitions.size())
            {
                std::stringstream ss;
                ss << "Tile map layer index " << spriteDefinition.Layer()
                    << " must be less than " << _perLayerSpriteDefinitions.size();
                throw std::out_of_range(ss.str());
            }

            _perLayerSpriteDefinitions[spriteDefinition.Layer()]
                .push_back(std::move(spriteDefinition));
        }

        inline const IImage& Image() const { return *_image; }
        inline const glm::vec2& AtlasSizeInTiles() const { return _atlasSizeInTiles; }
        inline size_t NumberOfDrawingLayers() const { return _perLayerSpriteDefinitions.size(); }

        inline const std::vector<TileMapDefinition>& TileMapDefinitionsForLayer(size_t layer) const
        {
            return _perLayerTileMapDefinitions[layer];
        }

        inline const std::vector<SpriteDefinition>& SpriteDefinitionsForLayer(size_t layer) const
        {
            return _perLayerSpriteDefinitions[layer];
        }

    private:
        const IImage* _image;
        glm::vec2 _atlasSizeInTiles;
        std::vector<std::vector<TileMapDefinition>> _perLayerTileMapDefinitions;
        std::vector<std::vector<SpriteDefinition>> _perLayerSpriteDefinitions;
    };
}
