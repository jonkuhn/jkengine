#pragma once

#include "AfterBuildPtr.h"

namespace Graphics
{
    class ISprite;

    class SpriteDefinition final
    {
    public:
        SpriteDefinition(AfterBuildPtr<ISprite>* spriteAfterBuild, size_t layer)
            : _spriteAfterBuild(spriteAfterBuild),
              _layer(layer)
        {

        }

        inline size_t Layer() const { return _layer; }

        inline void SetAfterBuildPtr(ISprite* sprite) const
        {
            _spriteAfterBuild->Initialize(sprite);
        }

    private:
        AfterBuildPtr<ISprite>* _spriteAfterBuild;
        size_t _layer;
    };
}
