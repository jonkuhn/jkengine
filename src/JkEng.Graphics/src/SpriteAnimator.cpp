#include "SpriteAnimator.h"

using namespace JkEng::Graphics;

bool SpriteAnimator::CurrentAnimationIsFinished()
{
    if (_loop)
    {
        // A looping animation is never considered to be finished
        return false;
    }
    return _animations[_currentAnimationIndex].IsOnLastFrame();
}

void SpriteAnimator::Play(size_t animationIndex, bool loop, float frameRateMultiplier)
{
    if (animationIndex != _currentAnimationIndex)
    {
        // Clear the animation that was current so that if/when
        // we switch back to it it starts at the beginning.
        //
        // NOTE: The animation we are switching _to_ can _also_ be
        // expected to be at the beginning because they start at
        // the beginning and this causes them to be reset to the
        // beginning every time we switch away.
        _animations[_currentAnimationIndex].Reset();
    }

    _isPlaying = true;
    _loop = loop;
    _currentAnimationIndex = animationIndex;
    _frameRateMultiplier = frameRateMultiplier;
    
}

void SpriteAnimator::Pause()
{
    _isPlaying = false;
}

void SpriteAnimator::Resume()
{
    _isPlaying = true;
}

void SpriteAnimator::Update(JkEng::Graphics::ISprite& sprite, float deltaTime)
{
    if (_isPlaying)
    {
        _animations[_currentAnimationIndex].Update(deltaTime, _loop, _frameRateMultiplier);
    }
    sprite.AtlasLocation(_animations[_currentAnimationIndex].CurrentAtlasLocation());
}
