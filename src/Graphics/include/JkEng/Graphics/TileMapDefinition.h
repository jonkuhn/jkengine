#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#pragma clang diagnostic pop

#include <JkEng/AfterCreatePtr.h>

namespace JkEng::Graphics
{
    class IImage;
    class ITileMap;

    class TileMapDefinition final
    {
    public:
        TileMapDefinition(AfterCreatePtr<ITileMap>* tileMapAfterBuild, size_t layer, const IImage* image)
            : _tileMapAfterBuild(tileMapAfterBuild),
              _image(image),
              _layer(layer)
        {

        }

        inline const IImage& Image() const { return *_image; }
        inline size_t Layer() const { return _layer; }
        inline void SetAfterCreatePtr(ITileMap* tileMap) const
        {
            _tileMapAfterBuild->Initialize(tileMap);
        }

    private:
        AfterCreatePtr<ITileMap>* _tileMapAfterBuild;
        const IImage* _image;
        size_t _layer;
    };
}
