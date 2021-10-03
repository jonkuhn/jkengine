#include "Aabb.h"

using namespace JkEng::Physics;

void Aabb::SetPositionAndSize(const glm::vec2& position, const glm::vec2& size)
{
    _leftXMin = position.x;
    _rightXMax = position.x + size.x;
    _bottomYMin = position.y;
    _topYMax = position.y + size.y;
}
