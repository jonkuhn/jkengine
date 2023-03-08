#pragma once

#include "Animation.h"
#include "ISprite.h"

namespace JkEng::Graphics
{
    class SpriteAnimator
    {
    public:
        SpriteAnimator(std::vector<Animation> animations)
          : _animations(std::move(animations)),
            _currentAnimationIndex(0),
            _frameRateMultiplier(1.0f),
            _isPlaying(false),
            _loop(false)
        {

        }

        inline size_t CurrentAnimationIndex()
        {
            return _currentAnimationIndex;
        }

        bool CurrentAnimationIsFinished();

        void Play(size_t animationIndex, bool loop, float frameRateMultiplier);
        void Pause();
        void Resume();
        void Update(JkEng::Graphics::ISprite& sprite, float deltaTime);

    private:
        std::vector<Animation> _animations;
        size_t _currentAnimationIndex;
        float _frameRateMultiplier;
        bool _isPlaying;
        bool _loop;
    };
}