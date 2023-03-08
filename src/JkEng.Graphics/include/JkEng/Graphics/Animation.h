#pragma once
#include <vector>

#include "GridLocation.h"

namespace JkEng::Graphics
{
    class Animation
    {
    public:
        Animation(std::vector<JkEng::Graphics::GridLocation> frames, float eachFrameDuration)
            : _currentFrameIndex(0),
              _currentFrameTime(0.0f),
              _eachFrameDuration(eachFrameDuration),
              _frames(std::move(frames))
        {
            if (_frames.size() == 0)
            {
                throw std::invalid_argument("frames must not be empty");
            }

            if (_eachFrameDuration <= 0)
            {
                throw std::invalid_argument("eachFrameDuration must be positive");
            }
        }
    
        void Update(float deltaTime, bool loop, float frameRateMultiplier);
    
        inline void Reset()
        {
            _currentFrameIndex = 0;
            _currentFrameTime = 0.0f;
        }
    
        inline const GridLocation& CurrentAtlasLocation() const
        {
            return _frames[_currentFrameIndex];
        }
    
        inline bool IsOnLastFrame() const
        {
            return _currentFrameIndex == (_frames.size() - 1);
        }
    
    private:
        size_t _currentFrameIndex;
        float _currentFrameTime;
        float _eachFrameDuration;
        std::vector<JkEng::Graphics::GridLocation> _frames;
        GridLocation _deleteMe;
    };

}
