#pragma once

#include <JkEng/AfterCreatePtr.h>

namespace JkEng::Graphics
{
    class ISprite;

    class SpriteDefinition final
    {
    public:
        SpriteDefinition(AfterCreatePtr<ISprite>* spriteAfterBuild, size_t layer)
            : _spriteAfterBuild(spriteAfterBuild),
              _layer(layer)
        {

        }

        inline size_t Layer() const { return _layer; }

        inline void SetAfterCreatePtr(ISprite* sprite) const
        {
            _spriteAfterBuild->Initialize(sprite);
        }

    private:
        AfterCreatePtr<ISprite>* _spriteAfterBuild;
        size_t _layer;
    };
}
