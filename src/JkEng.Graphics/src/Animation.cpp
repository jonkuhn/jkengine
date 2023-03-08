#include "Animation.h"

using namespace JkEng::Graphics;

void Animation::Update(float deltaTime, bool loop, float frameRateMultiplier)
{
    _currentFrameTime += deltaTime;
    if (_currentFrameTime > (_eachFrameDuration / frameRateMultiplier))
    {
        _currentFrameTime = 0.0f;
        if (loop)
        {
            _currentFrameIndex = (_currentFrameIndex + 1) % _frames.size();
        }
        else if (_currentFrameIndex < (_frames.size() - 1))
        {
            _currentFrameIndex++;
        }
    }
}
