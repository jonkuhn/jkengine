#pragma once

#include <stdexcept>
#include <sstream>

#include "TileAtlasDefinition.h"

namespace Graphics
{
    class SceneDefinition final
    {
    public:

        SceneDefinition(size_t numberOfDrawingLayers)
            : _numberOfDrawingLayers(numberOfDrawingLayers)
            
        {

        }

        inline size_t NumberOfDrawingLayers() const
        {
            return _numberOfDrawingLayers;
        }

        inline void AddTileAtlas(TileAtlasDefinition tileAtlasDefinition)
        {
            if (tileAtlasDefinition.NumberOfDrawingLayers() != _numberOfDrawingLayers)
            {
                std::stringstream ss;
                ss << "TileAtlas layer count " << tileAtlasDefinition.NumberOfDrawingLayers()
                    << " must be equal to SceneDefinition layer count "
                    << _numberOfDrawingLayers;
                throw std::out_of_range(ss.str());
            }
            _tileAtlasDefinitions.push_back(std::move(tileAtlasDefinition));
        }

        inline const std::vector<TileAtlasDefinition>& TileAtlasDefinitions() const
        {
            return _tileAtlasDefinitions;
        }

    private:
        size_t _numberOfDrawingLayers;
        std::vector<TileAtlasDefinition> _tileAtlasDefinitions;
    };
}
