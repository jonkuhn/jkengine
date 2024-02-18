#include "OpenGL/Object2d.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

using namespace JkEng::Graphics::OpenGL;

void Object2d::EnsureModelMatrixIsUpdated() const
{
    if (!_modelNeedsUpdated)
    {
        return;
    }

    // The order here is very important.  Transformations take place
    // in the opposite order in which they appear here.
    // Scale and mirroring is applied first.  Next, if it was mirrored
    // it is translated back so the bottom left corner is at the origin
    //  Next, it is translated so that it is centered at the origin so
    // that rotation occurs around the middle.  Then, rotation is applied
    // and it is translated back the amount it was translated earlier so
    // that position still corresponds to to the lower left when not rotated.
    _model = glm::mat4(1.0f);
    _model = glm::translate(_model, _position);
    _model = glm::translate(_model, glm::vec3(0.5f * _size.x, 0.5f * _size.y, 0.0f));  
    _model = glm::rotate(_model, glm::radians(_rotationDegrees), glm::vec3(0.0f, 0.0f, -1.0f));  
    _model = glm::translate(_model, glm::vec3(-0.5f * _size.x, -0.5f * _size.y, 0.0f));  
    _model = glm::translate(
        _model,
        glm::vec3(
            _mirrorX ? _size.x : 0.0f,
            _mirrorY ? _size.y : 0.0f,
            0.0f)
    );  
    _model = glm::scale(
        _model,
        glm::vec3(
            _mirrorX ? -_size.x : _size.x,
            _mirrorY ? -_size.y : _size.y,
            0.0f)
    );

    _modelNeedsUpdated = false;
}