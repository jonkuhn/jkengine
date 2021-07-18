#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma clang diagnostic pop

#include "../ICamera2d.h"

namespace Graphics::OpenGL
{
    class Camera2d : public ICamera2d
    {
    private:
        // A Z position of 1.0 will position the camera such that objects
        // positioned with Z < 1.0 will be in front of the camera
        static constexpr float CameraPositionZ = 1.0f;

        // A NearZ of 0 means that objects starting at the Z coordinate
        // of the camera position will be visible.
        static constexpr float NearZ = 0.0f;

        // A FarZ of 2.0 means that objects up to 2.0 Z coordinates in front
        // of the camera position will be visible.
        static constexpr float FarZ = 2.0f;

        // Front of camera points along the negative Z axis
        static constexpr glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        // Top of the camera points along the positive Y axis
        static constexpr glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

    public:
        Camera2d()
        {
            Center(glm::vec2(0.0f, 0.0f));
            FieldOfView(ICamera2d::Fov());
        }

        void Center(const glm::vec2& centerPosition) override
        {
            _position.x = centerPosition.x;
            _position.y = centerPosition.y;
            _viewMatrix = glm::lookAt(_position, _position + CameraFront, CameraUp);
        }

        glm::vec2 Center() override
        {
            return glm::vec2(_position);
        }

        void FieldOfView(ICamera2d::Fov fov) override
        {
            // The nearest visible Z coordinate is the same as the
            // Z coordinate of the camera position.
            float constexpr nearestVisibleToCameraZ = 0.0f;

            _fov = std::move(fov);

            // Set the Z coordinate of the camera position to be the
            // desired nearest visible Z coordinate
            _position.z = _fov.near;

            // Convert the desired furthest visible Z coordinate into
            // a distance from the camera.
            auto furthestVisibleFromCameraZ = _fov.near - _fov.far;

            _projectionMatrix = glm::ortho(
                _fov.left, _fov.right, _fov.bottom, _fov.top,
                nearestVisibleToCameraZ,
                furthestVisibleFromCameraZ);
        }

        const ICamera2d::Fov& FieldOfView() override
        {
            return _fov;
        }

        inline const glm::mat4& ViewMatrix()
        {
            return _viewMatrix;
        }

        inline const glm::mat4& ProjectionMatrix()
        {
            return _projectionMatrix;
        }

    private:
        ICamera2d::Fov _fov;

        glm::vec3 _position;
        glm::mat4 _viewMatrix;
        glm::mat4 _projectionMatrix;
    };
}
