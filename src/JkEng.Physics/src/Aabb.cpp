#include "Aabb.h"

using namespace JkEng::Physics;

void Aabb::SetPositionAndSize(const glm::vec2& position, const glm::vec2& size)
{
    _bottomLeft = position;
    _topRight = position + size;
}
