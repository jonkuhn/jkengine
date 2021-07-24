#include "ObjectInstance2d.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

using namespace Graphics::OpenGL;

void ObjectInstance2d::EnsureModelMatrixIsUpdated()
{
    if (!_modelNeedsUpdated)
    {
        return;
    }

    // The order here is very important.  Transformations take place
    // in the opposite order in which they appear here.
    // Scale is applied first.  Next, it is translated so that it is
    // centered at the origin so that rotation occurs around the middle.
    // Then, rotation is applied and it is translated back the amount it
    // was translated earlier so that position still cooresponds to 
    // to the lower left when not rotated.
    _model = glm::mat4(1.0f);
    _model = glm::translate(_model, _position);
    _model = glm::translate(_model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));  
    _model = glm::rotate(_model, glm::radians(_rotationDegrees), glm::vec3(0.0f, 0.0f, -1.0f));  
    _model = glm::translate(_model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));  
    _model = glm::scale(_model, glm::vec3(_size.x, _size.y, 0.0f));  

    _modelNeedsUpdated = false;
}